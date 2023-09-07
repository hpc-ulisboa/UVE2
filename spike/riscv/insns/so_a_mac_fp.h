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
    assert_msg("Given streams have different widths", src1.getElementsWidth() == src2.getElementsWidth());
    auto elements1 = src1.getElements(true);
    auto elements2 = src2.getElements(true);

    /* Grab used types for storage and operation */
    using StorageType = typename std::remove_reference_t<decltype(dest)>::ElementsType;
    using OperationType = decltype(extra);

    if (src1.getType() == RegisterConfig::Load || src2.getType() == RegisterConfig::Load)
        std::vector<StorageType> out(dest.vLen); // zeroing predication
    else 
        auto out = dest.getElements(false); // merging predication 

    /* print elements1
    std::cout << "elements1: ";
    for (auto e : elements1) {
        std::cout << readAS<float>(e) << " ";
    }
    std::cout << std::endl;
    // print elements2
    std::cout << "elements2: ";
    for (auto e : elements2) {
        std::cout << readAS<float>(e) << " ";
    }
    std::cout << std::endl;
    // print elements3
    std::cout << "destElements: ";
    for (auto e : destElements) {
        std::cout << readAS<float>(e) << " ";
    }
    std::cout << std::endl;
    */
    
    auto validElementsIndex = std::min(src1.getValidIndex(), src2.getValidIndex());

    auto pi = pred.getPredicate();

    for (size_t i = 0; i < validElementsIndex; i++) {
        if (pi.at((i + 1) * sizeof(OperationType) - 1)) {
            auto e1 = readAS<OperationType>(elements1.at(i));
            auto e2 = readAS<OperationType>(elements2.at(i));
            auto e3 = readAS<OperationType>(destElements.at(i));
            out.at(i) = readAS<StorageType>(e1 * e2 + e3);
            //std::cout << "MAC   e1: " << e1 << " e2: " << e2 << " e3: " << e3 << " result: " << readAS<OperationType>(out.at(i)) << std::endl;
        }
    }
    dest.setValidIndex(dest.vLen);
    dest.setElements(true, out);
};

/* If the destination register is not configured, we have to build it before the
operation so that its element size matches before any calculations are done */
std::visit([&](auto &dest) {
    if (dest.getStatus() == RegisterStatus::NotConfigured) {
        if (std::holds_alternative<StreamReg64>(src1Reg)) {
            P.SU.makeStreamRegister<std::uint64_t>(RegisterConfig::NoStream, streamReg);
            dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg32>(src1Reg)) {
            P.SU.makeStreamRegister<std::uint32_t>(RegisterConfig::NoStream, streamReg);
            dest.endConfiguration();
        } else
            assert_msg("Trying to run so.a.mac.fp with invalid src type", false);
    }
}, destReg);

std::visit(overloaded{
               [&](StreamReg64 &dest, StreamReg64 &src1, StreamReg64 &src2) { baseBehaviour(dest, src1, src2, predReg, double{}); },
               [&](StreamReg32 &dest, StreamReg32 &src1, StreamReg32 &src2) { baseBehaviour(dest, src1, src2, predReg, float{}); },
               [&](auto &dest, auto &src1, auto &src2) { assert_msg("Invoking so.a.mac.fp with invalid parameter sizes", false); }
}, destReg, src1Reg, src2Reg);