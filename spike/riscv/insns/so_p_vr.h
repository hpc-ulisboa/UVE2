auto dest = insn.uve_pred_rd();
auto& destPReg = P.SU.predicates[dest];
auto& src1Reg = P.SU.registers[insn.uve_pred_vs1()];
auto &predReg = P.SU.predicates[insn.uve_pred()];

auto baseBehaviour = [](auto &destP, auto &src1, auto &pred) {  
    auto StorageType = src1.getElementWidth();
    auto destElements = destP.getPredicate();
    auto validElementsIndex = src1.getValidIndex();

    auto pLen = destP.vLen/sizeof(StorageType);

    auto pi = pred.getPredicate();
    std::vector<uint8_t> predicate(pLen*sizeof(StorageType));

    auto iterator = predicate.begin();

    uint8_t value = 0;

    for (size_t i = 0; i < pLen; i++) {
        value = pi.at((i+1)*sizeof(StorageType)-1) ? (uint8_t)(i < validElementsIndex) : destElements.at(i); // // valid elements are 1 in the destination predicate : merging
        std::fill(iterator, iterator+sizeof(StorageType), value);
        iterator += sizeof(StorageType);
    }

    return predicate;
};


std::visit(overloaded{
    [&](StreamReg8 &src1) { auto pr = baseBehaviour(destPReg, src1, predReg); P.SU.makePredRegister(pr, dest); },
    [&](StreamReg16 &src1) { auto pr = baseBehaviour(destPReg, src1, predReg); P.SU.makePredRegister(pr, dest); },
    [&](StreamReg32 &src1) { auto pr = baseBehaviour(destPReg, src1, predReg); P.SU.makePredRegister(pr, dest); },
    [&](StreamReg64 &src1) { auto pr = baseBehaviour(destPReg, src1, predReg); P.SU.makePredRegister(pr, dest); },
    [&](auto &src1) { assert_msg("Invoking so.p.vr with invalid parameter sizes", false); }
}, src1Reg);