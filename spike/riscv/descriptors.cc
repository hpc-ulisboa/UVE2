#include "descriptors.h"
#include <iostream>

/* Start of Dimension function definitions */
Dimension::Dimension(uint64_t offset, unsigned int size, int stride)
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

/*void Dimension::resetIndex() {
    iter_index = 0;
}*/

void Dimension::resetIterValues() {
    iter_offset = offset;
    iter_size = size;
    iter_stride = stride;
}

bool Dimension::isEmpty() const {
    return iter_size == 0;
}

void Dimension::advance() {
    ++iter_index;
}

bool Dimension::isLastIteration() const {
    // std::cout << "iter_index: " << iter_index << ", iter_size: " << iter_size
    // << std::endl;
    return iter_index + 1 >= iter_size;
}
/*bool Dimension::triggerIterationUpdate() const {
    return iter_index >= iter_size;
}*/

bool Dimension::isEndOfDimension() const {
    return endOfDimension;
}

void Dimension::setEndOfDimension(bool b) {
    // std::cout << "Setting end of dimension to: " << b << std::endl;
    endOfDimension = b;
    if (!b)
        iter_index = 0;
}

size_t Dimension::calcOffset(size_t width) const {
    // std::cout << "iter_offset: " << iter_offset << ", iter_stride: " <<
    // iter_stride << ", iter_index: " << iter_index << ", width: " << width <<
    // std::endl;
    return iter_offset + iter_stride * iter_index * width;
}

size_t Dimension::getSize() const {
    return iter_size;
}

/* Start of Modifier function definitions */

void Modifier::modDimension(Dimension &dim) const {
    switch (type) {
    case Type::Static:
        modStatic(dim);
        break;
    case Type::Indirect:
        modIndirect(dim);
        break;
    default:
        assert_msg("Unhandled Type case in modifiers's modDimension", false);
    }
}

void Modifier::modStatic(Dimension &dim) const {
    size_t valueChange = displacement;
    if (behaviour == Behaviour::Increment) {
        /* Nothing changes */
    } else if (behaviour == Behaviour::Decrement) {
        valueChange *= -1;
    } else {
        assert_msg("Unexpected behaviour type for a static modifier", false);
    }

    if (target == Target::Offset) {
        dim.iter_offset += valueChange;
        // std::cout << "iter_offset: " << dim.iter_offset << std::endl;
    } else if (target == Target::Size) {
        dim.iter_size += valueChange;
        // std::cout << "valueChange: " << valueChange << " iter_size: " <<
        // dim.iter_size << std::endl;
    } else if (target == Target::Stride) {
        dim.iter_stride += valueChange;
        // std::cout << "iter_stride: " << dim.iter_stride << std::endl;
    } else {
        assert_msg("Unexpected target for a static modifier", false);
    }
}

void Modifier::modIndirect(Dimension &dim) const {
    // TO DO
}

Modifier::Type Modifier::getType() const {
    return type;
}

void Modifier::printModifier() const {
    // print modifier
    std::cout << "Modifier: ";
    switch (type) {
    case Type::Static:
        std::cout << "Static";
        break;
    case Type::Indirect:
        std::cout << "Indirect";
        break;
    default:
        assert_msg("Unhandled Type case in modifiers's printModifier",
                   false);
    }
    std::cout << ", ";
    switch (target) {
    case Target::None:
        std::cout << "None";
        break;
    case Target::Offset:
        std::cout << "Offset";
        break;
    case Target::Size:
        std::cout << "Size";
        break;
    case Target::Stride:
        std::cout << "Stride";
        break;
    default:
        assert_msg("Unhandled Target case in modifiers's printModifier",
                   false);
    }
    std::cout << ", ";
    switch (behaviour) {
    case Behaviour::None:
        std::cout << "None";
        break;
    case Behaviour::Increment:
        std::cout << "Increment";
        break;
    case Behaviour::Decrement:
        std::cout << "Decrement";
        break;
    default:
        assert_msg("Unhandled Behaviour case in modifiers's printModifier",
                   false);
    }
    std::cout << ", displacement: " << displacement << ", size: " << size;
}