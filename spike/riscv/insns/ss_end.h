auto &destReg = P.SU.registers[insn.uve_rd()];
auto baseReg = insn.uve_conf_base();
auto sizeReg = insn.uve_conf_size();
auto strideReg = insn.uve_conf_stride();

uint64_t offset = READ_REG(baseReg);
unsigned int size = READ_REG(sizeReg);
int stride = READ_REG(strideReg);

std::visit([&](auto &reg) {
  reg.addDimension(Dimension(offset*reg.elementWidth, size, stride));
  reg.endConfiguration();
}, destReg);