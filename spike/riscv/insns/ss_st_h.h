#define readRegAS(T, reg) static_cast<T>( READ_REG(reg) )

auto streamReg = insn.uve_rd();
auto &destReg = P.SU.registers[streamReg];
auto baseReg = insn.uve_conf_base();
auto sizeReg = insn.uve_conf_size();
auto strideReg = insn.uve_conf_stride();

uint64_t base = READ_REG(baseReg);
unsigned int size = READ_REG(sizeReg);
int stride = READ_REG(strideReg);


P.SU.makeStreamRegister<std::uint16_t>(streamReg, RegisterConfig::Store);
/*operateRegister(P.SU, streamReg, [=](auto& reg) {
    reg.startConfiguration({ base, size, stride });
    reg.endConfiguration();
});*/
std::visit([&](auto& reg){
    reg.startConfiguration({ base, size, stride });
    reg.endConfiguration();
}, destReg);