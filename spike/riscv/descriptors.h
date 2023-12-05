#ifndef DIMENSION_HPP
#define DIMENSION_HPP

#include "helpers.h"
//#include "streaming_unit.h"

class streamingUnit_t;

struct Dimension {
    Dimension(size_t offset, size_t size, int stride)
        : offset(offset), size(size), stride(stride) {

        iter_offset = offset;
        iter_size = size;
        iter_stride = stride;
        iter_index = 0;
        endOfDimension = iter_size == 0;

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
    const int stride;
    size_t iter_offset;
    size_t iter_size;
    int iter_stride;
    size_t iter_index;
    bool endOfDimension;

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
        : target(target), behaviour(behaviour) {
            modApplied = false;
        }

    virtual void modDimension(Dimension &dim, const size_t elementWidth) = 0;

    virtual bool isDynamic() const {
        return false;
    }

    virtual bool isScatter() const {
        return false;
    }

    bool isApplied() const {
        return modApplied;
    }

    virtual void setApplied(const bool s) {
        modApplied = s;
    }

protected:
    const Target target;
    const Behaviour behaviour;

    bool modApplied;

    streamingUnit_t *su;

    // void modStatic(Dimension& dim) const;

    // void modIndirect(Dimension& dim) const;
};

struct StaticModifier : public Modifier {
    StaticModifier(Target t, Behaviour b, int d = 0, unsigned int s = 0)
        : Modifier(t, b), displacement(d), size(s) {
        assert_msg("Static modifier must be of type Increment or Decrement", b == Behaviour::Decrement || b == Behaviour::Increment);
    }

	void modDimension(Dimension &dim, const size_t elementWidth) override;

private:
    const int displacement;
    const unsigned int size;
};

struct DynamicModifier : public Modifier {
    DynamicModifier(Target t, Behaviour b, const size_t src, streamingUnit_t *su, bool sc = false /*default must be discussed*/)
        : Modifier(t, b), streamSource(src), scatter(sc), su(su) {
            indirectRegisterValue = 0;
            end = false;
        }

	void modDimension(Dimension &dim, const size_t elementWidth) override;

    bool isDynamic() const override{
        return true;
    }

    bool isScatter() const override{
        return scatter;
    }

private:
    const size_t streamSource;
    bool scatter;
    int indirectRegisterValue;
    bool end;

    streamingUnit_t *su;

    void calculateValueChange(auto &target, auto baseValue, Behaviour behaviour, int valueChange);
    void getIndirectRegisterValues();
};

#endif // DIMENSION_HPP
