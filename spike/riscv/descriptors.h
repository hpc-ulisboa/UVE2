#ifndef DIMENSION_HPP
#define DIMENSION_HPP

#include "helpers.h"
//#include "streaming_unit.h"
#include <cstddef> // size_t

class streamingUnit_t;

struct Dimension {
    Dimension(uint64_t offset, unsigned int size, int stride)
        : offset(offset), size(size), stride(stride) {

        iter_offset = offset;
        iter_size = size;
        iter_stride = stride;
        iter_index = 0;
        endOfDimension = iter_size == 0;
        modApplied = false;
        // endOfDimension = false;
        // std::cout << "offset: " << offset << ", size: " << size << ", stride: "
        // << stride << std::endl;
    }

    // void resetIndex();

    void resetIterValues();

    bool isEmpty() const;

    void advance();

    bool isLastIteration() const;

    bool isModApplied() const;

    bool isEndOfDimension() const;

    void setEndOfDimension(bool b);

    size_t calcOffset(size_t width) const;

    size_t getSize() const;

private:
    const size_t offset;
    const size_t size;
    const size_t stride;
    size_t iter_offset;
    size_t iter_size;
    size_t iter_stride;
    size_t iter_index;
    bool endOfDimension;
    bool modApplied;

    //friend class Modifier;
	friend class StaticModifier;
	friend class DynamicModifier;
};

enum class Target {
    Offset,
    Size,
    Stride
};

enum class Behaviour {
    Increment,
    Decrement,
    Set,
    Add,
    Subtract
};

struct Modifier {
    Modifier(Target target, Behaviour behaviour)
        : target(target), behaviour(behaviour) {}

    virtual void modDimension(Dimension &dim, const size_t elementWidth) = 0;

    virtual bool isDynamic() const {
        return false;
    }

protected:
    const Target target;
    const Behaviour behaviour;

    streamingUnit_t *su;

    // void modStatic(Dimension& dim) const;

    // void modIndirect(Dimension& dim) const;
};

struct StaticModifier : public Modifier {
    StaticModifier(Target t, Behaviour b, size_t d = 0, size_t s = 0)
        : Modifier(t, b), displacement(d), size(s) {
        assert_msg("Static modifier must be of type Increment or Decrement", b == Behaviour::Decrement || b == Behaviour::Increment);
    }

	void modDimension(Dimension &dim, const size_t elementWidth) override;

private:
    const size_t displacement;
    const size_t size;
};

struct DynamicModifier : public Modifier {
    DynamicModifier(Target t, Behaviour b, const size_t src, streamingUnit_t *su)
        : Modifier(t, b), streamSource(src), su(su) {}

	void modDimension(Dimension &dim, const size_t elementWidth) override;

    bool isDynamic() const override{
        return true;
    }

private:
    const size_t streamSource;

    void calculateValueChange(size_t &target, size_t baseValue, Behaviour behaviour, size_t valueChange);

    streamingUnit_t *su;
};

#endif // DIMENSION_HPP
