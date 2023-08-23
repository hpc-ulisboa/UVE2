auto streamReg = insn.uve_rd();
auto &destReg = P.SU.registers[streamReg];
auto &src1Reg = P.SU.registers[insn.uve_rs1()];
auto &src2Reg = P.SU.registers[insn.uve_rs2()];
auto &predReg = P.SU.predicates[insn.uve_pred()];

/* The extra argument is passed because we need to tell the lambda the computation type. In C++20 we would
    use a lambda template parameter, however in C++17 we don't have those. As such, we pass an extra value to
    later on infer its type and know the storage we need to use */
auto baseBehaviour = [](auto &dest, auto &src1, auto &src2, auto &pred, auto extra) {
    /* Each stream's elements must have the same width for content to be
     * operated on */
    assert_msg("Given streams have different widths", src1.getElementsWidth() == src2.getElementsWidth());
    /* We can only operate on the first available values of the stream */
    auto elements1 = src1.getElements(true);
    auto elements2 = src2.getElements(true);
    auto destElements = dest.getElements(false);
    auto validElementsIndex = std::min(src1.getValidIndex(), src2.getValidIndex());

    auto pi = pred.getPredicate();

    /* Grab used types for storage and operation */
    using StorageType = typename std::remove_reference_t<decltype(dest)>::ElementsType;
    using OperationType = decltype(extra);
    std::vector<StorageType> out = destElements;

    for (size_t i = 0; i < validElementsIndex; i++) {
        if (pi.at((i + 1) * sizeof(OperationType) - 1))
            out.at(i) = readAS<StorageType>(std::min(readAS<OperationType>(elements1.at(i)), readAS<OperationType>(elements2.at(i))));
    }
    dest.setElements(true, out);
    dest.setValidIndex(validElementsIndex);
};


/* If the destination register is not configured, we have to build it before the
operation so that its element size matches before any calculations are done */
std::visit([&](auto &dest) {
    if (dest.getStatus() == RegisterStatus::NotConfigured) {
        if (std::holds_alternative<StreamReg8>(src1Reg)) {
            P.SU.makeStreamRegister<std::uint8_t>(RegisterConfig::NoStream, streamReg);
            dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg16>(src1Reg)) {
            P.SU.makeStreamRegister<std::uint16_t>(RegisterConfig::NoStream, streamReg);
            dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg32>(src1Reg)) {
            P.SU.makeStreamRegister<std::uint32_t>(RegisterConfig::NoStream, streamReg);
            dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg64>(src1Reg)) {
            P.SU.makeStreamRegister<std::uint64_t>(RegisterConfig::NoStream, streamReg);
            dest.endConfiguration();
        } else  
            assert_msg("Trying to run so.a.min.us with invalid src type", false);
    }
}, destReg);

std::visit(overloaded{
               [&](StreamReg8 &dest, StreamReg8 &src1, StreamReg8 &src2) { baseBehaviour(dest, src1, src2, predReg, (unsigned char){}); },
               [&](StreamReg16 &dest, StreamReg16 &src1, StreamReg16 &src2) { baseBehaviour(dest, src1, src2, predReg, (unsigned short int){}); },
               [&](StreamReg32 &dest, StreamReg32 &src1, StreamReg32 &src2) { baseBehaviour(dest, src1, src2, predReg, (unsigned int){}); },
               [&](StreamReg64 &dest, StreamReg64 &src1, StreamReg64 &src2) { baseBehaviour(dest, src1, src2, predReg, (unsigned long int){}); },
               [&](auto &dest, auto &src1, auto &src2) { assert_msg("Invoking so.a.min.us with invalid parameter sizes", false); }
}, destReg, src1Reg, src2Reg);