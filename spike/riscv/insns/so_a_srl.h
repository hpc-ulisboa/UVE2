




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
    assert_msg("Given vectors have different widths", src1.getElementsWidth() == src2.getElementsWidth());
    size_t vLen = src1.getMode() == RegisterMode::Scalar ||  src2.getMode() == RegisterMode::Scalar ? 1 : dest.getVLen();
    /* We can only operate on the first available values of the stream */
    auto values = src1.getElements(true);
    auto shiftValues = src2.getElements(true);
    auto destElements = dest.getElements(false);
    auto validElementsIndex = std::min(src1.getValidIndex(), src2.getValidIndex());

    auto pi = pred.getPredicate();

    /* Grab used types for storage and operation */
    using StorageType = typename std::remove_reference_t<decltype(dest)>::ElementsType;
    using OperationType = decltype(extra);
    std::vector<StorageType> out = destElements;

    for (size_t i = 0; i < vLen; i++) {
        if (i < validElementsIndex){
            if (pi.at((i + 1) * sizeof(OperationType) - 1)) {
                auto value = readAS<OperationType>(values.at(i));
                auto shift = readAS<OperationType>(shiftValues.at(i));
                out.at(i) = readAS<StorageType>(value >> shift);
            }
        } else
            out.at(i) = 0; // zeroing out the rest of the elements
    }
    dest.setMode(vLen == 1 ? RegisterMode::Scalar : RegisterMode::Vector);
    dest.setElements(true, out);
    // std::cout << "\n\nOUT: " << out.size() << "\n\n";
    //dest.setValidIndex(dest.vLen);
};

/* If the destination register is not configured, we have to build it before the
operation so that its element size matches before any calculations are done */
std::visit([&](auto &dest) {
    if (dest.getStatus() == RegisterStatus::NotConfigured) {
        if (std::holds_alternative<StreamReg8>(src1Reg)) {
            P.SU.makeStreamRegister<std::uint8_t>(streamReg);
            dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg16>(src1Reg)) {
            P.SU.makeStreamRegister<std::uint16_t>(streamReg);
            dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg32>(src1Reg)) {
            P.SU.makeStreamRegister<std::uint32_t>(streamReg);
            dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg64>(src1Reg)) {
            P.SU.makeStreamRegister<std::uint64_t>(streamReg);
            dest.endConfiguration();
        } else  
            assert_msg("Trying to run so.a.srl with invalid src type", false);
    }
}, destReg);

std::visit(overloaded{
               [&](StreamReg8 &dest, StreamReg8 &src1, StreamReg8 &src2) { baseBehaviour(dest, src1, src2, predReg, (unsigned char){}); },
               [&](StreamReg16 &dest, StreamReg16 &src1, StreamReg16 &src2) { baseBehaviour(dest, src1, src2, predReg, (unsigned short int){}); },
               [&](StreamReg32 &dest, StreamReg32 &src1, StreamReg32 &src2) { baseBehaviour(dest, src1, src2, predReg, (unsigned int){}); },
               [&](StreamReg64 &dest, StreamReg64 &src1, StreamReg64 &src2) { baseBehaviour(dest, src1, src2, predReg, (unsigned long int){}); },
               [&](auto &dest, auto &src1, auto &src2) { assert_msg("Invoking so.a.srl with invalid parameter sizes", false); }
}, destReg, src1Reg, src2Reg);