auto streamReg = insn.uve_conf_destination();
auto &destReg = P.SU.registers[streamReg];
auto baseReg = insn.uve_conf_base();
auto sizeReg = insn.uve_conf_size();
auto strideReg = insn.uve_conf_stride();

reg_t base = READ_REG(baseReg);
int32_t size = READ_REG(sizeReg);
int32_t stride = READ_REG(strideReg);

/*
// For debug, print the indexes of each register as well as their contents 
fprintf(stderr, "UVE    Register u%ld\n", streamReg);
fprintf(stderr, "RISC-V Register x%2ld: %p (Base)\n", baseReg, (void*) base);
fprintf(stderr, "RISC-V Register x%2ld: %d (Size)\n", sizeReg, size);
fprintf(stderr, "RISC-V Register x%2ld: %d (Stride)\n", strideReg, stride);

// For testing, print all the values the configuration should generate 
for (int32_t i = 0; i < size; i++) {
    // Find where the next element to print should be 
    int32_t offset = i * stride;
    // Stride does not consider byte sizes, so we need to adjust the offset 
    int16_t byteOffset = offset * sizeof(std::int16_t);
    fprintf(stderr, "byteOffset[%d]: %d\n", offset, byteOffset);
    // Read from memory our value stored in the location given as an argument 
    short value1;
    int16_t value = MMU.load<uint16_t>(base + byteOffset);
    memcpy(&value1, &value, sizeof(value));
    fprintf(stderr, "array[%ld]: %hd\n", offset, value1);
}
*/

P.SU.makeStreamRegister<std::uint16_t>(RegisterConfig::Load, streamReg);
/*operateRegister(P.SU, streamReg, [=](auto& reg) {
    reg.startConfiguration({ base, size, stride });
    reg.endConfiguration();
});*/
std::visit([&](auto& reg){
    reg.startConfiguration({ base, size, stride });
    reg.endConfiguration();
}, destReg);