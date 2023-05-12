#include "Dimension.h"
#include <iostream>

/* Start of Dimension function definitions */
Dimension::Dimension(std::size_t offset, std::size_t size, std::size_t stride)
  : offset(offset), size(size), stride(stride)
{
  iter_offset = offset;
  iter_size = size;
  iter_stride = stride;
  iter_index = 0;
  endOfDimension = false;
}

void Dimension::resetIndex()
{
  iter_index = 0;
}

void Dimension::resetIterValues()
{
  iter_offset = offset;
  iter_size = size;
  iter_stride = stride;
}

void Dimension::advance()
{
  iter_index++;
}

bool Dimension::isLastIteration() const
{
  return iter_index + 1 == iter_size;
}
bool Dimension::triggerIterationUpdate() const
{
  return iter_index >= iter_size;
}

bool Dimension::isEndOfDimension() const
{
  return endOfDimension;
}

void Dimension::setEndOfDimension(bool b)
{
  endOfDimension = b;
}

std::size_t Dimension::calcOffset(std::size_t width) const
{
  return iter_offset + iter_stride * iter_index * width;
}

/* Start of Modifier function definitions */

void Modifier::modDimension(Dimension& dim) const
{
  switch (type) {
  case Type::Static:
    modStatic(dim);
    break;
  case Type::Indirect:
    modIndirect(dim);
    break;
  /*case Type::CfgVec:
    // Do nothing. This case gets handled in generateOffset
    break;
  */
  default:
    assert_msg("Unhandled Type case in modifiers's modDimension", false);
  }
}

void Modifier::modStatic(Dimension& dim) const
{
  std::size_t valueChange = displacement;
  if (behaviour == Behaviour::Increment) {
    /* Nothing changes */
  }
  else if (behaviour == Behaviour::Decrement) {
    valueChange *= -1;
  }
  else {
    assert_msg("Unexpected behaviour type for a static modifier", false);
  }

  if (target == Target::Offset) {
    dim.iter_offset += valueChange;
  }
  else if (target == Target::Size) {
    dim.iter_size += valueChange;
  }
  else if (target == Target::Stride) {
    dim.iter_stride += valueChange;
  }
  else {
    assert_msg("Unexpected target for a static modifier", false);
  }
}

void Modifier::modIndirect(Dimension& dim) const
{
  // TO DO
}

Modifier::Type Modifier::getType() const
{
  return type;
}
