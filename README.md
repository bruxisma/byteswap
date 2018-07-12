Overview
========

This is a draft proposal for adding a `std::byteswap` function to the C++
standard library. This repository features the proposal *and* a working C++17
implementation for GCC/Clang/MSVC. On all platforms, it will insert a bswap
instruction via a compiler intrinsic if called in a non-constexpr context,
and will perform a constexpr byteswap otherwise.

The proposal is still in draft mode and will be updated with proper wording
over time.

This implementation is released under an MIT License.
