# `cpp-bitflag`

Strongly typed flags with automatic bitwise operations and without macros.

*This is not intended to be a fancy enums library; it doesn't have pretty
printing or anything like that.*

Example usage:
```cpp
struct MyFlags : Bitflag<32, MyFlags> {
  static constexpr enum_t None{0u};
  static constexpr enum_t DoFoo{1u};
  static constexpr enum_t DoBar{1u << 1u};
};

MyFlags flags{MyFlags::make(MyFlags::DoFoo)};
if (flags & MyFlags::DoFoo) doFoo();
if (flags & MyFlags::DoBar) doBar();
```
