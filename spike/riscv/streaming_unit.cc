#include "streaming_unit.h"
#include "mmu.h"
#include "processor.h"

#define gMMU(p) (*(p->get_mmu()))

template <typename T>
void streamRegister_t<T>::addModifier(std::shared_ptr<Modifier> mod) {
    /* A recently added modifier alters the dimension inserted before the last one */
    // const auto modIndex = dimensions.size() - 2;
    assert_msg("Cannot append more modifiers as max dimensions were reached", dimensions.size() + 1 < su->maxDimensions);
    const auto modIndex = -1; // next dimension to be added (will increment as dimensions are added)
    auto iter = modifiers.find(modIndex);
    // assert_msg("Trying to add a modifier, when one already exists in this index", iter == modifiers.end());
    modifiers.insert({modIndex, mod});
    /* print modifiers
    std::cout << "\nModifiers: ";
    for (auto &m : modifiers)
        std::cout << m.first << "  ";
    std::cout << std::endl;*/
}

template <typename T>
void streamRegister_t<T>::addDimension(Dimension dim) {
    assert_msg("Cannot append more dimensions as the max value was reached", dimensions.size() < su->maxDimensions);

    dimensions.push_front(dim);

    vecCfg.push_front(false);

    std::unordered_multimap<int, std::shared_ptr<Modifier>> updatedModifiers;

    // Increment all modifiers' indexes
    for (auto &m : modifiers) {
        updatedModifiers.insert(std::make_pair(m.first + 1, m.second));
    }

    modifiers.swap(updatedModifiers);

    /* print modifiers
    std::cout << "Modifiers u" << registerN << ": ";
    for (auto &m : modifiers)
        std::cout << m.first << "  ";
    std::cout << std::endl;*/

    // print dimensions size
    // std::cout << "Dimensions size: " << dimensions.size() << std::endl;
}

template <typename T>
void streamRegister_t<T>::configureDim() {
    mode = RegisterMode::Vector;
    validIndex = vLen;
    // const auto cfgIndex = dimensions.size() - 1;
    const auto cfgIndex = 0;
    vecCfg.at(cfgIndex) = true;
}

template <typename T>
void streamRegister_t<T>::startConfiguration(Dimension dim) {
    status = RegisterStatus::NotConfigured;
    mode = RegisterMode::Scalar;
    validIndex = 1;
    dimensions.clear();
    dimensions.push_back(dim);
    vecCfg.push_back(false);
}

template <typename T>
void streamRegister_t<T>::endConfiguration() {
    status = RegisterStatus::Running;
}

template <typename T>
std::vector<T> streamRegister_t<T>::getElements(bool causesUpdate) {
    if (causesUpdate && this->type == RegisterConfig::Load)
        updateAsLoad();

    std::vector<T> e(elements.begin(), elements.end());

    return e;
}

template <typename T>
bool streamRegister_t<T>::getDynModElement(int &value) {
    mode = RegisterMode::Scalar;
    validIndex = 1;
    assert_msg("Dynamic modifier source is not correctly configured", this->type == RegisterConfig::Load);
    
    updateAsLoad();

    value = readAS<int>(elements.at(0));

    // check if any EOD flag is set in EODTable
    for (auto &eod : su->EODTable.at(registerN)) {
        if (eod)
            return 0;
    }

    return 1;
}

template <typename T>
void streamRegister_t<T>::setElements(std::vector<T> e, bool causesUpdate) {
    assert_msg("Trying to set values to a load stream", type != RegisterConfig::Load);

    elements = e;

    if (registerN == 10 || registerN == 17)
        std::cout << "u" << registerN << "    element: " << readAS<double>(elements.at(0)) << std::endl;
        

    if (causesUpdate && this->type == RegisterConfig::Store)
        updateAsStore();
}

template <typename T>
void streamRegister_t<T>::setValidIndex(const size_t i) {
    assert_msg("Trying to set valid index to invalid value", i <= vLen);

    validIndex = i;
}

template <typename T>
void streamRegister_t<T>::setMode(const RegisterMode m) {
    mode = m;
    if (m == RegisterMode::Scalar)
        validIndex = 1;
    else
        validIndex = vLen;
}

template <typename T>
bool streamRegister_t<T>::hasStreamFinished() const {
    return status == RegisterStatus::Finished;
}

template <typename T>
bool streamRegister_t<T>::isEndOfDimensionOfDim(size_t i) const {
    assert_msg("Trying to check EOD of invalid dimension", i < dimensions.size());
    return dimensions.at(i).isEndOfDimension();
}

template <typename T>
size_t streamRegister_t<T>::getElementWidth() const {
    return elementWidth;
}

template <typename T>
size_t streamRegister_t<T>::getVLen() const {
    return vLen;
}

template <typename T>
size_t streamRegister_t<T>::getValidIndex() const {
    return validIndex;
}

template <typename T>
RegisterStatus streamRegister_t<T>::getStatus() const {
    return status;
}

template <typename T>
RegisterConfig streamRegister_t<T>::getType() const {
    return type;
}

template <typename T>
RegisterMode streamRegister_t<T>::getMode() const {
    return mode;
}

/* FOR DEBUGGING*/
template <typename T>
void streamRegister_t<T>::printRegN(char *str) {
    if (registerN >= 0) {
        fprintf(stderr, ">>> UVE Register u%ld %s <<<\n", registerN, str);
    } else
        fprintf(stderr, ">>> Register number not set for debugging. %s<<<", str);
}

template <typename T>
size_t streamRegister_t<T>::generateOffset() {
    /* Result will be the final accumulation of all offsets calculated per dimension */
    size_t init = 0;
    int dimN = 0;

    return std::accumulate(dimensions.begin(), dimensions.end(), init, [&](size_t acc, Dimension &dim) {
        if (dim.isLastIteration() && isDimensionFullyDone(dimensions.begin(), dimensions.begin() + dimN)) {
            dim.setEndOfDimension(true);
        }
        ++dimN;
        // std::cout << "Accumulating dimension " << ++dimN << std::endl;
        return acc + dim.calcOffset(elementWidth);
    });
}

template <typename T>
bool streamRegister_t<T>::isDimensionFullyDone(const std::deque<Dimension>::const_iterator start, const std::deque<Dimension>::const_iterator end) const {
    return std::accumulate(start, end, true, [](bool acc, const Dimension &dim) {
        return acc && dim.isEndOfDimension();
    });
}

template <typename T>
bool streamRegister_t<T>::isStreamDone() const {
    return isDimensionFullyDone(dimensions.begin(), dimensions.end());
}

template <typename T>
bool streamRegister_t<T>::tryGenerateOffset(size_t &address) {
    /* There are two situations that prevent us from generating offsets/iterating a stream:
    1) We are at the last iteration of the outermost dimension
    2) We just finished the last iteration of a dimension and there is a configure
    stream vector modifier at that same dimension. In these cases, the generation can
    only resume after an exterior call to setEndOfDimension(false) */

    /* The outermost dimension is the last one in the container */

    if (isStreamDone()) {
        /*if(registerN == 2)
		    std:: cout << "u" << registerN << "    Stream is done" << std::endl;*/
        status = RegisterStatus::Finished;
        type = RegisterConfig::NoStream;
        return false;
    }

    for (size_t i = 0; i < dimensions.size() - 1; i++) {
        applyDynamicMods(i);
        if (vecCfg.at(i) && isDimensionFullyDone(dimensions.begin(), dimensions.begin() + i + 1)) {
            /*if(registerN == 2)
                std::cout << "u" << registerN << "    Stop dimension loading " << i << std::endl;*/
            return false;
        }
    }
    address = generateOffset();
    return true;
}

template <typename T>
void streamRegister_t<T>::applyDynamicMods(size_t dimN) {
    auto currentModifierIters = modifiers.equal_range(dimN);
    for (auto it = currentModifierIters.first; it != currentModifierIters.second; ++it) {
        if (it->second->isDynamic() && !it->second->isApplied()){
            //std::cout << "u" << registerN << "    Applying dynamic modifier to dim " << dimN << std::endl;
            it->second->modDimension(dimensions.at(dimN), elementWidth);
        }
    }
}

template <typename T>
void streamRegister_t<T>::setDynamicModsNotApplied(size_t dimN, bool ifScatter) {
    auto currentModifierIters = modifiers.equal_range(dimN);
    for (auto it = currentModifierIters.first; it != currentModifierIters.second; ++it) {
        if (it->second->isDynamic() && it->second->isScatter() == ifScatter)
            it->second->setApplied(false);
    }
}

template <typename T>
void streamRegister_t<T>::updateIteration() {
    if (isStreamDone()) {
        status = RegisterStatus::Finished;
        type = RegisterConfig::NoStream;
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
        auto &nextDim = dimensions.at(i + 1);
        /* The following calculations are only necessary if we ARE in the
        last iteration of a dimension */

        if (!currDim.isEndOfDimension()){
            //std::cout << "Dimension " << i << " is not EOD" << std::endl;
            continue;
        }

        // std::cout << "Looking for modifiers of dimension " << i << std::endl;

        // Reset EOD flag of current dimension
        currDim.setEndOfDimension(false);
        // Unflag regular dynamic modifiers of current dimension
        setDynamicModsNotApplied(i, false);
        
        // Iterate upper dimension
        nextDim.advance();
        // Unflag scatter dynamic modifiers of upper dimension
        setDynamicModsNotApplied(i+1, true);

        // Apply static modifiers to current dimension
        auto currentModifierIters = modifiers.equal_range(i);
        for (auto it = currentModifierIters.first; it != currentModifierIters.second; ++it) {
            if (!it->second->isDynamic()) {
                //std::cout << "u" << registerN << "    Applying static modifier to dim " << i << std::endl;
                it->second->modDimension(currDim, elementWidth);
            }
        }

        // The values at lower dimensions might have been modified. As such, we need to reset them before next iteration
        for (size_t j = 0; j < i; j++)
            dimensions.at(j).resetIterValues();
    }
}

template <typename T>
void streamRegister_t<T>::updateAsLoad() {
    assert_msg("Trying to update as load a non-load stream", type == RegisterConfig::Load);
    if (isStreamDone()) { // doesn't try to load if stream has finished
        status = RegisterStatus::Finished;
        type = RegisterConfig::NoStream;
        return;
    }

    // elements.clear();
    // elements.reserve(vLen);

    size_t eCount = 0;
    validIndex = 0; // reset valid index

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

    size_t max = mode == RegisterMode::Vector ? vLen : 1;

    bool causesUpdate = true;

    while (eCount < max && tryGenerateOffset(offset)) {
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
        // elements.push_back(value);
        // std::cout << "u"<< registerN << "   Loaded Value: " << readAS<double>(value) << std::endl;
        elements.at(eCount) = value;
        /*if (registerN==2)
            std::cout << "u" << registerN << "    Loaded Value: " << readAS<int>(value) << std::endl;*/
        ++validIndex;
        for (size_t i = 0; i < dimensions.size() - 1; i++)
            setDynamicModsNotApplied(i, true);
        if (tryGenerateOffset(offset)) {
            updateIteration(); // reset EOD flags and iterate stream
            ++eCount;
        } else{
			break;
		}

        causesUpdate = false;
    }
    su->updateEODTable(registerN); // save current state of the stream so that branches can catch EOD flags
    // std::cout << "eCount: " << eCount << std::endl;
    // std::cout << "vLen: " << vLen << std::endl;
    if (eCount < max) {      // iteration is already updated when register is full
        updateIteration(); // reset EOD flags and iterate stream
        /*for (size_t i = 0; i < dimensions.size() - 1; i++)
            setDynamicModsNotApplied(i, true);*/
    }
}

template <typename T>
void streamRegister_t<T>::updateAsStore() {
    assert_msg("Trying to update as store a non-store stream", type == RegisterConfig::Store);

    // std::cout << "Updating as store" << std::endl;
    if (isStreamDone()) {
        status = RegisterStatus::Finished;
        type = RegisterConfig::NoStream;
        return;
    }

    // std::cout << "Storing " << elements.size() << " elements. eCount=" << vLen << std::endl;
    size_t offset;
    size_t eCount = 0;

    /*
    std::cout << "Storing " << validIndex << " elements." << std::endl;
    // print vecCfg
    printRegN("\nvecCfg: ");
    for (auto &v : vecCfg)
        std::cout << v << " ";
    std::cout << std::endl;
    */

    while (eCount < validIndex && tryGenerateOffset(offset)) {
        // auto value = elements.front();
        // elements.erase(elements.begin());
        // elements.pop_front(); //-- std::array
        auto value = elements.at(eCount);
        // std::cout << "\nStored Values: " << readAS<double>(value) << " ";
        if constexpr (std::is_same_v<ElementsType, std::uint8_t>)
            gMMU(su->p).template store<std::uint8_t>(offset, readAS<ElementsType>(value));
        else if constexpr (std::is_same_v<ElementsType, std::uint16_t>)
            gMMU(su->p).template store<std::uint16_t>(offset, readAS<ElementsType>(value));
        else if constexpr (std::is_same_v<ElementsType, std::uint32_t>)
            gMMU(su->p).template store<std::uint32_t>(offset, readAS<ElementsType>(value));
        else
            gMMU(su->p).template store<std::uint64_t>(offset, readAS<ElementsType>(value));

        if (tryGenerateOffset(offset)) {
            updateIteration(); // reset EOD flags and iterate stream
            ++eCount;
        } else
            break;
    }
    // std::cout << std::endl;
    su->updateEODTable(registerN); // save current state of the stream so that branches can catch EOD flags
    if (eCount < validIndex)       // iteration is already updated when register is full
        updateIteration();         // reset EOD flags and iterate stream
    // elements.clear();
}

std::vector<uint8_t> PredRegister::getPredicate() const {
    return elements;
}

void streamingUnit_t::updateEODTable(const size_t stream) {
    int r = 0, d = 0;
    std::visit([&](const auto reg) {
        int d = 0;
        for (const auto dim : reg.dimensions) {
            EODTable.at(stream).at(d) = /*reg.vecCfg.at(d) &&*/ dim.isEndOfDimension(); // flags are only necessary if dimensions are vector coupled
            // fprintf(stderr, "EOD of u%d: %d\n", stream, EODTable.at(stream).at(d));
            ++d;
        }
    }, registers.at(stream));
}

template <typename T>
void streamingUnit_t::makeStreamRegister(size_t streamRegister, RegisterConfig type) {
    assert_msg("Tried to use a register index higher than the available registers", streamRegister < registerCount);
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
    assert_msg("Tried to create predicate with invalid size", elements.size() == predicates.at(predRegister).vLen);
    for (auto &p : elements)
        assert_msg("Invalid values for predicate (must be 0 or 1)", !p || p == 1);
    predicates.at(predRegister).elements = elements;
}

template class streamRegister_t<uint8_t>;
template class streamRegister_t<uint16_t>;
template class streamRegister_t<uint32_t>;
template class streamRegister_t<uint64_t>;
template void streamingUnit_t::makeStreamRegister<uint8_t>(size_t streamRegister, RegisterConfig type);
template void streamingUnit_t::makeStreamRegister<uint16_t>(size_t streamRegister, RegisterConfig type);
template void streamingUnit_t::makeStreamRegister<uint32_t>(size_t streamRegister, RegisterConfig type);
template void streamingUnit_t::makeStreamRegister<uint64_t>(size_t streamRegister, RegisterConfig type);