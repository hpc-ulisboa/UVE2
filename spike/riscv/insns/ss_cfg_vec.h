auto &streamReg = P.SU.registers[insn.uve_conf_destination()];

std::visit([&](auto &reg) {
    reg.configureDim();
}, streamReg);