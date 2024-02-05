auto &destReg = P.SU.registers[insn.uve_rd()];
auto origin = insn.uve_mod_origin();

std::visit([&](auto &reg) {
    reg.addModifier(std::make_shared<dynamicModifier_t>(Target::Stride, Behaviour::Decrement, origin, &(P.SU)));
}, destReg);