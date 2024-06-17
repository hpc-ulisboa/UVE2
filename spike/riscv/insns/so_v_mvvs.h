auto src = insn.uve_rs1();
auto &srcReg = P.SU.registers[src];
auto destReg = insn.uve_rd();

auto baseBehaviour = [](auto &src, auto &value) {
    value = src.getElements(false).at(0);
};

std::visit(overloaded{
    [&](StreamReg8 &src) { uint8_t value; baseBehaviour(src, value); WRITE_REG(destReg, value);},
    [&](StreamReg16 &src) { uint16_t value; baseBehaviour(src, value); WRITE_REG(destReg, value);},
    [&](StreamReg32 &src) { uint32_t value; baseBehaviour(src, value); WRITE_REG(destReg, value);},
    [&](StreamReg64 &src) { uint64_t value; baseBehaviour(src, value); WRITE_REG(destReg, value);},
    [&](auto &src) { assert_msg("Invoking so.v.mvvs with invalid parameter sizes", false); }},
srcReg);
