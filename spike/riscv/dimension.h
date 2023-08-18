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

  Modifier(Type type, Target target = Target::None, Behaviour behaviour = Behaviour::None, size_t displacement = 0, size_t size = 0)
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
  const size_t size;

  void modStatic(Dimension& dim) const;

  void modIndirect(Dimension& dim) const;
};

#endif // DIMENSION_HPP
