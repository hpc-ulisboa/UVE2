#ifndef STREAMING_UNIT_HPP
#define STREAMING_UNIT_HPP

#include "dimension.h"
//#include "processor.h"
#include "helpers.h"
#include <algorithm>
#include <array>
#include <cstddef> // std::size_t
#include <iostream>
#include <numeric>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

/* Necessary for using MMU */
class processor_t;
class streamingUnit_t;
// extern processor_t *globalProcessor;
#define gMMU(p) (*(p->get_mmu()))

/* --- Streaming Registers --- */

enum class RegisterConfig { Temporary,
                            Load,
                            Store };
enum class RegisterStatus { NotConfigured,
                            Running,
                            Finished };

/* T is one of std::uint8_t, std::uint16_t, std::uint32_t or std::uint64_t and
  represents the type of the elements of a stream at a given moment. It is the
  type configured to store a value at a given time */
template <typename T>
class streamRegister_t {
    streamingUnit_t *su;
    /* FOR DEBUGGING*/
    std::size_t registerN;
    /* Although the more semantically correct choice would be an array, using a
    vector allows us to avoid an index pointer to the last valid element. This design
    allows us to assume every element in the container is valid and any index checking
    operations can be done by calling the vector::size() method */
    std::vector<T> elements;
    /* Same ordeal as above. Although the amount of dimensions is capped, we can avoid
    indexing by just calling the size method */
    std::vector<Dimension> dimensions;
    /* Modifiers are different in that they don't have to scale linearly in a stream
    configuration. As such, it is better to have a container that maps a dimension's
    index to its modifier. When updating stream the iterators, we can test if a dimension
    for the given index exists before the calculations */
    std::unordered_map<int, Modifier> modifiers;
    RegisterConfig type;
    RegisterStatus status;
    /* This structure holds an array of bits indicating whether the corresponding dimension
    is configured to only load elements while the current dimension is not over or not. It
    is controlled using the instruction ss_cfg_vec */
    std::vector<bool> vecCfg;

    void updateStreamValues();
    size_t generateOffset();
    bool isDimensionFullyDone(const std::vector<Dimension>::const_iterator start, const std::vector<Dimension>::const_iterator end) const;
    bool isStreamDone() const;
    bool canGenerateOffset() const;
    void updateIteration(bool clearEOD = true);
    void updateAsLoad();
    void updateAsStore();

public:
    using ElementsType = T;
    /* The gem5 implementation was made with 64 bytes, so this value mirrors it.
    It can be changed to another value that is a power of 2, with atleast 8 bytes
    to support at the 64-bit operations */
    static constexpr size_t registerLength = 64; // in Bytes
    /* During computations, we test if two streams have the same element width
    using this property */
    static constexpr size_t elementsWidth = sizeof(T);
    /* This property limits how many elements can be manipulated during a
    computation and also how many can be loaded/stored at a time */
    static constexpr size_t maxAmountElements = registerLength / elementsWidth;

    /* In this implementation, the concept of a stream and register are heavily
    intertwined. As such, stream attributes, such as dimensions, modifiers, EOD flags,
    status and configuration are associated with the registers holding each stream.
    In real hardware, they would be independent, with streams being merely associated
    with a certain register and values being first loaded to a buffer in the
    Streaming Engine and then loaded to the correspondent register.
    Because here the stream load and store operations are synchronous to the
    rest of the execution, such is not necessary and a stream is loaded directly
    to the register that holds all its information.*/
    /* Last dimension cannot have a modifier */
    //static constexpr size_t maxDimensions = 8;
    //static constexpr size_t maxModifiers = su->maxDimensions - 1;

    streamRegister_t(streamingUnit_t *su = nullptr, RegisterConfig t = RegisterConfig::Temporary, std::size_t regN = -1) : su(su), registerN(regN), type(t) {
        status = RegisterStatus::NotConfigured;
    }

    void addModifier(Modifier mod);
    void addDimension(Dimension dim);
    void configureDim();
    void startConfiguration(Dimension dim);
    void endConfiguration();
    std::vector<T> getElements(bool causesUpdate);
    void setElements(bool causesUpdate, std::vector<T> e);
    bool hasStreamFinished() const;
    void clearEndOfDimensionOfDim(std::size_t i);
    bool isEndOfDimensionOfDim(std::size_t i) const;

    RegisterStatus getStatus() const;
    std::size_t getElementsWidth() const;
    std::size_t getMaxElements() const;
    RegisterConfig getType() const;

    /* FOR DEBUGGING*/
    void printRegN(char *str = "");

    friend class streamingUnit_t;
};

/* --- Predicate Registers --- */

struct PredRegister {
    static constexpr size_t registerLength = 64; // in Bytes
    static constexpr size_t elementsWidth = sizeof(uint8_t);
    static constexpr size_t maxAmountElements = registerLength / elementsWidth;

    PredRegister(std::vector<uint8_t> e = {}) {
        elements = e;
    }

private:
    std::vector<uint8_t> elements;

    friend class streamingUnit_t;
};

/* --- Streaming Unit --- */

using StreamReg8 = streamRegister_t<std::uint8_t>;
using StreamReg16 = streamRegister_t<std::uint16_t>;
using StreamReg32 = streamRegister_t<std::uint32_t>;
using StreamReg64 = streamRegister_t<std::uint64_t>;

struct streamingUnit_t {
    processor_t *p; // updated by the processor in processor.cc (ln 44)
    /* UVE specification is to have 32 streaming/vectorial registers */
    static constexpr std::size_t registerCount = 32;
    static constexpr std::size_t predRegCount = 16;
    static constexpr size_t maxDimensions = 8;
    /* There are 2 types at play when implementing the UVE specification. A storage
    type, which is how values get stored, and a computation type, the type a value
    should have when doing computations. Using a variant allows us to have almost
    full type-safety when storing/retriving values, including how many elements can
    be contained in a register at a given moment. During computations, we might need
    a raw cast to a signed or floating-point value.  */
    using RegisterType = std::variant<StreamReg8, StreamReg16, StreamReg32, StreamReg64>;

    std::array<std::array<bool, maxDimensions>, registerCount> EODTable;

    std::array<RegisterType, registerCount> registers;
    std::array<PredRegister, predRegCount> predicates;

    streamingUnit_t() : p(nullptr), registers{RegisterType{}}, predicates{PredRegister{}} {
        std::vector<uint8_t> ones(predicates.at(0).maxAmountElements, 1);
        predicates.at(0).elements = ones;
    }

    template <typename T>
    void makeStreamRegister(RegisterConfig type = RegisterConfig::Temporary, std::size_t streamRegister = -1);

    void makePredRegister(std::vector<uint8_t> elements, std::size_t predRegister = -1);

    void updateEODTable(const std::size_t stream);
};

#endif // STREAMING_UNIT_HPP
