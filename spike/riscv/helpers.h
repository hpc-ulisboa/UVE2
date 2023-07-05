#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <cassert>
#include <cstdint>
#include <type_traits>

/* Helper to make assert throwing contain a message. Value cond
  is expected to be an invariant. If it fails, the assert fails */
#define assert_msg(msg, cond) assert(((void)msg, cond))

template <class> inline constexpr bool always_false_v = false;


/* This function helps cast a storage type to a computation type 
  and vice-versa. */
template <typename Out, typename In> Out readAS(In src) {
  /* Due to weird type convertions with types of size 1 and 2, the following line
    is commented although the code works as intended, but is expected care from the caller */
  // assert_msg("Types Out and In differ in size. Was expected the same", sizeof(In) == sizeof(Out));
  return *reinterpret_cast<Out *>(&src);
}

/* The following helpers map the unsigned storage types to the corresponding
  computation type.
  WARNING: After many tries and a lot of time spent here, I wasn't able to cause
  a compilation error if a non supported storage type was given. My implementation
  was to test each suppoerted case and return a void in other instances; however this
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

template <typename Storage> using ComputationTypeUs = Storage;

#endif // HELPERS_HPP