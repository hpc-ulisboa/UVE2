auto &destReg = P.SU.registers[insn.uve_rd()];
auto origin = insn.uve_mod_origin();

std::visit([&](auto &reg) {
    reg.addModifier(std::make_shared<DynamicModifier>(Target::Size, Behaviour::Set, origin, &(P.SU)));
}, destReg);