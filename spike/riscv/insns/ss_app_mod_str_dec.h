auto &destReg = P.SU.registers[insn.uve_rd()];
auto sizeReg = insn.uve_mod_size();
auto dispReg = insn.uve_mod_disp();

//unsigned int size = READ_REG(sizeReg);
int32_t disp = READ_REG(dispReg);

std::visit([&](auto &reg) {
    reg.addStaticModifier(staticModifier_t(Target::Stride, staticBehaviour::Decrement, disp/*, size*/));
}, destReg);