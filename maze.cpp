#include <utility>
#include <array>
#include <optional>
#include <tuple>
#include <limits>

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
constexpr auto get(Position p, Maze<Y, X> m) {
  std::size_t y = p.first;
  std::size_t x = p.second;
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

template <typename T, std::size_t N>
constexpr bool contains(T t, std::array<T, N> arr) {
  for(auto && i : arr) {
    if(i == t) return true;
  }
  return false;
}

template <typename T, std::size_t N, size_t... I>
constexpr auto insert(T t, const std::array<T, N>& arr, std::index_sequence<I...>) {
  return std::array<T, N>{t, std::get<I>(arr)...};
}

template <typename T, std::size_t N, size_t... I>
constexpr auto insert(T t, const std::array<T, N>& arr) {
  return insert(t, arr, std::make_index_sequence<N-1>{});
}

constexpr auto up(const Position & p) {
  return Position{p.first - 1, p.second};
}
constexpr auto down(const Position & p) {
  return Position{p.first + 1, p.second};
}
constexpr auto left(const Position & p) {
  return Position{p.first, p.second - 1};
}
constexpr auto right(const Position & p) {
  return Position{p.first, p.second + 1};
}

template <std::size_t Y, std::size_t X>
constexpr bool solve(Position p, const Maze<Y, X> m, const std::array<Position, X*Y> h) {
  if(contains(p, h)) return false;
  auto h1 = insert(p, h);
  auto value = get(p, m);
  if(!value.has_value()) return false;
  auto val = value.value();
  if(val == Finish) return true;
  if(val == Wall) return false;
  else return (solve(up(p), m, h1)) || 
    solve(down(p), m, h1) || 
    solve(right(p), m, h1) || 
    solve(left(p), m, h1);
}

template <std::size_t Y, std::size_t X>
constexpr bool solve(Position p, const Maze<Y, X> m) {
  return solve(p, m, std::array<Position, X*Y>{});
}

template <std::size_t Y, std::size_t X>
constexpr bool solve(const Maze<Y, X> m) {
  auto p = find_position(Start, m);
  if(!p) return false;
  else return solve(p.value(), m, std::array<Position, X*Y>{});
}

int main() {
  {
    constexpr Maze<4,4> m = {
      std::array<int,4>{1,1,2,0}, 
      std::array<int,4>{3,1,1,0}, 
      std::array<int,4>{0,1,0,0}, 
      std::array<int,4>{0,0,0,1}
    };
    static_assert(get(Position(0,0), m).value() == Wall, "");
    static_assert(find_position(Start, m).value() == Position(0, 2), "");
    static_assert(solve(m) == true, "");
  }
  
  {
    constexpr Maze<4,4> m = {
      std::array<int,4>{0,1,2,0}, 
      std::array<int,4>{3,1,1,0}, 
      std::array<int,4>{0,1,0,0}, 
      std::array<int,4>{0,1,0,1}
    };
    static_assert(solve(m) == false, "");
  }
  
  constexpr Maze<4,4> m = {
    std::array<int,4>{0,1,2,0}, 
    std::array<int,4>{3,1,1,0}, 
    std::array<int,4>{0,1,0,0}, 
    std::array<int,4>{0,0,0,1}
  };
  return solve(m); 
}
