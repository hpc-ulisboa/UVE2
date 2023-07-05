#ifndef DIMENSION_HPP
#define DIMENSION_HPP

#include <cstddef> // std::size_t
#include "helpers.h"

struct Dimension
{
  Dimension(std::size_t offset, std::size_t size, std::size_t stride);

  void resetIndex();

  //void resetIterValues();

  bool isEmpty() const;

  void advance();

  bool isLastIteration() const;

  //bool triggerIterationUpdate() const;

  bool isEndOfDimension() const;

  void setEndOfDimension(bool b);

  std::size_t calcOffset(std::size_t width) const;

  std::size_t getSize() const;

private:
  const std::size_t offset;
  const std::size_t size;
  const std::size_t stride;
  std::size_t iter_offset;
  std::size_t iter_size;
  std::size_t iter_stride;
  std::size_t iter_index;
  bool endOfDimension;

  friend class Modifier;
};

struct Modifier
{
  enum class Type
  {
    Static,
    Indirect,
    //CfgVec // necessary?
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

  Modifier(Type type, Target target = Target::None, Behaviour behaviour = Behaviour::None, std::size_t displacement = 0, std::size_t size = 0)
    : type(type), target(target), behaviour(behaviour), displacement(displacement), size(size)
  {}

  void modDimension(Dimension& dim) const;

  void printModifier() const;

  Type getType() const;

private:
  const Type type;
  const Target target;
  const Behaviour behaviour;
  const std::size_t displacement;
  const std::size_t size;

  void modStatic(Dimension& dim) const;

  void modIndirect(Dimension& dim) const;
};

#endif // DIMENSION_HPP
