auto &destReg = P.SU.registers[insn.uve_rd()];
auto sizeReg = insn.uve_mod_size();
auto dispReg = insn.uve_mod_disp();

size_t size = READ_REG(sizeReg);
int32_t disp = READ_REG(dispReg);

/* For debug, print the indexes of each register as well as their contents 
fprintf(stderr, "UVE    Register u%ld\n", streamReg);
fprintf(stderr, "RISC-V Register x%2ld: %d (Size)\n", sizeReg, size);
fprintf(stderr, "RISC-V Register x%2ld: %d (Displacement)\n", dispReg, disp);
*/

std::visit([&](auto &reg) {
    reg.addModifier(Modifier(Modifier::Type::Static, Modifier::Target::Size, Modifier::Behaviour::Increment, disp, size));
    reg.endConfiguration();
}, destReg);