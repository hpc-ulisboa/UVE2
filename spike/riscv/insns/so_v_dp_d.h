auto streamReg = insn.uve_rd();
auto& destReg = P.SU.registers[streamReg];
auto baseReg = insn.uve_rs1();
auto& srcReg = P.SU.registers[baseReg];
auto &predReg = P.SU.predicates[insn.uve_v_pred()];

const double value = READ_REG(baseReg);

auto baseBehaviour = [](auto &dest, auto &pred, const auto value) {
    auto p = pred.getPredicate();
    auto destElements = dest.getElements(false);
    auto destValidIndex = dest.getMaxElements();
    std::vector<uint64_t> out(destValidIndex, readAS<uint64_t>(value));
    for (size_t i = 0; i < destValidIndex; ++i)
        out.at(i) = p.at((i+1)*sizeof(uint64_t)-1) ? readAS<uint64_t>(value) : destElements.at(i);
    dest.setElements(true, out);
    dest.setValidIndex(destValidIndex);
};

// If the destination register is a temporary, we have to build it before the operation so that its element size matches before any calculations are done
std::visit([&](auto &dest) {
    if (dest.getStatus() != RegisterStatus::Running) {
        P.SU.makeStreamRegister<std::uint64_t>(RegisterConfig::Temporary, streamReg);
        /*operateRegister(P.SU, streamReg, [=](auto& reg) {
          reg.endConfiguration();
        });*/
        dest.endConfiguration();
    }
}, destReg);

std::visit(overloaded {
    [&, value](StreamReg64& dest) { baseBehaviour(dest, predReg, value); },
    [&, value](auto& dest) { assert_msg("Invoking so.v.dp.d with invalid parameter sizes", false); }
}, destReg);