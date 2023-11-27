auto streamReg = insn.uve_rd();
auto &destReg = P.SU.registers[streamReg];
auto &srcReg = P.SU.registers[insn.uve_rs1()];
auto &predReg = P.SU.predicates[insn.uve_v_pred()];


auto baseBehaviour = [](auto &dest, auto &src, auto &pred) {
    using StorageType = typename std::remove_reference_t<decltype(dest)>::ElementsType;
    /* We can only operate on the first available values of the stream */
    auto validElementsIndex = src.getValidIndex();

    auto elements = src.getElements();
    std::reverse(elements.begin(), elements.begin()+validElementsIndex); // reverse the valid source elements

    auto destElements = dest.getElements(false); // doesn't iterate the stream

    std::vector<StorageType> out(dest.getVLen());

    auto pi = pred.getPredicate();
    std::reverse(pi.begin(), pi.begin()+validElementsIndex*sizeof(StorageType)); // reverse the necessary instruction predicate

    for (size_t i = 0; i < validElementsIndex; ++i)
        out.at(i) = pi.at((i+1)*sizeof(StorageType)-1) ? elements.at(i) : destElements.at(i);

    dest.setValidIndex(validElementsIndex);
    dest.setElements(out);
};

/* If the destination register is not configured, we have to build it before the
operation so that its element size matches before any calculations are done */
std::visit([&](auto &dest) {
    if (dest.getStatus() == RegisterStatus::NotConfigured) {
        if (std::holds_alternative<StreamReg64>(srcReg)) {
            P.SU.makeStreamRegister<std::uint64_t>(streamReg);
			dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg32>(srcReg)) {
            P.SU.makeStreamRegister<std::uint32_t>(streamReg);
			dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg16>(srcReg)) {
            P.SU.makeStreamRegister<std::uint16_t>(streamReg);
			dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg8>(srcReg)) {
            P.SU.makeStreamRegister<std::uint8_t>(streamReg);
			dest.endConfiguration();
        } else {
            assert_msg("Trying to run so.v.mvt with invalid src type", false);
        }
    }
}, destReg);

std::visit(overloaded{
               [&](StreamReg64 &dest, StreamReg64 &src) { baseBehaviour(dest, src, predReg); },
               [&](StreamReg32 &dest, StreamReg32 &src) { baseBehaviour(dest, src, predReg); },
               [&](StreamReg16 &dest, StreamReg16 &src) { baseBehaviour(dest, src, predReg); },
               [&](StreamReg8 &dest, StreamReg8 &src) { baseBehaviour(dest, src, predReg); },
               [&](auto &dest, auto &src) { assert_msg("Invoking so.v.mvt with invalid parameter sizes", false); }},
           destReg, srcReg);