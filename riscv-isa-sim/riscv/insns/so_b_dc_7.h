auto regN = insn.uve_branch_rs();
auto branchIMM = insn.uve_branch_imm();

if (P.SU.EODTable.at(regN).at(6))
    set_pc(pc + branchIMM);