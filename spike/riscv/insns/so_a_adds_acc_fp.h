auto destReg = insn.uve_rd();
auto &srcReg = P.SU.registers[insn.uve_rs1()];
auto &predReg = P.SU.predicates[insn.uve_pred()];

// The extra argument is passed because we need to tell the lambda the computation type. In C++20 we would use a lambda template parameter, however in C++17 we don't have those. As such, we pass an extra value to later on infer its type and know the storage we need to use
auto baseBehaviour = [](auto &value, auto &src, auto &pred, auto extra) {
    auto elements = src.getElements();
    auto pi = pred.getPredicate();
    auto validElementsIndex = src.getValidIndex();

    using OperationType = decltype(extra);
    using StorageType = typename std::remove_reference_t<decltype(value)>;

    OperationType acc = readAS<OperationType>(value);

    for (size_t i = 0; i < validElementsIndex; i++) {
        if (pi.at((i+1)*sizeof(OperationType)-1)){
            acc += readAS<OperationType>(elements.at(i));
        }
    }

    value = readAS<StorageType>(acc);
};

std::visit(overloaded{
    [&](StreamReg64 &src) { auto value = READ_FREG_D(destReg); baseBehaviour(value, src, predReg, double{}); WRITE_FREG(destReg, value); },
    [&](StreamReg32 &src) { auto value = READ_FREG_F(destReg); baseBehaviour(value, src, predReg, float{}); WRITE_FREG(destReg, value);},
    [&](auto &src) { assert_msg("Invoking so.a.adds.acc.fp with invalid parameter sizes", false); }
}, srcReg);
