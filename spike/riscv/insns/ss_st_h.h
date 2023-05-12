#define readRegAS(T, reg) static_cast<T>( READ_REG(reg) )

auto streamReg = insn.uve_conf_destination();
auto baseReg = insn.uve_conf_base();
auto sizeReg = insn.uve_conf_size();
auto strideReg = insn.uve_conf_stride();

reg_t   src   = READ_REG(baseReg);
int32_t size   = READ_REG(sizeReg);
int32_t stride = READ_REG(strideReg);


P.SU.registers[streamReg] = makeStreamRegister<std::uint16_t>(RegisterType::Store);
operateRegister(P.SU, streamReg, [=](auto& reg) {
    reg.startConfiguration({ src, size, stride });
    reg.endConfiguration();
});

