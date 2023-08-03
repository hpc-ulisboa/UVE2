#define readRegAS(T, reg) static_cast<T>( READ_REG(reg) )

auto streamReg = insn.uve_rd();
auto& destReg = P.SU.registers[streamReg];
auto baseReg = insn.uve_rs1();
auto& srcReg = P.SU.registers[baseReg];
auto &predReg = P.SU.predicates[insn.uve_v_pred()];


const auto value = readRegAS(std::uint32_t, baseReg);

auto baseBehaviour = [](auto& dest, auto &pred, const auto value) {
    std::deque<uint8_t> p = pred.getPredicate();
    auto destElements = dest.getElements(false);
    auto destValidIndex = destElements.size();
    std::deque<uint32_t> out;
    for (size_t i = 0; i < dest.getMaxElements(); ++i) {
        if (p.at(i))
            out.push_back(value);
        else
            out.push_back(i < destValidIndex ? destElements.at(i) : 0);
    }
    dest.setElements(true, out);
};

// If the destination register is a temporary, we have to build it before the operation so that its element size matches before any calculations are done
std::visit([&](auto &dest) {
    if (dest.getStatus() != RegisterStatus::Running) {
        P.SU.makeStreamRegister<std::uint32_t>(RegisterConfig::Temporary, streamReg);
        /*operateRegister(P.SU, streamReg, [=](auto& reg) {
          reg.endConfiguration();
        });*/
        dest.endConfiguration();
    }
}, destReg);

std::visit(overloaded {
    [&, value](StreamReg32& dest) { baseBehaviour(dest, predReg, value); },
    [&, value](auto& dest) { assert_msg("Invoking so.v.dp.w with invalid parameter sizes", false); }
  },
destReg);