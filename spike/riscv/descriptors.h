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
  };*/

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
    //SetValue,
    //Add,
    //Subtract
  };

  Modifier(Type type, Target target, Behaviour behaviour, size_t displacement = 0, size_t size = 0)
    : target(target), behaviour(behaviour)
  {}

  void modDimension(Dimension& dim) const;

  void printModifier() const;

  //Type getType() const;

private:
  //const Type type;
  const Target target;
  const Behaviour behaviour;
  //const size_t displacement;
  //const size_t size;

  //void modStatic(Dimension& dim) const;

  //void modIndirect(Dimension& dim) const;
};

struct StaticModifier : public Modifier
{
  StaticModifier(Type type, Target target, Behaviour behaviour, size_t displacement = 0, size_t size = 0)
    : type(type), target(target), behaviour(behaviour), displacement(displacement), size(size)
  {}

private:
  const size_t displacement;
  const size_t size;
};

struct StaticModifier : public Modifier
{
  StaticModifier(Type type, Target target, Behaviour behaviour, size_t displacement = 0, size_t size = 0)
    : type(type), target(target), behaviour(behaviour), displacement(displacement), size(size)
  {}

private:
  const size_t displacement;
  const size_t size;
};

#endif // DIMENSION_HPP
