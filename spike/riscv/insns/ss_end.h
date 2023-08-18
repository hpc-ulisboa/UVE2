auto &destReg = P.SU.registers[insn.uve_rd()];
auto baseReg = insn.uve_conf_base();
auto sizeReg = insn.uve_conf_size();
auto strideReg = insn.uve_conf_stride();

reg_t base = READ_REG(baseReg);
size_t size = READ_REG(sizeReg);
size_t stride = READ_REG(strideReg);

std::visit([&](auto &reg) {
  reg.addDimension(Dimension(base, size, stride));
  reg.endConfiguration();
}, destReg);