auto destReg = insn.uve_rd();
auto &srcReg = P.SU.registers[insn.uve_rs1()];
auto &predReg = P.SU.predicates[insn.uve_pred()];

// The extra argument is passed because we need to tell the lambda the computation type. In C++20 we would use a lambda template parameter, however in C++17 we don't have those. As such, we pass an extra value to later on infer its type and know the storage we need to use
auto baseBehaviour = [](auto &value, auto &src, auto &pred, auto extra) {
    auto elements = src.getElements();
    auto pi = pred.getPredicate();
    auto validElementsIndex = src.getValidElements();

    using OperationType = decltype(extra);
    using StorageType = typename std::remove_reference_t<decltype(src)>::ElementsType;

    OperationType acc = 0;

    for (size_t i = 0; i < validElementsIndex; i++)
        if (pi.at((i+1)*sizeof(OperationType)-1))
            acc += readAS<OperationType>(elements.at(i));

    value = readAS<StorageType>(acc);
};

std::visit(overloaded{
    [&](StreamReg64 &src) { uint64_t value = 0; baseBehaviour(value, src, predReg, double{}); WRITE_REG(destReg, value); },
    [&](StreamReg32 &src) { uint32_t value = 0; baseBehaviour(value, src, predReg, float{}); WRITE_REG(destReg, value);},
    [&](auto &src) { assert_msg("Invoking so.a.adds.fp with invalid parameter sizes", false); }
}, srcReg);
