#ifndef STREAMING_UNIT_HPP
#define STREAMING_UNIT_HPP

#include <array>
#include <cstddef> // std::size_t
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <variant>
#include <iostream>
#include <vector>
#include <type_traits>
#include "Helpers.h"
#include "Dimension.h"


/* Necessary for using MMU */
class processor_t;
extern processor_t *globalProcessor;
#define gMMU (*(globalProcessor->get_mmu()))

enum class RegisterType { Temporary, Load, Store};
enum class RegisterStatus { NotConfigured, Running, Finished };

/* T is one of std::uint8_t, std::uint16_t, std::uint32_t or std::uint64_t and
  represents the type of the elements of a stream at a given moment. It is the
  type configured to store a value at a given time */
template <typename T>
struct StreamRegister
{
  /* The gem5 implementation was made with 64 bytes, so this value mirrors it.
  It can be changed to another value that is a power of 2, with atleast 8 bytes
  to support at the 64-bit operations */
  static constexpr size_t registerLength = 64; // in Bytes
  /* Last dimension cannot have a modifier */
  static constexpr size_t maxDimensions = 8;
  static constexpr size_t maxModifiers = maxDimensions - 1;

  using ElementsType = T;
  /* During computations, we test if two streams have the same element width
  using this property */
  static constexpr size_t elementsWidth = sizeof(ElementsType);
  /* This property limits how many elements can be manipulated during a
  computation and also how many can be loaded/stored at a time */
  static constexpr size_t maxAmountElements = registerLength / elementsWidth;


  StreamRegister(RegisterType t = RegisterType::Temporary)
    : modifiers{}, type(t)
  {
    status = RegisterStatus::NotConfigured;
    configEOD = false;
  }

  void addModifier(Modifier mod)
  {
    /* A recently added modifier alters the last inserted dimension */
    const auto modIndex = dimensions.size() - 2;
    auto iter = modifiers.find(modIndex);
    assert_msg("Trying to add a modifier, when one already exists in this index", iter == modifiers.end());
    modifiers.insert({ modIndex, mod });
  }

  void addDimension(Dimension dim)
  {
    assert_msg("Cannot append more dimensions as the max value was reached", dimensions.size() < maxDimensions);
    dimensions.push_back(dim);
    // print dimensions
    // std::cout << "Added dimension with offset " << dim.offset << ", size " << dim.size << " and stride " << dim.stride << std::endl;
    // print dimensions size
    // std::cout << "Dimensions size: " << dimensions.size() << std::endl;
  }

  void startConfiguration(Dimension dim)
  {
    status = RegisterStatus::NotConfigured;
    dimensions.clear();
    dimensions.push_back(dim);
  }

  void endConfiguration()
  {
    status = RegisterStatus::Running;
    if (this->type == RegisterType::Load) {
      updateAsLoad();
    }
  }

  std::vector<ElementsType> getElements(bool causesUpdate)
  {
    assert_msg("Trying to get values from a store stream", type != RegisterType::Store);

    auto ret = elements;
    if (causesUpdate) {
      updateStreamValues();
    }
    return ret;
  }

  void setElements(bool causesUpdate, std::vector<ElementsType> e)
  {

    //std::cout << "Setting elements to " << e.size() << " elements" << std::endl;

    assert_msg("Trying to set values to a load stream", type != RegisterType::Load);

    elements = e;

    // print elements
    /*std::cout << "Elements: ";
    for (auto& e : elements) {
      std::cout << e << " ";
    }
    */
    if (causesUpdate){
      //std::cout << "Updating stream values (if stream)" << std::endl;
      updateStreamValues();
    }
  }

  bool hasStreamFinished() const
  {
    return status == RegisterStatus::Finished;
  }

  void clearEndOfDimensionOfDim(std::size_t i)
  {
    assert_msg("Trying to clear eof of invalid dimension", i < dimensions.size() - 1);
    /* Cannot clear this flag when the stream has finished, it would prevent us from knowing that */
    if (isStreamDone())
      return;
    dimensions.at(i).setEndOfDimension(false);
  }

  bool isEndOfDimensionOfDim(std::size_t i) const
  {
    assert_msg("Trying to check eof of invalid dimension", i < dimensions.size() - 1);
    std::cout << "Checking eof of dimension " << i << " = " << dimensions.at(i).isEndOfDimension() << std::endl;
    return dimensions.at(i).isEndOfDimension();
  }

  void tryIterate()
  {
    if (canIterate()) {
      updateIteration();
    }
  }

  RegisterStatus getStatus() const {
      return status;
  }
  std::size_t getElementsWidth() const {
      return elementsWidth;
  }
  std::size_t getMaxElements() const {
      return maxAmountElements; 
  }
  RegisterType getType() const {
      return type;
  }

private:
  /* Although the more semantically correct choice would be an array, using a
  vector allows us to avoid an index pointer to the last valid element. This design
  allows us to assume every element in the container is valid and any index checking
  operations can be done by calling the vector::size() method */
  std::vector<ElementsType> elements;
  /* Same ordeal as above. Although the amount of dimensions is capped, we can avoid
  indexing by just calling the size method */
  std::vector<Dimension> dimensions;
  /* Modifiers are different in that they don't have to scale linearly in a stream
  configuration. As such, it is better to have a container that maps a dimension's
  index to its modifier. When updating stream the iterators, we can test if a dimension
  for the given index exists before the calculations */
  std::unordered_map<int, Modifier> modifiers;
  RegisterType type;
  RegisterStatus status;

  void updateStreamValues()
  {
    if (this->type == RegisterType::Load) {
      updateAsLoad();
    }
    else if (this->type == RegisterType::Store) {

      updateAsStore();
    }
    else if (this->type == RegisterType::Temporary) {
      // do nothing
    }
    else {
      assert_msg("Unhandled type of stream was asked to update", false);
    }
  }


  size_t generateOffset()
  {
    /* Result will be the final accumulation of all offsets calculated per dimension */
    std::size_t init = 0;
    return std::accumulate(dimensions.begin(), dimensions.end(), init, [](size_t acc, Dimension& dim) {
      dim.setEndOfDimension(dim.isLastIteration());
      return acc + dim.calcOffset(elementsWidth);
    });
  }

  bool isDimensionFullyDone(const std::vector<Dimension>::const_iterator start, const std::vector<Dimension>::const_iterator end) const
  {
    return std::accumulate(start, end, true, [](bool acc, const Dimension& dim) { return acc && dim.isEndOfDimension(); });
  }

  bool isStreamDone() const
  {
    return isDimensionFullyDone(dimensions.begin(), dimensions.end());
  }

  bool canGenerateOffset() const
  {
    /* There are two situations that prevent us from generating offsets:
    1) We are at the last iteration of the outermost dimension
    2) We just finished the last iteration of a dimension and there is a configure
    stream vector modifier at that same dimension. In these cases, the generation can
    only resume after an exterior call to resetIndex() */

    /* The outermost dimension is the last one in the container */
    if (isStreamDone()) {
      return false;
    }
 /*
    // We don't check the last dimension as it cannot have a modifier attached
    for (size_t i = 0; i < dimensions.size() - 1; i++) {
      // auto& currDim = dimensions.at(i);
      auto currentModifierIterator = modifiers.find(i);
      const bool dimensionsDone = isDimensionFullyDone(dimensions.begin(), dimensions.begin() + i + 1);
      if (dimensionsDone && currentModifierIterator != modifiers.end()) {
        auto type = currentModifierIterator->second.getType();
        if (type == Modifier::Type::CfgVec) {
          return false;
        }
      }
    }
*/
    return true;
  }

  void updateIteration()
  {
    if (isStreamDone()) {
      return;
    }
    /* Iteration starts from the innermost dimension and updates the next if the current
    reaches an overflow; at which point, for the current dimension, the values are reset */
    dimensions.at(0).advance();

    /* No extra processing is needed if there is only 1 dimension */
    if (dimensions.size() == 1) {
      return;
    }
    //std::cout << "Updating iteration. Dimensions: " << dimensions.size() << std::endl;
    for (size_t i = 0; i < dimensions.size() - 1; i++) {
      auto& currDim = dimensions.at(i);
      /* The following calculations are only necessary if we ARE in the
      last iteration of a dimension */
      if (!currDim.triggerIterationUpdate())
        continue;
      //std::cout << "Looking for modifiers of dimension " << i << std::endl;
      auto currentModifierIter = modifiers.find(i);
      const bool modifierExists = currentModifierIter != modifiers.end();

      // print std::unordered_map currentMofifierIter
      // Helper lambda function to print key-value pairs
      auto print_key_value = [](const auto& key, const auto& value)
      {
          std::cout << "Key:[" << key << "] Value:[";
          value.printModifier();
          std::cout << "]\n";
      };

      // print std::unordered_map
      //std::cout << "modifiers contains:\n";
      //for (auto& p : modifiers) print_key_value(p.first, p.second);

      currDim.resetIndex();
      currDim.setEndOfDimension(false);
      dimensions.at(i + 1).advance();
      if (modifierExists) {
        //std::cout << "Updating modifier" << std::endl;
        currentModifierIter->second.modDimension(currDim);
      }
      // The values at lower dimensions might have been modified. As such, we need to reset them before next iteration
      for (size_t j = 0; j < i; j++) {
        dimensions.at(j).resetIterValues();
      }
    }
  }

  bool canIterate() const
  {
    if (isStreamDone()) {
      return false;
    }
  /*
    for (const auto& [key, modifier] : modifiers) {
      if (modifier.getType() == Modifier::Type::CfgVec) {
        if (isDimensionFullyDone(dimensions.begin(), dimensions.begin() + key + 1)) {
          return false;
        }
      }
    }
  */
    return true;
  }

  void clearAllEODFlags()
  {
    for (auto& dim : dimensions) {
      dim.setEndOfDimension(false);
    }
  }

  void updateAsLoad()
  {
    if (isStreamDone()) {
      status = RegisterStatus::Finished;
      return;
    }
    elements.clear();
    elements.reserve(maxAmountElements);

    std::size_t eCount = maxAmountElements;
    while (eCount > 0 && canGenerateOffset()) {
      //std::cout << "Iteration: " << eCount << std::endl;
      std::size_t offset = generateOffset();
      //std::cout << "Offset: " << offset << std::endl;
      auto value = [](auto address) -> ElementsType {
          if constexpr (std::is_same_v<ElementsType, std::uint8_t>)
              return readAS<ElementsType>(gMMU.template load<std::uint8_t>(address));
          else if constexpr (std::is_same_v<ElementsType, std::uint16_t>)
              return readAS<ElementsType>(gMMU.template load<std::uint16_t>(address));
          else if constexpr (std::is_same_v<ElementsType, std::uint32_t>)
              return readAS<ElementsType>(gMMU.template load<std::uint32_t>(address));
          else
              return readAS<ElementsType>(gMMU.template load<std::uint64_t>(address));
      }(offset);
      elements.push_back(value);

      eCount--;
      if (canIterate())
        updateIteration();
    }
  }

  void updateAsStore()
  {
    if (isStreamDone()) {
      status = RegisterStatus::Finished;
      //std::cout << "Stream finished" << std::endl;
      return;
    }

    //std::cout << "Storing " << elements.size() << " elements. eCount=" << maxAmountElements << std::endl;

    std::size_t eCount = std::min(maxAmountElements, elements.size());
    while (eCount > 0 && canGenerateOffset()) {
      //std::cout << "Generating offset" << std::endl;
      std::size_t offset = generateOffset();
      //std::cout << "Offset: " << offset << std::endl;
      auto value = elements.front();
      elements.erase(elements.begin());
      if constexpr (std::is_same_v<ElementsType, std::uint8_t>)
              gMMU.template store<std::uint8_t>(offset, readAS<ElementsType>(value));
      else if constexpr (std::is_same_v<ElementsType, std::uint16_t>)
              gMMU.template store<std::uint16_t>(offset, readAS<ElementsType>(value));
      else if constexpr (std::is_same_v<ElementsType, std::uint32_t>)
              gMMU.template store<std::uint32_t>(offset, readAS<ElementsType>(value));
      else
              gMMU.template store<std::uint64_t>(offset, readAS<ElementsType>(value));
      eCount--;
      if (canIterate())
        updateIteration();
    }
    elements.clear();
  }

};

template <typename T>
struct PredRegister
{
  static constexpr size_t registerLength = 64; // in Bytes
  using ElementsType = T;
  static constexpr size_t elementsWidth = sizeof(ElementsType);
  static constexpr size_t maxAmountElements = registerLength / elementsWidth;

  PredRegister(std::vector<ElementsType> e)
  {
    elements = e;
  }

  private:
  std::vector<ElementsType> elements;
};

using StreamReg8 = StreamRegister<std::uint8_t>;
using StreamReg16 = StreamRegister<std::uint16_t>;
using StreamReg32 = StreamRegister<std::uint32_t>;
using StreamReg64 = StreamRegister<std::uint64_t>;

using PredReg8 = PredRegister<std::uint8_t>;
using PredReg16 = PredRegister<std::uint16_t>;
using PredReg32 = PredRegister<std::uint32_t>;
using PredReg64 = PredRegister<std::uint64_t>;

struct StreamingUnit
{
  /* UVE specification is to have 32 streaming/vectorial registers */
  static constexpr std::size_t registerCount = 32;
  static constexpr std::size_t predRegCount = 16;
  /* There are 2 types at play when implementing the UVE specification. A storage
  type, which is how values get stored, and a computation type, the type a value
  should have when doing computations. Using a variant allows us to have almost
  full type-safety when storing/retriving values, including how many elements can
  be contained in a register at a given moment. During computations, we might need
  a raw cast to a signed or floating-point value.  */
  using RegisterType = std::variant<StreamReg8, StreamReg16, StreamReg32, StreamReg64>;
  using PredType = std::variant<PredReg8, PredReg16, PredReg32, PredReg64>;
  /* Property defined as public so that this class can be small and have most
  operations happen outside of it */
  std::array<RegisterType, registerCount> registers;
  std::array<PredType, registerCount> predicates;
};

template <typename T, typename Arg = T>
auto makeStreamRegister(RegisterType type = RegisterType::Temporary)
{
  if constexpr (std::is_same_v<T, std::uint8_t>) {
    return StreamReg8{type};
  }
  else if constexpr (std::is_same_v<T, std::uint16_t>) {
    return StreamReg16{type};
  }
  else if constexpr (std::is_same_v<T, std::uint32_t>) {
    return StreamReg32{type};
  }
  else if constexpr (std::is_same_v<T, std::uint64_t>) {
    return StreamReg64{type};
  }
  else {
    static_assert(always_false_v<T>,
      "Cannot create register with this element width");
  }
}

template <typename T, typename Arg = T>
auto makePredRegister(std::vector<T> elements)
{
  if constexpr (std::is_same_v<T, std::uint8_t>) {
    return PredReg8{elements};
  }
  else if constexpr (std::is_same_v<T, std::uint16_t>) {
    return PredReg16{elements};
  }
  else if constexpr (std::is_same_v<T, std::uint32_t>) {
    return PredReg32{elements};
  }
  else if constexpr (std::is_same_v<T, std::uint64_t>) {
    return PredReg64{elements};
  }
  else {
    static_assert(always_false_v<T>,
      "Cannot create predicate register with this element width");
  }
}

#endif // STREAMING_UNIT_HPP
