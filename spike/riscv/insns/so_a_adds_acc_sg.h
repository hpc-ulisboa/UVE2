#define readRegAS(T, reg) static_cast<T>( READ_REG(reg) )

auto destReg = insn.uve_rd();
auto &srcReg = P.SU.registers[insn.uve_rs1()];
auto &predReg = P.SU.predicates[insn.uve_pred()];

// The extra argument is passed because we need to tell the lambda the computation type. In C++20 we would use a lambda template parameter, however in C++17 we don't have those. As such, we pass an extra value to later on infer its type and know the storage we need to use
auto baseBehaviour = [](auto &value, auto &src, auto &pred, auto extra) {
    auto elements = src.getElements(true);
    auto pi = pred.getPredicate();
    auto validElementsIndex = src.getValidIndex();

    using OperationType = decltype(extra);

    for (size_t i = 0; i < validElementsIndex; i++) {
        if (pi.at((i+1)*sizeof(OperationType)-1))
            value += readAS<OperationType>(elements.at(i));
    }
};

std::visit(overloaded{
    [&](StreamReg8 &src) { signed char value = value = readRegAS(signed char, destReg); baseBehaviour(destReg, src, predReg, (signed char){}); WRITE_REG(destReg, value); },
    [&](StreamReg16 &src) { short int value = value = readRegAS(short int, destReg); baseBehaviour(destReg, src, predReg, (short int){}); WRITE_REG(destReg, value); },
    [&](StreamReg32 &src) { int value = value = readRegAS(int, destReg); baseBehaviour(destReg, src, predReg, int{}); WRITE_REG(destReg, value); },
    [&](StreamReg64 &src) { long int value = value = readRegAS(long int, destReg); baseBehaviour(destReg, src, predReg, (long int){}); WRITE_REG(destReg, value); },
    [&](auto &src) { assert_msg("Invoking so.a.adds.acc.sg with invalid parameter sizes", false); }
}, srcReg);