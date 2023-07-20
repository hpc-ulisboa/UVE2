#define readRegAS(T, reg) static_cast<T>( READ_REG(reg) )

auto streamReg = insn.uve_rd();
auto& destReg = P.SU.registers[streamReg];
auto baseReg = insn.uve_rs1();
auto& srcReg = P.SU.registers[baseReg];
auto &predReg = P.SU.predicates[insn.uve_pred()];


const auto value = readRegAS(std::uint16_t, baseReg);

auto baseBehaviour = [](auto& dest, auto &pred, const auto value) {
    decltype(dest.getElements(false)) out(dest.getMaxElements(), value);
    dest.setElements(true, out);
};

// If the destination register is a temporary, we have to build it before the operation so that its element size matches before any calculations are done
std::visit([&](auto &dest) {
    if (dest.getStatus() != RegisterStatus::Running) {
        P.SU.makeStreamRegister<std::uint16_t>(RegisterConfig::Temporary, streamReg);
        /*operateRegister(P.SU, streamReg, [=](auto& reg) {
          reg.endConfiguration();
        });*/
        dest.endConfiguration();
    }
}, destReg);

std::visit(overloaded {
    [&, value](StreamReg16& dest) { baseBehaviour(dest, predReg, value); },
    [&, value](auto& dest) { assert_msg("Invoking so.v.dp.h with invalid parameter sizes", false); }
  },
destReg);