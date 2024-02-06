#include "descriptors.h"
#include "streaming_unit.h"
#include <iostream>
#include <limits.h>

/* Start of dimension_t function definitions */

void dimension_t::resetIterValues() {
    iter_offset = offset;
    iter_size = size;
    iter_stride = stride;
}

bool dimension_t::isEmpty() const {
    return iter_size == 0;
}

void dimension_t::advance() {
    ++iter_index;
}

bool dimension_t::isLastIteration() const {
    return iter_index + 1 >= iter_size;
}

bool dimension_t::isEndOfDimension() const {
    return endOfDimension;
}

void dimension_t::setEndOfDimension(bool b) {
    // std::cout << "Setting end of dimension to: " << b << std::endl;
    endOfDimension = b;
    if (!b)
        iter_index = 0;
}

size_t dimension_t::calcAddress(size_t width) const {
    /*std::cout << "iter_offset: " << iter_offset << ", iter_stride: " <<
    iter_stride << ", iter_index: " << iter_index << ", width: " << width << std::endl;
    std::cout << "iter_stride * iter_index * width: " << iter_stride * iter_index * width << std::endl;
    */
    return iter_offset + iter_stride * iter_index * width;
}

size_t dimension_t::getSize() const {
    return iter_size;
}

/* Start of modifier_t function definitions */

void staticModifier_t::modDimension(dimension_t &dim, const size_t elementWidth) {
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

    modApplied = true;
}

void dynamicModifier_t::calculateValueChange(auto &target, auto baseValue, Behaviour behaviour, int valueChange) {
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

void dynamicModifier_t::getIndirectRegisterValues() {
    auto &src = (su->registers).at(sourceStream);
    std::visit(overloaded{
        [&](auto &reg) { sourceEnd = !reg.getDynModElement(indirectRegisterValue);}
    }, src);

    /*print values
    for (auto &v : indirectRegisterValues) {
        printf("value: %d\n", v);
    }*/
}

void dynamicModifier_t::modDimension(dimension_t &dim, const size_t elementWidth) {
    // size_t valueChange = behaviour == Behaviour::Increment ? displacement : -1*displacement;
    if(!sourceEnd){
        getIndirectRegisterValues();

        if (target == Target::Offset) {
            calculateValueChange(dim.iter_offset, dim.offset, behaviour, indirectRegisterValue * elementWidth);
            dim.setEndOfDimension(false);

            if(scatter && behaviour != Behaviour::Increment && behaviour != Behaviour::Decrement)
                dim.iter_size = UINT_MAX;
            /* print dimension
            std::cout << "dimension_t: ";
            std::cout << "offset: " << dim.iter_offset << ", ";
            std::cout << "size: " << dim.iter_size << ", ";
            std::cout << "stride: " << dim.iter_stride << ", ";
            std::cout << "EOD: " << (int)dim.endOfDimension << std::endl;*/
            //std::cout << "iter_offset: " << dim.iter_offset << std::endl;
        } else if (target == Target::Size) {
            calculateValueChange(dim.iter_size, dim.size, behaviour, indirectRegisterValue);
            if (dim.iter_size)
                dim.setEndOfDimension(false);
            //std::cout << "iter_size: " << dim.iter_size << std::endl;
        } else if (target == Target::Stride) {
            calculateValueChange(dim.iter_stride, dim.stride, behaviour, indirectRegisterValue);
            //std::cout << "iter_stride: " << dim.iter_stride << std::endl;
        } else {
            assert_msg("Unexpected target for a dynamic modifier", false);
        }

        modApplied = true;

    } else {
        dim.setEndOfDimension(true);
        sourceEnd = false;
    }
  }

/*void modifier_t::printModifier() const {
    // print modifier
    std::cout << "modifier_t: ";
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