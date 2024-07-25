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

    for (size_t i = 0; i < validElementsIndex; i++) {
        if (pi.at((i+1)*sizeof(OperationType)-1))
            acc += readAS<OperationType>(elements.at(i));
    }

    value = readAS<StorageType>(acc);
};

std::visit(overloaded{
    [&](StreamReg8 &src) { uint8_t value = 0; baseBehaviour(destReg, src, predReg, (unsigned char){}); WRITE_REG(destReg, value); },
    [&](StreamReg16 &src) { uint16_t value = 0; baseBehaviour(destReg, src, predReg, (unsigned short int){}); WRITE_REG(destReg, value); },
    [&](StreamReg32 &src) { uint32_t value = 0; baseBehaviour(destReg, src, predReg, (unsigned int){}); WRITE_REG(destReg, value); },
    [&](StreamReg64 &src) { uint64_t value = 0; baseBehaviour(destReg, src, predReg, (unsigned long int){}); WRITE_REG(destReg, value); },
    [&](auto &src) { assert_msg("Invoking so.a.adds.us with invalid parameter sizes", false); }
}, srcReg);