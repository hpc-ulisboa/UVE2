#ifndef HELPERS_HPP
#define HELPERS_HPP

//#include <bit> C++ 20 :(
#include <cassert>
#include <cstdint>
#include <cstring>  // Include the header for std::memcpy
#include <type_traits>

#include <algorithm>
#include <array>
#include <cstddef> // size_t
#include <deque>
#include <iostream>
#include <memory>
#include <numeric>
#include <unordered_map>
#include <variant>
#include <vector>

/* Helper to make assert throwing contain a message. Value cond
  is expected to be an invariant. If it fails, the assert fails */
#define assert_msg(msg, cond) assert(((void)msg, cond))

template <class>
inline constexpr bool always_false_v = false;

/* This function helps cast a storage type to a computation type
  and vice-versa. */
template <typename Out, typename In>
Out readAS(In src) {
    assert_msg("Size mismatch between Out and In types", sizeof(In) == sizeof(Out));
    // return *reinterpret_cast<Out *>(&src);
    // return std::bit_cast<Out>(src); C++ 20 :(
    Out result;
    std::memcpy(&result, &src, sizeof(Out));
    return result;
}

/* The following helpers map the unsigned storage types to the corresponding
  computation type.
  WARNING: After many tries and a lot of time spent here, I wasn't able to cause
  a compilation error if a non supported storage type was given. My implementation
  was to test each supported case and return a void in other instances; however this
  helper is used inside a lambda given to a std::visit on a std::variant. As such, I
  *believe* all possibilities of the helper must be able to instanciante for the
  code to compile. As defining a variable with type void is impossible the compilation
  always failed, even with a valid storage type. The final implemantation was to
  DEFAULT THE COMPUTATION TYPE OF UNTESTED TYPES TO A SINGLE ONE. As such, it is up
  to the caller to insure only valid and expected types are given */
template <typename Storage>
using ComputationTypeFp =
    std::conditional_t<std::is_same_v<Storage, std::uint32_t>, float,
                       double>;

template <typename Storage>
using ComputationTypeSg = std::conditional_t<
    std::is_same_v<std::uint8_t, Storage>, std::int8_t,
    std::conditional_t<
        std::is_same_v<std::uint16_t, Storage>, std::int16_t,
        std::conditional_t<std::is_same_v<std::uint32_t, Storage>,
                           std::int32_t, std::int64_t>>>;

template <typename Storage>
using ComputationTypeUs = Storage;

/* Copied from cppreference as it provides a generic overloaded visitor implementation  */
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

#endif // HELPERS_HPP