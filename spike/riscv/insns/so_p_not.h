auto dest = insn.uve_pred_rd();
auto& src = P.SU.predicates[insn.uve_pred_rs1()];
auto& destPReg = P.SU.predicates[dest];
auto &predReg = P.SU.predicates[insn.uve_pred()];

std::vector<uint8_t> predicate = src.getPredicate();

for (auto &p : predicate){
    p = !p;
}

P.SU.makePredRegister(predicate, dest);
