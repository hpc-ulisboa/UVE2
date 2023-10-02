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
    [&](StreamReg64 &src) { double value = readRegAS(double, destReg); baseBehaviour(value, src, predReg, double{}); WRITE_REG(destReg, value); },
    [&](StreamReg32 &src) { float value = readRegAS(float, destReg); baseBehaviour(value, src, predReg, float{}); WRITE_REG(destReg, value);},
    [&](auto &src) { assert_msg("Invoking so.a.adds.acc.fp with invalid parameter sizes", false); }
}, srcReg);
