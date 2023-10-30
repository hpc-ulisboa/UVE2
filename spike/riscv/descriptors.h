#ifndef DIMENSION_HPP
#define DIMENSION_HPP

#include <cstddef> // size_t
#include "helpers.h"

struct Dimension
{
  Dimension(uint64_t offset, unsigned int size, int stride);

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
  const uint64_t offset;
  const unsigned int size;
  const int stride;
  uint64_t iter_offset;
  unsigned int iter_size;
  int iter_stride;
  size_t iter_index;
  bool endOfDimension;

  friend class Modifier;
};

struct Modifier
{
  enum class Type
  {
    Static,
    Indirect
  };
  enum class Target
  {
    None, // necessary?
    Offset,
    Size,
    Stride
  };
  enum class Behaviour
  {
    None, // necessary?
    Increment,
    Decrement
    //add "set-value" behaviour
  };

  Modifier(Type type, Target target = Target::None, Behaviour behaviour = Behaviour::None, size_t displacement = 0, unsigned int size = 0)
    : type(type), target(target), behaviour(behaviour), displacement(displacement), size(size)
  {}

  void modDimension(Dimension& dim) const;

  void printModifier() const;

  Type getType() const;

private:
  const Type type;
  const Target target;
  const Behaviour behaviour;
  const size_t displacement;
  const unsigned int size;

  void modStatic(Dimension& dim) const;

  void modIndirect(Dimension& dim) const;
};

#endif // DIMENSION_HPP
