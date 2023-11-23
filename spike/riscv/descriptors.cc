#include "descriptors.h"
#include "streaming_unit.h"
#include <iostream>

/* Start of Dimension function definitions */

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

bool Dimension::advance() {
    return ++iter_index == 1;
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

void StaticModifier::modDimension(Dimension &dim, const size_t elementWidth) {
    size_t valueChange = behaviour == Behaviour::Increment ? displacement : -1 * displacement;

    if (target == Target::Offset) {
        dim.iter_offset += valueChange * elementWidth;
        // std::cout << "iter_offset: " << dim.iter_offset << std::endl;
    } else if (target == Target::Size) {
        dim.iter_size += valueChange;
        // std::cout << "valueChange: " << (int)valueChange << " iter_size: " << dim.iter_size << std::endl;
    } else if (target == Target::Stride) {
        dim.iter_stride += valueChange;
        // std::cout << "iter_stride: " << dim.iter_stride << std::endl;
    } else {
        assert_msg("Unexpected target for a static modifier", false);
    }
}

void DynamicModifier::calculateValueChange(size_t &target, size_t baseValue, Behaviour behaviour, size_t valueChange) {
    switch (behaviour) {
    case Behaviour::Add:
        target = baseValue + valueChange;
        break;
    case Behaviour::Subtract:
        target = baseValue - valueChange;
        break;
    case Behaviour::Set:
        target = valueChange;
        break;
    case Behaviour::Increment:
        target += valueChange;
        break;
    case Behaviour::Decrement:
        target -= valueChange;
        break;
    default:
        assert_msg("Unexpected behaviour for a dynamic modifier", false);
    }
}

void DynamicModifier::modDimension(Dimension &dim, const size_t elementWidth) {
    // size_t valueChange = behaviour == Behaviour::Increment ? displacement : -1*displacement;
    auto &src = (su->registers).at(streamSource);
    size_t valueChange;
    std::visit([&](auto &reg) {
        valueChange = (size_t)(reg.getElements(true).at(0));
    }, src);

    if (target == Target::Offset) {
        calculateValueChange(dim.iter_offset, dim.offset, behaviour, valueChange * elementWidth);
        std::cout << "iter_offset: " << dim.iter_offset << std::endl;
    } else if (target == Target::Size) {
        calculateValueChange(dim.iter_size, dim.size, behaviour, valueChange);
        // std::cout << "iter_size: " << dim.iter_size << std::endl;
    } else if (target == Target::Stride) {
        calculateValueChange(dim.iter_stride, dim.stride, behaviour, valueChange);
        std::cout << "iter_stride: " << dim.iter_stride << std::endl;
    } else {
        assert_msg("Unexpected target for a dynamic modifier", false);
    }
}

/*void Modifier::printModifier() const {
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
}*/