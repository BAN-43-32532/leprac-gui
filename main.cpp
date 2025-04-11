#include <iostream>
#include <magic_enum/magic_enum.hpp>

enum class Color {
  RED = -10,
  BLUE = 0,
  GREEN = 10,

  NONE = -1,
};

int main() {
  std::cout << std::boolalpha;
  // Basic
  {
    Color c1 = Color::RED;
    std::cout << magic_enum::enum_name(c1) << std::endl; // RED
  }

  // Enum value to string
  {
    Color color = Color::RED;
    auto color_name = magic_enum::enum_name(color);
    std::cout << color_name << std::endl;
    // color_name -> "RED"
  }

  // String to enum value
  {
    std::string color_name{"GREEN"};
    auto color = magic_enum::enum_cast<Color>(color_name);
    if (color.has_value()) {
      std::cout << (color.value() == Color::GREEN) << std::endl;
      // color.value() -> Color::GREEN
    }
  }
  auto value = "rEd";
  {
    // case insensitive enum_cast
    auto color =
      magic_enum::enum_cast<Color>(value, magic_enum::case_insensitive);
    std::cout << (color.value() == Color::RED) << std::endl;
  }

  {
    // enum_cast with BinaryPredicate
    auto color = magic_enum::enum_cast<Color>(value, [](char lhs, char rhs) {
      return std::tolower(lhs) == std::tolower(rhs);
    });
    std::cout << (color.value() == Color::RED) << std::endl;
  }
  {
    // enum_cast with default
    value = "rred";
    auto color_or_default =
      magic_enum::enum_cast<Color>(value).value_or(Color::NONE);
    std::cout << (color_or_default == Color::NONE) << std::endl;
  }
  return 0;
}
