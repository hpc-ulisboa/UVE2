// ss.app.ind.sca.ofs.add

auto &destReg = P.SU.registers[insn.uve_rd()];
auto origin = insn.uve_mod_origin();

std::visit([&](auto &reg) {
    reg.addScatterGModifier(scatterGModifier_t(dynamicBehaviour::Subtract, origin, &(P.SU)));
    reg.endConfiguration();
}, destReg);