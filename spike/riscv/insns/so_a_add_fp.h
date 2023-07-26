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
    // print pred elements
    const bool src1Check = src1.getType() == RegisterConfig::Load || src1.getType() == RegisterConfig::Temporary;
    const bool src2Check = src2.getType() == RegisterConfig::Load || src2.getType() == RegisterConfig::Temporary;
    if (src1Check && src2Check)
        assert_msg("Given streams have different widths", src1.getElementsWidth() == src2.getElementsWidth());
    /* We can only operate on the first available values of the stream */
    auto elements1 = src1.getElements(true);
    auto elements2 = src2.getElements(true);
    auto destElements = dest.getElements(false);
    auto validElementsIndex = std::min(elements1.size(), elements2.size());

    std::deque<uint8_t> p = pred.getPredicate();

    //std::cout << "\nADD s1: " << elements1.size() << "\t s2: " << elements2.size() << "\n";
    //  print elements1
    using Operation = decltype(extra);
    //std::cout << "\nADD loaded elements (u3 - b(i))\n";
    /*for (size_t i = 0; i < elements1.size(); i++) {
        std::cout << i << ": " << *reinterpret_cast<Operation *>(&elements1.at(i)) << '\n';
    }
    std::cout << "\n";*/
    if (validElementsIndex) {
        /* Grab used types for storage and operation */
        using StorageType = typename std::remove_reference_t<decltype(src1)>::ElementsType;
        using OperationType = decltype(extra);
        decltype(dest.getElements(false)) out;
        auto destValidIndex = destElements.size();
        OperationType value = 0;
        for (size_t i = 0; i < validElementsIndex; i++) {
            if(p.at(i)){
                auto e1 = *reinterpret_cast<OperationType *>(&elements1.at(i));
                auto e2 = *reinterpret_cast<OperationType *>(&elements2.at(i));
                value = e1 + e2;
                std::cout << "ADD element1: " << e1 << " element2: " << e2 << " result: " << value << "\n";
            } else {
                value =  i < destValidIndex ? *reinterpret_cast<OperationType *>(destElements.at(i)) : 0;
            }
            out.push_back(*reinterpret_cast<StorageType *>(&value));
        }
        dest.setElements(true, out);
        // std::cout << "\n\nOUT: " << out.size() << "\n\n";
    }
};

/* If the destination register is a temporary, we have to build it before the
operation so that it's element size matches before any calculations are done */
std::visit([&](auto &dest) {
    if (dest.getStatus() != RegisterStatus::Running) {
        if (std::holds_alternative<StreamReg64>(src1Reg)) {
            P.SU.makeStreamRegister<std::uint64_t>(RegisterConfig::Temporary, streamReg);
            dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg32>(src1Reg)) {
            P.SU.makeStreamRegister<std::uint32_t>(RegisterConfig::Temporary, streamReg);
            dest.endConfiguration();
        } else {
            assert_msg("Trying to run so.a.add.fp with invalid src type", false);
        }
    }
}, destReg);

std::visit(overloaded{
    [&](StreamReg64 &dest, StreamReg64 &src1, StreamReg64 &src2) { baseBehaviour(dest, src1, src2, predReg, double{}); },
    [&](StreamReg32 &dest, StreamReg32 &src1, StreamReg32 &src2) { baseBehaviour(dest, src1, src2, predReg, float{}); },
    [&](auto &dest, auto &src1, auto &src2) { assert_msg("Invoking so.a.add.fp with invalid parameter sizes", false); }
}, destReg, src1Reg, src2Reg);