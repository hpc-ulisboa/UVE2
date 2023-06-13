auto streamReg = insn.uve_branch_reg();
auto branchIMM = insn.uve_branch_imm();

const bool notComplete = operateRegister(P.SU, streamReg, [=](auto& reg) {
    return !reg.isEndOfDimensionOfDim(0);
});

if (notComplete){
    std::cout << "not complete" << std::endl;
    set_pc(pc + branchIMM);
}