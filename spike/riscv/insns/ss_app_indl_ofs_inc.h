auto &destReg = P.SU.registers[insn.uve_rd()];
auto origin = insn.uve_mod_origin();

std::visit([&](auto &reg) {
    reg.addDynamicModifier(dynamicModifier_t(Target::Offset, dynamicBehaviour::Increment, origin, &(P.SU)));
}, destReg);