auto dest = insn.uve_pred_rd();
auto& destPReg = P.SU.predicates[dest];
auto &predReg = P.SU.predicates[insn.uve_pred()];

auto pi = predReg.getPredicate();
auto destElements = destPReg.getPredicate();

std::vector<uint8_t> predicate(destPReg.vLen, 1);

for (int i = 0; i < destPReg.vLen; i++){
    if (!pi.at(i))
        predicate.at(i) = destElements.at(i); // merging
}

P.SU.makePredRegister(predicate, dest);
