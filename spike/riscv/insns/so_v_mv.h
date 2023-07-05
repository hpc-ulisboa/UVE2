auto streamReg = insn.uve_comp_dest();
auto &destReg = P.SU.registers[streamReg];
auto &srcReg = P.SU.registers[insn.uve_conf_base()];

auto baseBehaviour = [](auto &dest, auto &src) {
    /* Streams can only output/input values if they are in the running status */
    // const bool runningcheck = src.getStatus() != RegisterStatus::Finished;
    // assert_msg("Stream was not configured to be running", runningcheck);
    // if (dest.getType() == RegisterConfig::Store) {
    //   assert_msg("Store destination stream was not running",
    //     dest.getStatus() != RegisterStatus::Finished);
    // }
    /* We can only operate on the first available values of the stream */
    auto elements = src.getElements(true);
    /* Grab used types for storage and operation */
    using StorageType = typename std::remove_reference_t<decltype(src)>::ElementsType;
    using OperationType = ComputationTypeFp<StorageType>;
    decltype(dest.getElements(false)) out;
    for (size_t i = 0; i < elements.size(); i++) {
        auto e = readAS<OperationType>(elements.at(i));
        auto outPreStore = readAS<StorageType>(e);
        out.push_back(outPreStore);
    }
    dest.setElements(true, out);
};

/* If the destination register is a temporary, we have to build it before the
operation so that its element size matches before any calculations are done */
std::visit([&](auto &dest) {
    if (dest.getStatus() != RegisterStatus::Running) {
        if (std::holds_alternative<StreamReg64>(srcReg)) {
            P.SU.makeStreamRegister<std::uint64_t>(RegisterConfig::Temporary, streamReg);
			dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg32>(srcReg)) {
            P.SU.makeStreamRegister<std::uint32_t>(RegisterConfig::Temporary, streamReg);
			dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg16>(srcReg)) {
            P.SU.makeStreamRegister<std::uint16_t>(RegisterConfig::Temporary, streamReg);
			dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg8>(srcReg)) {
            P.SU.makeStreamRegister<std::uint8_t>(RegisterConfig::Temporary, streamReg);
			dest.endConfiguration();
        } else {
            assert_msg("Trying to run so.v.mv with invalid src type", false);
        }
    }
}, destReg);

std::visit(overloaded{
               [&](StreamReg64 &dest, StreamReg64 &src) { baseBehaviour(dest, src); },
               [&](StreamReg32 &dest, StreamReg32 &src) { baseBehaviour(dest, src); },
               [&](StreamReg16 &dest, StreamReg16 &src) { baseBehaviour(dest, src); },
               [&](StreamReg8 &dest, StreamReg8 &src) { baseBehaviour(dest, src); },
               [&](auto &dest, auto &src) { assert_msg("Invoking so.v.mv with invalid parameter sizes", false); }},
           destReg, srcReg);