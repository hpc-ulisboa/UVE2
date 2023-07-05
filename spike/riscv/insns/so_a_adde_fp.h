// std::cout << "\n---ADDE---" << "\n";
auto streamReg = insn.uve_comp_dest();
auto &destReg = P.SU.registers[streamReg];
auto &srcReg = P.SU.registers[insn.uve_comp_src1()];

// The extra argument is passed because we need to tell the lambda the computation type. In C++20 we would use a lambda template parameter, however in C++17 we don't have those. As such, we pass an extra value to later on infer its type and know the storage we need to use
auto baseBehaviour = [](auto &dest, auto &src, auto extra) {
    auto elements = src.getElements(true);
    auto validElementsIndex = elements.size();
    // Grab used types for storage and operation
    if (validElementsIndex) {
        using Storage = typename std::remove_reference_t<decltype(src)>::ElementsType;
        using Operation = decltype(extra);
        decltype(dest.getElements(false)) out;
        auto value = elements.size() > 0 ? *reinterpret_cast<Operation *>(&elements.at(0)) : 0;
        // std::cout << "\nADDE s1: " << elements.size() << "\n";
        for (size_t i = 1; i < validElementsIndex; i++) {
            auto e = *reinterpret_cast<Operation *>(&elements.at(i));
            value += e;
            // std::cout << "Adde Iter: " << i << " element: " << e << " total sum: " << value << '\n';
        }
        std::cout << "ADDE Total: " << value << '\n';
        out.push_back(*reinterpret_cast<Storage *>(&value));
        dest.setElements(true, out);
        // std::cout << "\n\nOUT: " << out.size() << "\n\n";
    }
};

// If the destination register is a temporary, we have to build it before the operation so that it's element size matches before any calculations are done
std::visit([&](auto &dest) {
    if (dest.getStatus() != RegisterStatus::Running) {
        // std::cout << "\n\nMaking temporary add\n\n";
        if (std::holds_alternative<StreamReg64>(srcReg)) {
            P.SU.makeStreamRegister<std::uint64_t>(RegisterConfig::Temporary, streamReg);
            /*operateRegister(P.SU, streamReg, [=](auto& reg) {
              reg.endConfiguration();
            });*/
            dest.endConfiguration();
        } else if (std::holds_alternative<StreamReg32>(srcReg)) {
            P.SU.makeStreamRegister<std::uint32_t>(RegisterConfig::Temporary, streamReg);
            /*operateRegister(P.SU, streamReg, [=](auto& reg) {
              reg.endConfiguration();
            });*/
            dest.endConfiguration();
        } else
            assert_msg("Trying to run so.a.adde.fp with invalid src type", false);
    }
}, destReg);

std::visit(overloaded{
               [&](StreamReg64 &dest, StreamReg64 &src) { baseBehaviour(dest, src, double{}); },
               [&](StreamReg32 &dest, StreamReg32 &src) { baseBehaviour(dest, src, float{}); },
               [&](auto &dest, auto &src) { assert_msg("Invoking so.a.adde.fp with invalid parameter sizes", false); }},
           destReg, srcReg);