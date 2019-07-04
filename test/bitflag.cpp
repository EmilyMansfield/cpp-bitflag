#include "bitflag/bitflag.hpp"
#include <catch2/catch.hpp>

struct OddBits : em::Bitflag<19u, OddBits> {
  static constexpr enum_t A{1u << 18u};
};

TEST_CASE("Can use non-power-of-2 sizes") {
  STATIC_REQUIRE(OddBits::num_bits == 19u);
  STATIC_REQUIRE(sizeof(OddBits::underlying_t) >= 19u / 8u);

  OddBits flag = OddBits::make(OddBits::A);
  REQUIRE(static_cast<OddBits::underlying_t>(flag) == (1u << 18u));
}

struct SmallFlag : em::Bitflag<8u, SmallFlag> {
  static constexpr enum_t None{0u};
  static constexpr enum_t Foo{1u};
  static constexpr enum_t Bar{2u};
  static constexpr enum_t Baz{4u};
};

TEST_CASE("Has correct type properties") {
  STATIC_REQUIRE(std::is_copy_constructible_v<SmallFlag>);
  STATIC_REQUIRE(std::is_nothrow_move_assignable_v<SmallFlag>);
  STATIC_REQUIRE(std::is_copy_assignable_v<SmallFlag>);
  STATIC_REQUIRE(std::is_nothrow_move_assignable_v<SmallFlag>);
  STATIC_REQUIRE(!std::is_constructible_v<SmallFlag, SmallFlag::underlying_t>);
}

TEST_CASE("Can compare flags") {
  SmallFlag empty{SmallFlag::make(SmallFlag::None)};
  SmallFlag foo{SmallFlag::make(SmallFlag::Foo)};
  SmallFlag bar{SmallFlag::make(SmallFlag::Bar)};

  REQUIRE(foo == foo);
  REQUIRE_FALSE(foo == bar);
  REQUIRE_FALSE(bar == foo);
  REQUIRE(foo != bar);
  REQUIRE_FALSE(foo != foo);

  REQUIRE(foo);
  REQUIRE(bar);

  REQUIRE_FALSE(empty);
}

TEST_CASE("Can use bitwise operators") {
  using base_t = SmallFlag::underlying_t;

  SmallFlag empty{SmallFlag::make(SmallFlag::None)};
  SmallFlag foo{SmallFlag::make(SmallFlag::Foo)};
  SmallFlag bar{SmallFlag::make(SmallFlag::Bar)};

  SECTION("Can use bitwise or") {
    REQUIRE(static_cast<base_t>(foo | bar) == 0b11u);
    REQUIRE((foo | bar) == (bar | foo));
    REQUIRE((foo | empty) == foo);
    REQUIRE((foo | foo) == foo);

    SmallFlag tmp{foo};
    tmp |= bar;
    REQUIRE(tmp == (foo | bar));
  }

  SECTION("Can use bitwise and") {
    REQUIRE(static_cast<base_t>(foo & bar) == 0u);
    REQUIRE((foo & bar) == (bar & foo));
    REQUIRE((foo & empty) == empty);
    REQUIRE((foo & foo) == foo);

    SmallFlag tmp{foo};
    tmp &= bar;
    REQUIRE(tmp == (foo & bar));
  }

  SECTION("Can use bitwise xor") {
    REQUIRE(static_cast<base_t>(foo ^ bar) == 0b11u);
    REQUIRE((foo ^ bar) == (bar ^ foo));
    REQUIRE((foo ^ foo) == empty);
    REQUIRE((foo ^ empty) == foo);

    SmallFlag tmp{foo};
    tmp ^= bar;
    REQUIRE(tmp == (foo ^ bar));
  }

  SECTION("Can ue bitwise not") {
    REQUIRE(static_cast<base_t>(~bar) == 0b11111101u);
    REQUIRE(~~bar == bar);
  }
}

struct ZeroSize : em::Bitflag<0u, ZeroSize> {};

TEST_CASE("Can make zero size flag", "[static]") {
  STATIC_REQUIRE(ZeroSize::num_bits == 0u);
  STATIC_REQUIRE(sizeof(ZeroSize::underlying_t) <= 1u);
}
