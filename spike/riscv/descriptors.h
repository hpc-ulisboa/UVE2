#ifndef DIMENSION_HPP
#define DIMENSION_HPP

#include <cstddef> // size_t
#include "helpers.h"

struct Dimension
{
  Dimension(size_t offset, size_t size, size_t stride);

  //void resetIndex();

  void resetIterValues();

  bool isEmpty() const;

  void advance();

  bool isLastIteration() const;

  //bool triggerIterationUpdate() const;

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

struct Modifier
{
  /*enum class Type
  {
    Static,
    Indirect
  };

  enum class Target
  {
    Offset,
    Size,
    Stride
  };

  enum class Behaviour
  {
    Increment,
    Decrement,
    SetValue,
    Add,
    Subtract
  };
  */
  Modifier(size_t d = 1)
    : displacement(d)
  {}

  virtual void modDimension(Dimension& dim) const;

  //void printModifier() const;
  //Type getType() const;

private:
  //const Type type;
  //const Target target;
  //const Behaviour behaviour;
  size_t displacement;
  //const size_t size;

  //void modStatic(Dimension& dim) const;

  //void modIndirect(Dimension& dim) const;
};

struct StaticModifier : public Modifier
{
  StaticModifier(Target t, Behaviour b, size_t d = 0, size_t s = 0)
    : Modifier(t, b), displacement(d), size(s) {
    assert_msg(b == Behaviour::Decrement ||  b == Behaviour::Increment, "Static modifier must be of type Increment or Decrement");
    }

private:
  const size_t displacement;
  const size_t size;
};

struct DynamicModifier : public Modifier
{
  DynamicModifier(Target target, Behaviour behaviour, size_t streamSource)
    : target(target), behaviour(behaviour), streamSource(streamSource)
  {}

private:
  const size_t streamSource;

};

#endif // DIMENSION_HPP
