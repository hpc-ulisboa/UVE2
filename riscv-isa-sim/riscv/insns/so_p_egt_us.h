auto dest = insn.uve_pred_rd();
auto& destPReg = P.SU.predicates[dest];
auto& src1Reg = P.SU.registers[insn.uve_pred_vs1()];
auto& src2Reg = P.SU.registers[insn.uve_pred_rs2()];
auto& predReg = P.SU.predicates[insn.uve_pred()];

auto baseBehaviour = [](auto &destP, auto &src1, auto &src2, auto &pred, auto extra) {
    /* Each stream's elements must have the same width for content to be
     * operated on */
    assert_msg("Given vectors have different widths", src1.getElementWidth() == src2.getElementWidth());
    /* We can only operate on the first available values of the stream */
    auto elements1 = src1.getElements();
    auto elements2 = src2.getElements();
    auto destElements = destP.getPredicate();
    auto validElementsIndex = std::min(src1.getValidElements(), src2.getValidElements());

    auto pi = pred.getPredicate();
    std::vector<uint8_t> predicate(pred.vLen);

    auto iterator = predicate.begin();

    if (validElementsIndex) {
        /* Grab used types for storage and operation */
        using OperationType = decltype(extra);
        uint8_t value = 0;
        for (size_t i = 0; i < validElementsIndex; i++) {
            if(pi.at((i+1)*sizeof(OperationType)-1)){
                OperationType e1 = readAS<OperationType>(elements1.at(i));
                OperationType e2 = readAS<OperationType>(elements2.at(i));
                value = e1 >= e2;
            } else
                value = readAS<OperationType>(destElements.at(i));
            std::fill(iterator, iterator+sizeof(OperationType), value);
            iterator += sizeof(OperationType);
        }
    }
    return predicate;
};

std::visit(overloaded{
    [&](StreamReg8 &src1, StreamReg8 &src2) { auto pr = baseBehaviour(destPReg, src1, src2, predReg, (unsigned char){}); P.SU.makePredRegister(pr, dest);},
    [&](StreamReg16 &src1, StreamReg16 &src2) { auto pr = baseBehaviour(destPReg, src1, src2, predReg, (unsigned short int){}); P.SU.makePredRegister(pr, dest);},
    [&](StreamReg32 &src1, StreamReg32 &src2) { auto pr = baseBehaviour(destPReg, src1, src2, predReg, (unsigned int){}); P.SU.makePredRegister(pr, dest);},
    [&](StreamReg64 &src1, StreamReg64 &src2) { auto pr = baseBehaviour(destPReg, src1, src2, predReg, (unsigned long int){}); P.SU.makePredRegister(pr, dest);},
    [&](auto &src1, auto &src2) { assert_msg("Invoking so.p.egt.us with invalid parameter sizes", false); }
}, src1Reg, src2Reg);