#include <utility>
#include <array>
#include <optional>

template <std::size_t Y, std::size_t X>
using Maze = std::array<std::array<int, X>, Y>;

using Position = std::pair<std::size_t, std::size_t>;

enum {
  Free = 0,
  Wall = 1,
  Start = 2,
  Finish = 3
};

template<std::size_t Y, std::size_t X>
constexpr auto get(std::size_t y, std::size_t x, Maze<Y, X> m) {
  if(y < Y && x < X)
    return std::make_optional(m[y][x]);
  else 
    return std::optional<int>{};
}

template<std::size_t Y, std::size_t X>
constexpr std::optional<Position> find_position(int elem, Maze<Y, X> m) {
  for(std::size_t y = 0 ; y<Y ; ++y) {
    for(std::size_t x = 0 ; x<X ; ++x) {
      if(m[y][x] == elem) return std::make_optional(Position(y, x));
    }
  }
  return std::optional<Position>{};
}

int main() {
  constexpr Maze<3,4> m = {std::array<int,4>{1,1,2,1}, {1,1,1,1}, {1,1,1,1}};
  static_assert(find_position(2, m).value() == Position(0, 2), "");
  return 0; 
}
