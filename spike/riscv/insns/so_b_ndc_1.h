auto streamReg = insn.uve_branch_reg();
auto branchIMM = insn.uve_branch_imm();

std::cout << "BRANCH NC" << std::endl;

const bool notComplete = operateRegister(P.SU, streamReg, [=](auto& reg) {
    return !reg.isEndOfDimensionOfDim(0);
});

std::cout << "notComplete: " << notComplete << std::endl;

if (notComplete)
    set_pc(pc + branchIMM);

std::cout << "BRANCH NC END" << std::endl;
