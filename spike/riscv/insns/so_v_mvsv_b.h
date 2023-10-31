#define readRegAS(T, reg) static_cast<T>( READ_REG(reg) )

auto streamReg = insn.uve_rd();
auto &destReg = P.SU.registers[streamReg];
auto baseReg = insn.uve_rs1();

const uint8_t value = readRegAS(uint8_t, baseReg);


auto baseBehaviour = [](auto &dest, auto value) {
    std::vector<uint8_t> out(dest.getVLen());

    out.at(0) = value;

    dest.setMode(RegisterMode::Scalar);
    dest.setElements(true, out);
};

/* If the destination register is not configured, we have to build it before the
operation so that its element size matches before any calculations are done */
std::visit([&](auto &dest) {
    if (dest.getStatus() == RegisterStatus::NotConfigured) {
        P.SU.makeStreamRegister<std::uint8_t>(streamReg);
		dest.endConfiguration();
    }
}, destReg);

std::visit(overloaded{
    [&](StreamReg8 &dest) { baseBehaviour(dest, value); },
    [&](auto &dest) { assert_msg("Invoking so.v.mvsv.b with invalid parameter sizes", false); }},
destReg);