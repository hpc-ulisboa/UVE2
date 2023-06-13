#define readRegAS(T, reg) static_cast<T>( READ_REG(reg) )

auto streamReg = insn.uve_comp_dest();
auto& destReg = P.SU.registers[streamReg];
auto baseReg = insn.uve_comp_src1();
auto& srcReg = P.SU.registers[baseReg];

const auto value = readRegAS(std::uint8_t, baseReg);

auto baseBehaviour = [](auto& dest, const auto value) {
    decltype(dest.getElements(false)) out(dest.getMaxElements(), value);
    dest.setElements(true, out);
};

// If the destination register is a temporary, we have to build it before the operation so that its element size matches before any calculations are done
const bool makeTemporary = std::visit([](const auto& dest){
  return dest.getStatus() != RegisterStatus::Running;
}, destReg);
if (makeTemporary) {
    destReg = makeStreamRegister<std::uint8_t>(RegisterType::Temporary);
    operateRegister(P.SU, streamReg, [=](auto& reg) {
      reg.endConfiguration();
    });
}

std::visit(overloaded {
    [&, value](StreamReg8& dest) { baseBehaviour(dest, value); },
    [&, value](auto& dest) { assert_msg("Invoking so.v.dp.b with invalid parameter sizes", false); }
  },
destReg);