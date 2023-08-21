auto streamReg = insn.uve_rd();
auto &destReg = P.SU.registers[streamReg];
auto &src1Reg = P.SU.registers[insn.uve_rs1()];
auto &src2Reg = P.SU.registers[insn.uve_rs2()];
auto &predReg = P.SU.predicates[insn.uve_pred()];

/* The extra argument is passed because we need to tell the lambda the computation type. In C++20 we would
    use a lambda template parameter, however in C++17 we don't have those. As such, we pass an extra value to
    later on infer its type and know the storage we need to use */
auto baseBehaviour = [](auto &dest, auto &src1, auto &src2, auto &pred, auto extra) {
    /* Each stream's elements must have the same width for content to be
     * operated on */
    const bool src1Check = src1.getType() == RegisterConfig::Load || src1.getType() == RegisterConfig::NoStream;
    const bool src2Check = src2.getType() == RegisterConfig::Load || src2.getType() == RegisterConfig::NoStream;
    if (src1Check && src2Check) {
        assert_msg("Given streams have different widths",
                   src1.getElementsWidth() == src2.getElementsWidth());
    }
    /* We can only operate on the first available values of the stream */
    auto elements1 = src1.getElements(true);
    auto elements2 = src2.getElements(true);
    auto destElements = dest.getElements(false);
    auto validElementsIndex = std::min(src1.getValidIndex(), src2.getValidIndex());

    auto p = pred.getPredicate();

    // std::cout << "\nDIV s1: " << elements1.size() << "\t s2: " << elements2.size() << "\n";
    // print elements2
    /*using Operation = decltype(extra);
    std::cout << "\nDIV loaded elements (u4 - L(i,i))\n";
    for (size_t i = 0; i < elements2.size(); i++) {
      std::cout << i << ": " << *reinterpret_cast<Operation*>(&elements2.at(i)) << '\n';
    }
    std::cout << "\n";*/
    if (validElementsIndex) {
        /* Grab used types for storage and operation */
        using StorageType = typename std::remove_reference_t<decltype(dest)>::ElementsType;
        using OperationType = decltype(extra);
        std::vector<StorageType> out(dest.getMaxElements());
        OperationType value = 0;
        for (size_t i = 0; i < validElementsIndex; i++) {
            if (p.at((i + 1) * sizeof(OperationType) - 1)) {
                auto e1 = readAS<OperationType>(elements1.at(i));
                auto e2 = readAS<OperationType>(elements2.at(i));
                value = e1 / e2;
            } else
                value = readAS<OperationType>(destElements.at(i));
            out.at(i) = readAS<StorageType>(value);
        }
        dest.setElements(true, out);
    }
    dest.setValidIndex(validElementsIndex);
};

/* If the destination register is not configured, we have to build it before the
operation so that it's element size matches before any calculations are done */
std::visit([&](auto &dest) {
    if (dest.getStatus() == RegisterStatus::NotConfigured) {
        // std::cout << "\n\nMaking temporary add\n\n";
        if (std::holds_alternative<StreamReg64>(src1Reg)) {
            P.SU.makeStreamRegister<std::uint64_t>(RegisterConfig::NoStream, streamReg);
            /*operateRegister(P.SU, streamReg, [=](auto& reg) {
              reg.endConfiguration();
            });*/
            dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg32>(src1Reg)) {
            P.SU.makeStreamRegister<std::uint32_t>(RegisterConfig::NoStream, streamReg);
            /*operateRegister(P.SU, streamReg, [=](auto& reg) {
              reg.endConfiguration();
            });*/
            dest.endConfiguration();
        } else
            assert_msg("Trying to run so.a.adde.fp with invalid src type", false);
    }
},
           destReg);

std::visit(overloaded{
               [&](StreamReg64 &dest, StreamReg64 &src1, StreamReg64 &src2) { baseBehaviour(dest, src1, src2, predReg, double{}); },
               [&](StreamReg32 &dest, StreamReg32 &src1, StreamReg32 &src2) { baseBehaviour(dest, src1, src2, predReg, float{}); },
               [&](auto &dest, auto &src1, auto &src2) { assert_msg("Invoking so.a.div.fp with invalid parameter sizes", false); }},
           destReg, src1Reg, src2Reg);