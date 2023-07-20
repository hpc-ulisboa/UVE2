auto &streamReg = P.SU.registers[insn.uve_rd()];

std::visit([&](auto &reg) {
    reg.configureDim();
}, streamReg);