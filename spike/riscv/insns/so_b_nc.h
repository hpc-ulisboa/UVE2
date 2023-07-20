auto& streamReg = P.SU.registers[insn.uve_branch_rs()];
auto branchIMM = insn.uve_branch_imm();

const bool notComplete = std::visit([](const auto &reg){
    return !reg.hasStreamFinished();
}, streamReg);

/*auto streamReg = insn.uve_branch_rs();
auto branchIMM = insn.uve_branch_imm();

const bool notComplete = operateRegister(P.SU, streamReg, [=](auto& reg) {
    return !reg.hasStreamFinished();
});
*/

if (notComplete)
    set_pc(pc + branchIMM);