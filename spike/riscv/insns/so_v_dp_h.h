#define readRegAS(T, reg) static_cast<T>( READ_REG(reg) )

auto streamReg = insn.uve_rd();
auto& destReg = P.SU.registers[streamReg];
auto baseReg = insn.uve_rs1();
auto& srcReg = P.SU.registers[baseReg];
auto &predReg = P.SU.predicates[insn.uve_v_pred()];

const uint16_t value = readRegAS(uint16_t, baseReg);

auto baseBehaviour = [](auto &dest, auto &pred, const auto value) {
    auto pi = pred.getPredicate();
    auto destElements = dest.getElements(false);
    auto destValidIndex = dest.getMaxElements();
    std::vector<uint16_t> out(destValidIndex);
    for (size_t i = 0; i < destValidIndex; ++i)
        out.at(i) = pi.at((i+1)*sizeof(uint16_t)-1) ? value : destElements.at(i);
    dest.setElements(true, out);
    dest.setValidIndex(destValidIndex);
};

// If the destination register is a temporary, we have to build it before the operation so that its element size matches before any calculations are done
std::visit([&](auto &dest) {
    if (dest.getStatus() == RegisterStatus::NotConfigured) {
        P.SU.makeStreamRegister<std::uint16_t>(RegisterConfig::NoStream, streamReg);
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