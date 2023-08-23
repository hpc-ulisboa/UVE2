#include "streaming_unit.h"
#include "processor.h"
#include "mmu.h"

#define gMMU(p) (*(p->get_mmu()))


template <typename T>
void streamRegister_t<T>::addModifier(Modifier mod) {
    /* A recently added modifier alters the dimension inserted before the last one */
    const auto modIndex = dimensions.size() - 2;
    auto iter = modifiers.find(modIndex);
    assert_msg("Trying to add a modifier, when one already exists in this index", iter == modifiers.end());
    modifiers.insert({modIndex, mod});
}

template <typename T>
void streamRegister_t<T>::addDimension(Dimension dim) {
    assert_msg("Cannot append more dimensions as the max value was reached", dimensions.size() < su->maxDimensions);
    dimensions.push_back(dim);
    vecCfg.push_back(false);
    // print dimensions
    // std::cout << "Added dimension with offset " << dim.offset << ", size " << dim.size << " and stride " << dim.stride << std::endl;
    // print dimensions size
    // std::cout << "Dimensions size: " << dimensions.size() << std::endl;
}

template <typename T>
void streamRegister_t<T>::configureDim() {
    const auto cfgIndex = dimensions.size() - 1;
    vecCfg.at(cfgIndex) = true;
}

template <typename T>
void streamRegister_t<T>::startConfiguration(Dimension dim) {
    status = RegisterStatus::NotConfigured;
    dimensions.clear();
    dimensions.push_back(dim);
    vecCfg.push_back(false);
    getElements(false);
}

template <typename T>
void streamRegister_t<T>::endConfiguration() {
    status = RegisterStatus::Running;
    /*if (this->type == RegisterConfig::Load) {
      updateAsLoad();
    }*/
}

template <typename T>
std::vector<T> streamRegister_t<T>::getElements(bool causesUpdate) {
    //assert_msg("Trying to get values from a store stream", type != RegisterConfig::Store);

    if (causesUpdate)
        updateStreamValues();

    std::vector<T> e(elements.begin(), elements.end());

    return e;
}

template <typename T>
void streamRegister_t<T>::setElements(bool causesUpdate, std::vector<T> e) {
    assert_msg("Trying to set values to a load stream", type != RegisterConfig::Load);

    elements = e;

    if (causesUpdate)
        updateStreamValues();
}

template <typename T>
void streamRegister_t<T>::setValidIndex(const size_t i) {
    assert_msg("Trying to set valid index to invalid value", i <= maxAmountElements);

    validIndex = i;
}

template <typename T>
bool streamRegister_t<T>::hasStreamFinished() const {
    return status == RegisterStatus::Finished;
}

template <typename T>
void streamRegister_t<T>::clearEndOfDimensionOfDim(size_t i) {
    assert_msg("Trying to clear eof of invalid dimension", i < dimensions.size());
    /* Cannot clear this flag when the stream has finished, it would prevent us from knowing that */
    if (isStreamDone())
        return;
    dimensions.at(i).setEndOfDimension(false);
}

template <typename T>
bool streamRegister_t<T>::isEndOfDimensionOfDim(size_t i) const {
    assert_msg("Trying to check eof of invalid dimension", i < dimensions.size());
    return dimensions.at(i).isEndOfDimension();
}

template <typename T>
RegisterStatus streamRegister_t<T>::getStatus() const {
    return status;
}

template <typename T>
size_t streamRegister_t<T>::getElementsWidth() const {
    return elementsWidth;
}

template <typename T>
size_t streamRegister_t<T>::getMaxElements() const {
    return maxAmountElements;
}

template <typename T>
size_t streamRegister_t<T>::getValidIndex() const {
    return validIndex;
}

template <typename T>
RegisterConfig streamRegister_t<T>::getType() const {
    return type;
}

/* FOR DEBUGGING*/
template <typename T>
void streamRegister_t<T>::printRegN(char *str) {
    if (registerN >= 0)
        fprintf(stderr, ">>> UVE Register u%ld %s <<<\n", registerN, str);
    else
        fprintf(stderr, ">>> Register number not set for debugging. %s<<<", str);
}

template <typename T>
void streamRegister_t<T>::updateStreamValues() {
    // std::cout << "Updating stream values" << std::endl;
    if (this->type == RegisterConfig::Load) {
        updateAsLoad();
    } else if (this->type == RegisterConfig::Store) {
        updateAsStore();
    } else if (this->type == RegisterConfig::NoStream) {
        // do nothing
    } else {
        assert_msg("Unhandled type of stream was asked to update", false);
    }
}

template <typename T>
size_t streamRegister_t<T>::generateOffset() {
    /* Result will be the final accumulation of all offsets calculated per dimension */
    size_t init = 0;
    return std::accumulate(dimensions.begin(), dimensions.end(), init, [](size_t acc, Dimension &dim) {
        if (dim.isLastIteration()){
            //std::cout << "Last iteration of dimension." << std::endl;
            dim.setEndOfDimension(true);
        }
        return acc + dim.calcOffset(elementsWidth);
    });
}

template <typename T>
bool streamRegister_t<T>::isDimensionFullyDone(const std::vector<Dimension>::const_iterator start, const std::vector<Dimension>::const_iterator end) const {
    return std::accumulate(start, end, true, [](bool acc, const Dimension &dim) {
        return acc && dim.isEndOfDimension();
    });
}

template <typename T>
bool streamRegister_t<T>::isStreamDone() const {
    return isDimensionFullyDone(dimensions.begin(), dimensions.end());
}

template <typename T>
bool streamRegister_t<T>::tryGenerateOffset(size_t& address) {
    /* There are two situations that prevent us from generating offsets/iterating a stream:
    1) We are at the last iteration of the outermost dimension
    2) We just finished the last iteration of a dimension and there is a configure
    stream vector modifier at that same dimension. In these cases, the generation can
    only resume after an exterior call to setEndOfDimension(false) */

    /* The outermost dimension is the last one in the container */
    if (isStreamDone()){
        //std::cout << "Stream is done." << std::endl;
        return false;
    }
        
    for (size_t i = 0; i < dimensions.size() - 1; i++) {
        if (vecCfg.at(i) && isDimensionFullyDone(dimensions.begin(), dimensions.begin() + i + 1)){
            //std::cout << "This should never be printed in saxpy kernel." << std::endl;
            return false;
        }
    }
    address = generateOffset();
    return true;
}

template <typename T>
void streamRegister_t<T>::updateIteration() {
    if (isStreamDone()){
        status = RegisterStatus::Finished;
        return;
    }

    /* Iteration starts from the innermost dimension and updates the next if the current reaches an overflow */
    dimensions.at(0).advance();

    /* No extra processing is needed if there is only 1 dimension */
    if (dimensions.size() == 1)
        return;

    // std::cout << "Updating iteration. Dimensions: " << dimensions.size() << std::endl;
    for (size_t i = 0; i < dimensions.size() - 1; ++i) {
        auto &currDim = dimensions.at(i);
        /* The following calculations are only necessary if we ARE in the
        last iteration of a dimension */

        if (!currDim.isEndOfDimension())
            continue;

        //std::cout << "Looking for modifiers of dimension " << i << std::endl;
        auto currentModifierIter = modifiers.find(i);
        const bool modifierExists = currentModifierIter != modifiers.end();

        // currDim.resetIndex();
        //printRegN("Updating EOD of dimension.");

        dimensions.at(i + 1).advance();
        currDim.setEndOfDimension(false);
        if (modifierExists) {
            //printRegN("Applying modifier.");
            currentModifierIter->second.modDimension(currDim);
        }

        // The values at lower dimensions might have been modified. As such, we need to reset them before next iteration
        for (size_t j = 0; j < i; j++) {
          dimensions.at(j).resetIterValues();
        }
    }
}

template <typename T>
void streamRegister_t<T>::updateAsLoad() {
    if (isStreamDone()) { // doesn't try to load if stream has finished
        return;
    }

    //elements.clear();
    //elements.reserve(maxAmountElements);

    size_t eCount = 0;

    /*----------------------------------- Loading pipeline -----------------------------------
    ************************************* THIS IS OUTDATED ***********************************
     * Iterate stream
     * Try to load 1 element if not EOD of vector coupled dimension and stream is not finished
     *      Generate offset
     *      Push element to register
     *      Break if:
     *          EOD if vector coupled
     *          Register limit reached
     *      Iterate stream
     * End
     *
     * This iterates the stream before their first load, skipping 1 iteration.
     *----------------------------------------------------------------------------------------*/
    
    size_t offset;

    while (eCount < maxAmountElements && tryGenerateOffset(offset)){
        //std::cout << "Can generate offset before (eCount = " << eCount << ")" << std::endl;
        auto value = [this](auto address) -> ElementsType {
            if constexpr (std::is_same_v<ElementsType, std::uint8_t>)
                return readAS<ElementsType>(gMMU(su->p).template load<std::uint8_t>(address));
            else if constexpr (std::is_same_v<ElementsType, std::uint16_t>)
                return readAS<ElementsType>(gMMU(su->p).template load<std::uint16_t>(address));
            else if constexpr (std::is_same_v<ElementsType, std::uint32_t>)
                return readAS<ElementsType>(gMMU(su->p).template load<std::uint32_t>(address));
            else
                return readAS<ElementsType>(gMMU(su->p).template load<std::uint64_t>(address));
        }(offset);
        //elements.push_back(value);
        elements.at(eCount) = value;
        //std::cout << "Loaded Value: " << readAS<double>(value) << std::endl;
        if(tryGenerateOffset(offset)){
            //std::cout << "Can generate offset after (eCount = " << eCount << ")" << std::endl;
            updateIteration(); // reset EOD flags and iterate stream
            ++eCount;
        } else
            break;
    }
    su->updateEODTable(registerN); // save current state of the stream so that branches can catch EOD flags
    //std::cout << "eCount: " << eCount << std::endl;
    //std::cout << "maxAmountElements: " << maxAmountElements << std::endl;
    if(eCount < maxAmountElements){ // iteration is already updated when register is full
        updateIteration(); // reset EOD flags and iterate stream
        validIndex = ++eCount;
    } else 
        validIndex = eCount;
}
 
template <typename T>
void streamRegister_t<T>::updateAsStore() {
    // std::cout << "Updating as store" << std::endl;
    if (isStreamDone()) {
        return;
    }

    // std::cout << "Storing " << elements.size() << " elements. eCount=" << maxAmountElements << std::endl;
    size_t offset;
    size_t eCount = 0;
    while (eCount < validIndex && tryGenerateOffset(offset)) {
        //auto value = elements.front();
        //elements.erase(elements.begin());
        //elements.pop_front(); //-- std::array
        auto value = elements.at(eCount);
        //std::cout << "Stored Value: " << readAS<ElementsType>(value) << std::endl;
        if constexpr (std::is_same_v<ElementsType, std::uint8_t>)
            gMMU(su->p).template store<std::uint8_t>(offset, readAS<ElementsType>(value));
        else if constexpr (std::is_same_v<ElementsType, std::uint16_t>)
            gMMU(su->p).template store<std::uint16_t>(offset, readAS<ElementsType>(value));
        else if constexpr (std::is_same_v<ElementsType, std::uint32_t>)
            gMMU(su->p).template store<std::uint32_t>(offset, readAS<ElementsType>(value));
        else
            gMMU(su->p).template store<std::uint64_t>(offset, readAS<ElementsType>(value));
        if(tryGenerateOffset(offset)){
            updateIteration(); // reset EOD flags and iterate stream
            ++eCount;
        } else
            break;
    }
    su->updateEODTable(registerN); // save current state of the stream so that branches can catch EOD flags
    if(eCount < validIndex) // iteration is already updated when register is full
        updateIteration(); // reset EOD flags and iterate stream    
    //elements.clear();
}

std::vector<uint8_t> PredRegister::getPredicate() const{
    return elements;
}

void streamingUnit_t::updateEODTable(const size_t stream) {
    int r = 0, d = 0;
    std::visit([&](const auto reg){
        int d = 0;
        for (const auto dim : reg.dimensions) {
            EODTable.at(stream).at(d) = reg.vecCfg.at(d) && dim.isEndOfDimension(); // flags are only necessary if dimensions are vector coupled
            //fprintf(stderr, "EOD of u%d: %d\n", stream, EODTable.at(stream).at(d));
            ++d;
        }
    }, registers.at(stream));
}

template <typename T>
void streamingUnit_t::makeStreamRegister(RegisterConfig type, size_t streamRegister) {
    assert_msg("Tried to use a register index higher than the available register", streamRegister < registerCount);
    if constexpr (std::is_same_v<T, std::uint8_t>) {
        registers.at(streamRegister) = StreamReg8{this, type, streamRegister};
    } else if constexpr (std::is_same_v<T, std::uint16_t>) {
        registers.at(streamRegister) = StreamReg16{this, type, streamRegister};
    } else if constexpr (std::is_same_v<T, std::uint32_t>) {
        registers.at(streamRegister) = StreamReg32{this, type, streamRegister};
    } else if constexpr (std::is_same_v<T, std::uint64_t>) {
        registers.at(streamRegister) = StreamReg64{this, type, streamRegister};
    } else {
        static_assert(always_false_v<T>, "Cannot create register with this element width");
    }
}

void streamingUnit_t::makePredRegister(std::vector<uint8_t> elements, size_t predRegister) {
    assert_msg("Tried to alter p0 register, which is hardwired to 1", predRegister);
    assert_msg("Tried to use a predicate register index higher than the available predicate registers", predRegister < predRegCount);
    assert_msg("Tried to create predicate with invalid size", elements.size() == predicates.at(predRegister).maxAmountElements);
    for (auto &p : elements)
        assert_msg("Invalid values for predicate (must be 0 or 1)", !p || p == 1);
    predicates.at(predRegister).elements = elements;
}

template class streamRegister_t<uint8_t>;
template class streamRegister_t<uint16_t>;
template class streamRegister_t<uint32_t>;
template class streamRegister_t<uint64_t>;
template void streamingUnit_t::makeStreamRegister<uint8_t>(RegisterConfig type, size_t streamRegister);
template void streamingUnit_t::makeStreamRegister<uint16_t>(RegisterConfig type, size_t streamRegister);
template void streamingUnit_t::makeStreamRegister<uint32_t>(RegisterConfig type, size_t streamRegister);
template void streamingUnit_t::makeStreamRegister<uint64_t>(RegisterConfig type, size_t streamRegister);