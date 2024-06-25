auto streamReg = insn.uve_rd();
auto &destReg = P.SU.registers[streamReg];
auto baseReg = insn.uve_conf_base();
auto sizeReg = insn.uve_conf_size();
auto strideReg = insn.uve_conf_stride();

uint64_t base = READ_REG(baseReg);
unsigned int size = READ_REG(sizeReg);
int stride = READ_REG(strideReg);

P.SU.makeStreamRegister<std::uint32_t>(streamReg, RegisterConfig::Load);
/*operateRegister(P.SU, streamReg, [=](auto& reg) {
    reg.startConfiguration({ base, size, stride });
});*/
std::visit([&](auto& reg){
    reg.startConfiguration({ base, size, stride });
    reg.configureVecDim();
}, destReg);