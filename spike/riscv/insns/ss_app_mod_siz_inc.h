auto &destReg = P.SU.registers[insn.uve_rd()];
auto sizeReg = insn.uve_mod_size();
auto dispReg = insn.uve_mod_disp();

size_t size = READ_REG(sizeReg);
int32_t disp = READ_REG(dispReg);

std::visit([&](auto &reg) {
    reg.addModifier(Modifier(Modifier::Type::Static, Modifier::Target::Size, Modifier::Behaviour::Increment, disp, size));
    reg.endConfiguration();
}, destReg);