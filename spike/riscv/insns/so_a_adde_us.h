auto streamReg = insn.uve_rd();
auto &destReg = P.SU.registers[streamReg];
auto &srcReg = P.SU.registers[insn.uve_rs1()];
auto &predReg = P.SU.predicates[insn.uve_pred()];

// The extra argument is passed because we need to tell the lambda the computation type. In C++20 we would use a lambda template parameter, however in C++17 we don't have those. As such, we pass an extra value to later on infer its type and know the storage we need to use
auto baseBehaviour = [](auto &dest, auto &src, auto &pred, auto extra) {
    auto elements = src.getElements(true);
    auto destElements = dest.getElements(false);
    auto pi = pred.getPredicate();
    auto validElementsIndex = src.getValidIndex();

    using StorageType = typename std::remove_reference_t<decltype(dest)>::ElementsType;
    using OperationType = decltype(extra);

    OperationType value = 0;

    std::vector<StorageType> out = destElements; // ??
 
    for (size_t i = 0; i < validElementsIndex; i++) {
        if (pi.at((i+1)*sizeof(OperationType)-1))
            value += readAS<OperationType>(elements.at(i));
    }
    
    out.at(0) = readAS<StorageType>(value);
    dest.setElements(true, out);
    dest.setValidIndex(1);
};

// If the destination register is not configured, we have to build it before the operation so that its element size matches before any calculations are done
std::visit([&](auto &dest) {
    if (dest.getStatus() == RegisterStatus::NotConfigured) {
        if (std::holds_alternative<StreamReg8>(srcReg)) {
            P.SU.makeStreamRegister<std::uint8_t>(RegisterConfig::NoStream, streamReg);
            dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg16>(srcReg)) {
            P.SU.makeStreamRegister<std::uint16_t>(RegisterConfig::NoStream, streamReg);
            dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg32>(srcReg)) {
            P.SU.makeStreamRegister<std::uint32_t>(RegisterConfig::NoStream, streamReg);
            dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg64>(srcReg)) {
            P.SU.makeStreamRegister<std::uint64_t>(RegisterConfig::NoStream, streamReg);
            dest.endConfiguration();
        } else
            assert_msg("Trying to run so.a.adde.us with invalid src type", false);
    }
}, destReg);

std::visit(overloaded{
    [&](StreamReg8 &dest, StreamReg8 &src) { baseBehaviour(dest, src, predReg, (unsigned char){}); },
    [&](StreamReg16 &dest, StreamReg16 &src) { baseBehaviour(dest, src, predReg, (unsigned short int){}); },
    [&](StreamReg32 &dest, StreamReg32 &src) { baseBehaviour(dest, src, predReg, (unsigned int){}); },
    [&](StreamReg64 &dest, StreamReg64 &src) { baseBehaviour(dest, src, predReg, (unsigned long int){}); },
    [&](auto &dest, auto &src) { assert_msg("Invoking so.a.adde.us with invalid parameter sizes", false); }
}, destReg, srcReg);