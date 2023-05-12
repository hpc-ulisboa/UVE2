std::cout << "\n---ADDE---" << "\n";

std::cout << "\n---1---" << "\n";
auto streamReg = insn.uve_comp_dest();

std::cout << "\n---2---" << "\n";
auto& destReg = P.SU.registers[streamReg];

/*  
std::cout << "\n---3---" << "\n";
auto& srcReg = P.SU.registers[insn.uve_comp_src1()];



// The extra argument is passed because we need to tell the lambda the computation type. In C++20 we would
    use a lambda template parameter, however in C++17 we don't have those. As such, we pass an extra value to
    later on infer its type and know the storage we need to use 
auto baseBehaviour = [](auto& dest, auto& src, auto extra) {
  auto elements = src.getElements(true);
  auto validElementsIndex = elements.size();
  //auto validElementsIndex = std::min(elements1.size(), elements2.size());
  // Grab used types for storage and operation
  using Storage = typename std::remove_reference_t<decltype(src)>::ElementsType;
  using Operation = decltype(extra);
  decltype(dest.getElements(false)) out;
  auto value = *reinterpret_cast<Operation*>(&elements.at(0));
  std::cout << "\nADDE s1: " << elements.size() << "\n";
  for (size_t i = 1; i < validElementsIndex; i++) {
    auto e = *reinterpret_cast<Operation*>(&elements.at(i));
    value += e;
    std::cout << "Adde Iter: " << i << " element: " << e << " total sum: " << value << '\n';
  }
  out.push_back(*reinterpret_cast<Storage*>(&value));
  dest.setElements(true, out);
  std::cout << "\n\nOUT: " << out.size() << "\n\n";
};

// If the destination register is a temporary, we have to build it before the operation so that it's element size matches before any calculations are done
const bool makeTemporary = std::visit([](const auto& dest){
  return dest.getStatus() != RegisterStatus::Running;
}, destReg);
if (makeTemporary) {
  //std::cout << "\n\nMaking temporary add\n\n";
  if (std::holds_alternative<StreamReg64>(srcReg)) {
    destReg = makeStreamRegister<std::uint64_t>(RegisterType::Temporary);
    operateRegister(P.SU, streamReg, [=](auto& reg) {
      reg.endConfiguration();
    });
  } else if (std::holds_alternative<StreamReg32>(srcReg)) {
    destReg = makeStreamRegister<std::uint32_t>(RegisterType::Temporary);
    operateRegister(P.SU, streamReg, [=](auto& reg) {
      reg.endConfiguration();
    });
  } else {
    assert_msg("Trying to run so.a.adde.fp with invalid src type", false);
  }
}

std::visit(overloaded {
    [&](StreamReg64& dest, StreamReg64& src) { baseBehaviour(dest, src, double{}); },
    [&](StreamReg32& dest, StreamReg32& src) { baseBehaviour(dest, src, float{}); },
    [&](auto& dest, auto& src) { assert_msg("Invoking so.a.adde.fp with invalid parameter sizes", false); }
  },
  destReg, srcReg);

  */