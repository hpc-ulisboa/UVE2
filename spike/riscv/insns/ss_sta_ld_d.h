#define readRegAS(T, reg) static_cast<T>(READ_REG(reg))

auto streamReg = insn.uve_conf_destination();
auto &destReg = P.SU.registers[streamReg];
auto baseReg = insn.uve_conf_base();
auto sizeReg = insn.uve_conf_size();
auto strideReg = insn.uve_conf_stride();

reg_t base = READ_REG(baseReg);
int32_t size = READ_REG(sizeReg);
int32_t stride = READ_REG(strideReg);

P.SU.makeStreamRegister<std::uint64_t>(RegisterConfig::Load, streamReg);
/*operateRegister(P.SU, streamReg, [=](auto& reg) {
    reg.startConfiguration({ base, size, stride });
});*/
std::visit([&](auto& reg){
    reg.startConfiguration({ base, size, stride });
}, destReg);