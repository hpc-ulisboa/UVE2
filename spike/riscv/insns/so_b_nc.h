auto streamReg = insn.uve_branch_reg();
auto branchIMM = insn.uve_branch_imm();

// std::cout << "BRANCH" << std::endl;

const bool notComplete = operateRegister(P.SU, streamReg, [=](auto& reg) {
    return !reg.hasStreamFinished();
});

if (notComplete)
    set_pc(pc + branchIMM);

//std::cout << "BRANCH END" << std::endl;