#ifndef DIMENSION_HPP
#define DIMENSION_HPP

#include "helpers.h"
//#include "streaming_unit.h"

class streamingUnit_t;

struct dimension_t {
    dimension_t(size_t offset, size_t size, int stride)
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
    size_t calcAddress(size_t width) const;
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

    //friend class modifier_t;
	friend class staticModifier_t;
	friend class dynamicModifier_t;
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

struct modifier_t {
    modifier_t(Target target, Behaviour behaviour)
        : target(target), behaviour(behaviour) {
            modApplied = false;
        }

    virtual void modDimension(dimension_t &dim, const size_t elementWidth) = 0;

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

    // void modStatic(dimension_t& dim) const;

    // void modIndirect(dimension_t& dim) const;
};

struct staticModifier_t : public modifier_t {
    staticModifier_t(Target t, Behaviour b, int d = 0, unsigned int s = 0)
        : modifier_t(t, b), displacement(d) {
        assert_msg("Static modifier must be of type Increment or Decrement", b == Behaviour::Decrement || b == Behaviour::Increment);
    }

	void modDimension(dimension_t &dim, const size_t elementWidth) override;

private:
    const int displacement;
};

struct dynamicModifier_t : public modifier_t {
    dynamicModifier_t(Target t, Behaviour b, const size_t src, streamingUnit_t *su, bool sc = false /*default must be discussed*/)
        : modifier_t(t, b), sourceStream(src), scatter(sc), su(su) {
            indirectRegisterValue = 0;
            sourceEnd = false;
        }

	void modDimension(dimension_t &dim, const size_t elementWidth) override;

    bool isDynamic() const override{
        return true;
    }

    bool isScatter() const override{
        return scatter;
    }

private:
    const size_t sourceStream;
    bool scatter;
    int indirectRegisterValue;
    bool sourceEnd;

    streamingUnit_t *su;

    void calculateValueChange(auto &target, auto baseValue, Behaviour behaviour, int valueChange);
    void getIndirectRegisterValues();
};

#endif // DIMENSION_HPP
