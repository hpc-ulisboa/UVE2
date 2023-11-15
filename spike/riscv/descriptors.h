#ifndef DIMENSION_HPP
#define DIMENSION_HPP

#include "helpers.h"
#include <cstddef> // size_t

struct Dimension {
    Dimension(size_t offset, size_t size, size_t stride);

    // void resetIndex();

    void resetIterValues();

    bool isEmpty() const;

    void advance();

    bool isLastIteration() const;

    // bool triggerIterationUpdate() const;

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

    friend class Modifier;
};

enum class Target {
    Offset,
    Size,
    Stride
};

enum class Behaviour {
    Increment,
    Decrement,
    SetValue,
    Add,
    Subtract
};

struct Modifier {
    Modifier(Target target, Behaviour behaviour)
        : target(target), behaviour(behaviour) {}

    virtual void modDimension(Dimension &dim, const size_t elementWidth) = 0;

private:
    const Target target;
    const Behaviour behaviour;
    // void modStatic(Dimension& dim) const;

    // void modIndirect(Dimension& dim) const;
};

struct StaticModifier : public Modifier {
    StaticModifier(Target t, Behaviour b, size_t d = 0, size_t s = 0)
        : Modifier(t, b), displacement(d), size(s) {
        assert_msg(((Behaviour)b == Behaviour::Decrement || (Behaviour)b == Behaviour::Increment), "Static modifier must be of type Increment or Decrement");
    }

private:
    const size_t displacement;
    const size_t size;
};

struct DynamicModifier : public Modifier {
    DynamicModifier(Target t, Behaviour b, size_t src)
        : Modifier(t, b), streamSource(src) {}

private:
    const size_t streamSource;
};

#endif // DIMENSION_HPP
