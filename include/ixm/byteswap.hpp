#ifndef IXM_BYTESWAP_HPP
#define IXM_BYTESWAP_HPP

#include <type_traits>
#include <cstdint>

#define IXM_IS_CONSTEXPR(...) noexcept(ixm::impl::is_constexpr(__VA_ARGS__))

namespace ixm {
inline namespace r0 {
namespace impl {

template <class T>
using is_byteswappable = std::conjunction<
  std::is_integral<T>,
  std::bool_constant<sizeof(T) % 2 == 0>
>;

template <class T>
constexpr bool is_byteswappable_v = is_byteswappable<T>::value;

template <class T> constexpr void is_constexpr(T&& t) { }

template <class T>
constexpr auto as_unsigned (T value) noexcept {
  return static_cast<std::make_unsigned_t<T>>(value);
}

#if defined(_MSC_VER)
inline auto bswap (std::uint64_t v) noexcept { return _byteswap_uint64(v); }
inline auto bswap (std::uint32_t v) noexcept { return _byteswap_ulong(v); }
inline auto bswap (std::uint16_t v) noexcept { return _byteswap_ushort(v); }

constexpr auto const_bswap (std::uint64_t v) noexcept {
  return ((v & UINT64_C(0x0000'0000'0000'00FF)) << 56) |
         ((v & UINT64_C(0x0000'0000'0000'FF00)) << 40) |
         ((v & UINT64_C(0x0000'0000'00FF'0000)) << 24) |
         ((v & UINT64_C(0x0000'0000'FF00'0000)) <<  8) |
         ((v & UINT64_C(0x0000'00FF'0000'0000)) >>  8) |
         ((v & UINT64_C(0x0000'FF00'0000'0000)) >> 24) |
         ((v & UINT64_C(0x00FF'0000'0000'0000)) >> 40) |
         ((v & UINT64_C(0xFF00'0000'0000'0000)) >> 56);
}

constexpr auto const_bswap (std::uint32_t v) noexcept {
  return ((v & UINT32_C(0x0000'00FF)) << 24) |
         ((v & UINT32_C(0x0000'FF00)) <<  8) |
         ((v & UINT32_C(0x00FF'0000)) >>  8) |
         ((v & UINT32_C(0xFF00'0000)) >> 24);
}

constexpr auto const_bswap (std::uint16_t v) noexcept {
  return ((v & UINT16_C(0x00FF)) << 8) |
         ((v & UINT16_C(0xFF00)) >> 8);
}

#else
constexpr auto bswap (std::uint64_t v) noexcept { return __builtin_bswap64(v); }
constexpr auto bswap (std::uint32_t v) noexcept { return __builtin_bswap32(v); }
constexpr auto bswap (std::uint16_t v) noexcept { return __builtin_bswap16(v); }
#endif /* defined(_MSC_VER) */

}}} /* namespace ixm::r0::impl */

namespace ixm {
inline namespace r0 {

template <
  class IntegerType,
  class=std::enable_if_t<impl::is_byteswappable_v<IntegerType>>
> constexpr IntegerType byteswap (IntegerType value) noexcept {
  #if defined(_MSC_VER)
    if constexpr (IXM_IS_CONSTEXPR(value)) {
      return impl::const_bswap(impl::as_unsigned(value));
    }
  #endif /* defined(_MSC_VER) */
  return impl::bswap(impl::as_unsigned(value));
}

}} /* namespace ixm::r0 */

#endif /* IXM_BYTESWAP_HPP */
