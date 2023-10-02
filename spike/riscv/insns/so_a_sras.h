#define readRegAS(T, reg) static_cast<T>( READ_REG(reg) )

auto streamReg = insn.uve_rd();
auto &destReg = P.SU.registers[streamReg];
auto &src1Reg = P.SU.registers[insn.uve_rs1()];
auto src2 = insn.uve_rs2();
auto &predReg = P.SU.predicates[insn.uve_pred()];

const uint64_t shiftValue = readRegAS(uint64_t, src2);

/* The extra argument is passed because we need to tell the lambda the computation type. In C++20 we would
    use a lambda template parameter, however in C++17 we don't have those. As such, we pass an extra value to
    later on infer its type and know the storage we need to use */
auto baseBehaviour = [](auto &dest, auto &src, uint64_t shiftValue, auto &pred, auto extra) {
    /* We can only operate on the first available values of the stream */
    size_t vLen = src.getMode() == RegisterMode::Scalar ? 1 : dest.getVLen();
    auto values = src.getElements(true);
    auto destElements = dest.getElements(false);
    auto validElementsIndex = src.getValidIndex();

    auto pi = pred.getPredicate();

    /* Grab used types for storage and operation */
    using StorageType = typename std::remove_reference_t<decltype(dest)>::ElementsType;
    using OperationType = decltype(extra);
    std::vector<StorageType> out = destElements;

    OperationType high_bit = static_cast<OperationType>(-1);

    for (size_t i = 0; i < vLen; i++) {
        if (i < validElementsIndex){
            if (pi.at((i + 1) * sizeof(OperationType) - 1)) {
                auto value = readAS<OperationType>(values.at(i));
                out.at(i) = readAS<StorageType>((value >> shiftValue) | -((value & high_bit) >> shiftValue)); // https://stackoverflow.com/questions/76495063/how-can-i-reliably-perform-an-arithmetic-shift-right-in-c
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
            assert_msg("Trying to run so.a.sras with invalid src type", false);
    }
}, destReg);

std::visit(overloaded{
               [&](StreamReg8 &dest, StreamReg8 &src) { baseBehaviour(dest, src, shiftValue, predReg, (unsigned char){}); },
               [&](StreamReg16 &dest, StreamReg16 &src) { baseBehaviour(dest, src, shiftValue, predReg, (unsigned short int){}); },
               [&](StreamReg32 &dest, StreamReg32 &src) { baseBehaviour(dest, src, shiftValue, predReg, (unsigned int){}); },
               [&](StreamReg64 &dest, StreamReg64 &src) { baseBehaviour(dest, src, shiftValue, predReg, (unsigned long int){}); },
               [&](auto &dest, auto &src) { assert_msg("Invoking so.a.sras with invalid parameter sizes", false); }
}, destReg, src1Reg);