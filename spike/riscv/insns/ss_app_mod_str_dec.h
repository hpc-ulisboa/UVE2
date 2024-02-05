auto &destReg = P.SU.registers[insn.uve_rd()];
auto sizeReg = insn.uve_mod_size();
auto dispReg = insn.uve_mod_disp();

//unsigned int size = READ_REG(sizeReg);
int32_t disp = READ_REG(dispReg);

std::visit([&](auto &reg) {
    reg.addModifier(std::make_shared<staticModifier_t>(Target::Stride, Behaviour::Decrement, disp/*, size*/));
}, destReg);