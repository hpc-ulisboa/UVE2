#ifndef STREAMING_UNIT_HPP
#define STREAMING_UNIT_HPP

#include "descriptors.h"
#include "helpers.h"
#include <algorithm>
#include <array>
#include <cstddef> // size_t
#include <deque>
#include <iostream>
#include <memory>
#include <numeric>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

/* Necessary for using MMU */
class processor_t;
//class streamingUnit_t;

// extern processor_t *globalProcessor;
#define gMMU(p) (*(p->get_mmu()))

/* --- Streaming Registers --- */

enum class RegisterConfig { NoStream,
                            Load,
                            Store };
enum class RegisterStatus { NotConfigured,
                            Running,
                            Finished };
enum class RegisterMode { Vector,
                          Scalar };

/* T is one of std::uint8_t, std::uint16_t, std::uint32_t or std::uint64_t and
  represents the type of the elements of a stream at a given moment. It is the
  type configured to store a value at a given time */
template <typename T>
struct streamRegister_t {
    using ElementsType = T;
    /* The gem5 implementation was made with 64 bytes, so this value mirrors it.
    It can be changed to another value that is a power of 2, with atleast 8 bytes
    to support at the 64-bit operations */
    static constexpr size_t registerLength = 64; // in Bytes
    //static constexpr size_t registerLength = 16; // in Bytes
    /* During computations, we test if two streams have the same element width
    using this property */
    static constexpr size_t elementWidth = sizeof(ElementsType);
    /* This property limits how many elements can be manipulated during a
    computation and also how many can be loaded/stored at a time */
    static constexpr size_t vLen = registerLength / elementWidth;

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
    // static constexpr size_t maxDimensions = 8;
    // static constexpr size_t maxModifiers = su->maxDimensions - 1;

    streamRegister_t(streamingUnit_t *su = nullptr, RegisterConfig t = RegisterConfig::NoStream, size_t regN = -1) :
     su(su), registerN(regN), type(t) {
        status = RegisterStatus::NotConfigured;
        mode = RegisterMode::Vector;
        validIndex = vLen;
    }

    void addModifier(std::shared_ptr<Modifier> mod);
    void addDimension(Dimension dim);
    void configureDim();
    void startConfiguration(Dimension dim);
    void endConfiguration();
    std::vector<ElementsType> getElements(bool causesUpdate);
    void setElements(bool causesUpdate, std::vector<ElementsType> e);
    void setValidIndex(const size_t i);
    void setMode(const RegisterMode m);
    bool hasStreamFinished() const;
    //void clearEndOfDimensionOfDim(size_t i);
    bool isEndOfDimensionOfDim(size_t i) const;

    size_t getelementWidth() const;
    size_t getVLen() const;
    size_t getValidIndex() const;
    size_t getRegisterLength() const;
    RegisterConfig getType() const;
    RegisterStatus getStatus() const;
    RegisterMode getMode() const;

    /* FOR DEBUGGING*/
    void printRegN(char *str = "");

    friend class streamingUnit_t;

private:
    streamingUnit_t *su;
    /* FOR DEBUGGING */
    size_t registerN;
    std::vector<ElementsType> elements = std::vector<ElementsType>(vLen);
    size_t validIndex;
    /* Same ordeal as above. Although the amount of dimensions is capped, we can avoid
    indexing by just calling the size method */
    std::deque<Dimension> dimensions;
    /* Modifiers are different in that they don't have to scale linearly in a stream
    configuration. As such, it is better to have a container that maps a dimension's
    index to its modifier. When updating stream the iterators, we can test if a dimension
    for the given index exists before the calculations */
    std::unordered_multimap<int, std::shared_ptr<Modifier>> modifiers;
    RegisterConfig type;
    RegisterStatus status;
    RegisterMode mode;
    /* This structure holds an array of bits indicating whether the corresponding dimension
    is configured to only load elements while the current dimension is not over or not. It
    is controlled using the instruction ss_cfg_vec */
    std::deque<bool> vecCfg;

    size_t generateOffset();
    bool isDimensionFullyDone(const std::deque<Dimension>::const_iterator start, const std::deque<Dimension>::const_iterator end) const;
    bool isStreamDone() const;
    bool tryGenerateOffset(size_t &address);
    void updateIteration();
    void updateAsLoad();
    void updateAsStore();
};

/* --- Predicate Registers --- */

struct PredRegister {
    static constexpr size_t registerLength = 64; // in Bytes
    static constexpr size_t elementWidth = sizeof(uint8_t);
    static constexpr size_t vLen = registerLength / elementWidth;

    PredRegister(std::vector<uint8_t> e = std::vector<uint8_t>(vLen)) {
        elements = e;
    }

    std::vector<uint8_t> getPredicate() const;

private:
    std::vector<uint8_t> elements = std::vector<uint8_t>(vLen);

    friend class streamingUnit_t;
};

/* --- Streaming Unit --- */

using StreamReg8 = streamRegister_t<std::uint8_t>;
using StreamReg16 = streamRegister_t<std::uint16_t>;
using StreamReg32 = streamRegister_t<std::uint32_t>;
using StreamReg64 = streamRegister_t<std::uint64_t>;

struct streamingUnit_t {
    processor_t *p = nullptr; // updated by the processor in processor.cc (ln 44)
    /* UVE specification is to have 32 streaming/vectorial registers */
    static constexpr size_t registerCount = 32;
    static constexpr size_t predRegCount = 16;
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

    streamingUnit_t() {
        predicates.at(0).elements = std::vector<uint8_t>(predicates.at(0).vLen, 1);
    }

    template <typename T>
    void makeStreamRegister(size_t streamRegister = -1, RegisterConfig type = RegisterConfig::NoStream);

    void makePredRegister(std::vector<uint8_t> elements, size_t predRegister = -1);

    void updateEODTable(const size_t stream);

    template <typename Operation>
    auto operateRegister(size_t streamRegister, Operation &&op) {
        assert_msg("Tried to use a register index higher than the available registers.", streamRegister < registerCount);
        return std::visit([op = std::move(op)](auto &reg) { return op(reg); }, registers.at(streamRegister));
    }

    template <typename Operation>
    auto operatePredReg(size_t predRegister, Operation &&op) {
        assert_msg("Tried to use a predicate register index higher than the available predicate registers.", predRegister < predRegCount);
        return std::visit([op = std::move(op)](auto &reg) { return op(reg); }, predicates.at(predRegister));
    }
};

#endif // STREAMING_UNIT_HPP
