auto regN = insn.uve_branch_rs();
//auto& streamReg = P.SU.registers[regN];
auto branchIMM = insn.uve_branch_imm();

/*
const bool notComplete = std::visit([](const auto &reg){
    return !reg.isEndOfDimensionOfDim(0);
}, streamReg);
*/

/*auto streamReg = insn.uve_branch_rs();
auto branchIMM = insn.uve_branch_imm();

const bool notComplete = operateRegister(P.SU, streamReg, [=](auto &reg) {
    return !reg.isEndOfDimensionOfDim(0);
});*/

//if (notComplete)

if (!P.SU.EODTable.at(regN).at(0))
    set_pc(pc + branchIMM);