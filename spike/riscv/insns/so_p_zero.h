auto dest = insn.uve_pred_rd();
auto& destPReg = P.SU.predicates[dest];
auto &predReg = P.SU.predicates[insn.uve_pred()];

std::vector<uint8_t> predicate(destPReg.vLen, 0);
P.SU.makePredRegister(predicate, dest);
