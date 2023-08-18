auto dest = insn.uve_pred_rd();
auto &destPReg = P.SU.predicates[dest];
auto &srcPReg = P.SU.predicates[insn.uve_pred_rs1()];
auto &predReg = P.SU.predicates[insn.uve_pred()];

auto pi = predReg.getPredicate();
std::reverse(pi.begin(), pi.end()); // reverse the instruction predicate

auto predicate = srcPReg.getPredicate();
std::reverse(predicate.begin(), predicate.end()); // reverse the source predicate
size_t size = srcPReg.maxAmountElements;

auto destPredicate = destPReg.getPredicate();

for (size_t i = 0; i < size; ++i) {
    if (!pi.at(i))
        predicate.at(i) = destPredicate.at(i);
}

P.SU.makePredRegister(predicate, dest);
