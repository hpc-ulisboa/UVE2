#define readRegAS(T, reg) static_cast<T>( READ_REG(reg) )

auto destReg = insn.uve_rd();
auto &srcReg = P.SU.registers[insn.uve_rs1()];
auto &predReg = P.SU.predicates[insn.uve_pred()];

// The extra argument is passed because we need to tell the lambda the computation type. In C++20 we would use a lambda template parameter, however in C++17 we don't have those. As such, we pass an extra value to later on infer its type and know the storage we need to use
auto baseBehaviour = [](auto &value, auto &src, auto &pred, auto extra) {
    auto elements = src.getElements();
    auto pi = pred.getPredicate();
    auto validElementsIndex = src.getValidIndex();

    using OperationType = decltype(extra);

    for (size_t i = 0; i < validElementsIndex; i++) {
        if (pi.at((i+1)*sizeof(OperationType)-1))
            value += readAS<OperationType>(elements.at(i));
    }
};

std::visit(overloaded{
    [&](StreamReg8 &src) { unsigned char value = readRegAS(unsigned char, destReg); baseBehaviour(destReg, src, predReg, (unsigned char){}); WRITE_REG(destReg, value); },
    [&](StreamReg16 &src) { unsigned short int value = readRegAS(unsigned short int, destReg); baseBehaviour(destReg, src, predReg, (unsigned short int){}); WRITE_REG(destReg, value); },
    [&](StreamReg32 &src) { unsigned int value = readRegAS(unsigned int, destReg); baseBehaviour(destReg, src, predReg, (unsigned int){}); WRITE_REG(destReg, value); },
    [&](StreamReg64 &src) { unsigned long int value = readRegAS(unsigned long int, destReg); baseBehaviour(destReg, src, predReg, (unsigned long int){}); WRITE_REG(destReg, value); },
    [&](auto &src) { assert_msg("Invoking so.a.adds.acc.us with invalid parameter sizes", false); }
}, srcReg);