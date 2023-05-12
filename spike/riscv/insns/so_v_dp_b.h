#define readRegAS(T, reg) static_cast<T>( READ_REG(reg) )

auto streamReg = insn.uve_conf_destination();
/* For now, this value is not used */
auto valueReg = insn.uve_conf_base();
auto value = readRegAS(std::size_t, valueReg);

P.SU.registers[streamReg] = makeStreamRegister<std::uint8_t>(RegisterType::Duplicate, value);
operateRegister(P.SU, streamReg, [=](auto& reg) {
    reg.endConfiguration();
});
