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
	friend class scatterGModifier_t;
};

enum class Target {
    Offset,
    Size,
    Stride
};

enum class staticBehaviour {
    Increment,
    Decrement
};

enum class dynamicBehaviour {
    Increment,
    Decrement,
    Set,
    Add,
    Subtract
};

struct staticModifier_t {
    staticModifier_t(Target t, staticBehaviour b, int d = 0, unsigned int td = 0)
        :target(t), behaviour(b), displacement(d), targetDim(td) {
    }

	void modDimension(std::deque<dimension_t> &dims, const size_t elementWidth);

    int getTargetDim() const {
        return targetDim;
    }

private:
    const Target target;
    const staticBehaviour behaviour;

    const int displacement;
    const int targetDim;
};

struct dynamicModifier_t {
    dynamicModifier_t(Target t, dynamicBehaviour b, const size_t src, streamingUnit_t *su, int td = 0)
        : target(t), behaviour(b), sourceStream(src), su(su), targetDim(td) {
            indirectRegisterValue = 0;
            sourceEnd = false;
            modApplied = false;
        }

	void modDimension(std::deque<dimension_t> &dims, const size_t elementWidth);

    bool isApplied() const {
        return modApplied;
    }

    void setApplied(const bool s) {
        modApplied = s;
    }

    int getTargetDim() const {
        return targetDim;
    }

private:
    const Target target;
    const dynamicBehaviour behaviour;

    bool modApplied;

    const size_t sourceStream;
    int indirectRegisterValue;
    bool sourceEnd;

    streamingUnit_t *su;

    const int targetDim;

    void calculateValueChange(auto &target, auto baseValue, dynamicBehaviour behaviour, int valueChange);
    void getIndirectRegisterValues();
};

struct scatterGModifier_t {
    scatterGModifier_t(dynamicBehaviour b, const size_t src, streamingUnit_t *su)
        : behaviour(b), sourceStream(src), su(su) {
            indirectRegisterValue = 0;
            sourceEnd = false;
            modApplied = false;
        }

	void modDimension(dimension_t &dim, const size_t elementWidth);

    bool isApplied() const {
        return modApplied;
    }

    void setApplied(const bool s) {
        modApplied = s;
    }

private:
    const Target target = Target::Offset; // Only offset is supported for now
    const dynamicBehaviour behaviour;

    bool modApplied;

    const size_t sourceStream;
    int indirectRegisterValue;
    bool sourceEnd;

    streamingUnit_t *su;

    void calculateValueChange(auto &target, auto baseValue, dynamicBehaviour behaviour, int valueChange);
    void getIndirectRegisterValues();
};

#endif // DIMENSION_HPP
