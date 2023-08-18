auto dest = insn.uve_pred_rd();
auto& destPReg = P.SU.predicates[dest];
auto& src1Reg = P.SU.registers[insn.uve_pred_vs1()];
auto& src2Reg = P.SU.registers[insn.uve_pred_rs2()];
auto& predReg = P.SU.predicates[insn.uve_pred()];

auto baseBehaviour = [](auto &destP, auto &src1, auto &src2, auto &pred, auto extra) {
    /* Each stream's elements must have the same width for content to be
     * operated on */
    const bool src1Check = src1.getType() == RegisterConfig::Load || src1.getType() == RegisterConfig::Temporary;
    const bool src2Check = src2.getType() == RegisterConfig::Load || src2.getType() == RegisterConfig::Temporary;
    if (src1Check && src2Check)
        assert_msg("Given streams have different widths", src1.getElementsWidth() == src2.getElementsWidth());
    /* We can only operate on the first available values of the stream */
    auto elements1 = src1.getElements(true);
    auto elements2 = src2.getElements(true);
    auto destElements = destP.getPredicate();
    auto validElementsIndex = std::min(src1.getValidIndex(), src2.getValidIndex());

    auto p = pred.getPredicate();
    std::vector<uint8_t> predicate(pred.maxAmountElements);

    auto iterator = predicate.begin();

    if (validElementsIndex) {
        /* Grab used types for storage and operation */
        using OperationType = decltype(extra);
        uint8_t value = 0;
        for (size_t i = 0; i < validElementsIndex; i++) {
            if(p.at((i+1)*sizeof(OperationType)-1)){
                auto e1 = readAS<OperationType>(elements1.at(i));
                auto e2 = readAS<OperationType>(elements2.at(i));
                value = e1 == e2;
            } else
                value = destElements.at(i);
            std::fill(iterator, iterator+sizeof(OperationType), value);
            iterator += sizeof(OperationType);
        }
    }
    return predicate;
};

std::visit(overloaded{
    [&](StreamReg64 &src1, StreamReg64 &src2) { auto pr = baseBehaviour(destPReg, src1, src2, predReg, double{}); P.SU.makePredRegister(pr, dest);},
    [&](StreamReg32 &src1, StreamReg32 &src2) { auto pr = baseBehaviour(destPReg, src1, src2, predReg, float{}); P.SU.makePredRegister(pr, dest);},
    [&](auto &src1, auto &src2) { assert_msg("Invoking so.pr.eq.fp with invalid parameter sizes", false); }
}, src1Reg, src2Reg);