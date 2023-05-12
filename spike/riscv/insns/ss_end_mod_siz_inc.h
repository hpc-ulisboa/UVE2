auto streamReg = insn.uve_comp_dest();
auto dispReg = insn.uve_comp_src1();
auto sizeReg = insn.uve_comp_src2();

int32_t displacement = READ_REG(dispReg);
int32_t size = READ_REG(sizeReg);

//std::cout << "END MOD SIZ INC" << std::endl;

operateRegister(P.SU, streamReg, [=](auto& reg) {
  reg.addModifier(Modifier(Modifier::Type::Static, Modifier::Target::Size, Modifier::Behaviour::Increment, displacement, size));
  reg.endConfiguration();
});

//std::cout << "END END MOD SIZ INC" << std::endl;