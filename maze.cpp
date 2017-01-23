#include <utility>
#include <array>
#include <optional>
#include <tuple>
#include <limits>

enum MazeElement{
  Free = 0,
  Wall = 1,
  Start = 2,
  Finish = 3
};

using Position = std::pair<std::size_t, std::size_t>;

template <std::size_t Y, std::size_t X>
using Maze = std::array<std::array<int, X>, Y>;

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
constexpr std::optional<Position> find_position(MazeElement elem, Maze<Y, X> m) {
  for(std::size_t y = 0 ; y<Y ; ++y) {
    for(std::size_t x = 0 ; x<X ; ++x) {
      if(m[y][x] == elem) return std::make_optional(Position(y, x));
    }
  }
  return std::optional<Position>{};
}

template <typename T, typename... Tail>
constexpr auto ignore_tail(T t, Tail...) {
  return t;
}

template <size_t N, typename T, size_t... I>
constexpr auto create_array_impl(T t, std::index_sequence<I...>) {
  return std::array<T, N>{ignore_tail(t, I)...};
}

template <size_t N, typename T>
constexpr auto create_array(T t) {
  return create_array_impl<N>(t, std::make_index_sequence<N>{});
}


template <typename T, std::size_t N>
constexpr bool contains(T t, std::array<T, N> arr) {
  for(auto && i : arr) {
    if(i == t) return true;
  }
  return false;
}

template <typename T, std::size_t N, size_t... I>
constexpr auto insert(T t, std::array<T, N> arr, std::index_sequence<I...>) {
  return std::array<T, N>{t, std::get<I>(arr)...};
}

template <typename T, std::size_t N, size_t... I>
constexpr auto insert(T t, std::array<T, N> arr) {
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
constexpr bool solve(Position pos, Maze<Y, X> m, std::array<Position, X*Y> path) {
  if(contains(pos, path)) return false;
  auto new_path = insert(pos, path);
  auto opt = get(pos, m);
  if(!opt.has_value()) return false;
  auto val = opt.value();
  if(val == Finish) return true;
  if(val == Wall) return false;
  else return (solve(up(pos), m, new_path)) || 
    solve(down(pos), m, new_path) || 
    solve(right(pos), m, new_path) || 
    solve(left(pos), m, new_path);
}

template <std::size_t Y, std::size_t X>
constexpr bool solve(Position p, Maze<Y, X> m) {
  using size_t_limits = std::numeric_limits<size_t>;
  auto arr = create_array<Y*X>(Position{size_t_limits::max(), size_t_limits::max()});
  return solve(p, m, arr);
}

template <std::size_t Y, std::size_t X>
constexpr bool solve(Maze<Y, X> m) {
  auto p = find_position(Start, m);
  return p.has_value() && solve(p.value(), m);
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
  
  constexpr Maze<6,6> m = {
    std::array<int,6>{3,1,2,0,1,0}, 
    std::array<int,6>{0,1,1,0,0,0}, 
    std::array<int,6>{0,1,0,0,1,0}, 
    std::array<int,6>{0,0,0,1,0,0},
    std::array<int,6>{0,1,0,0,0,1},
    std::array<int,6>{0,0,0,1,1,0}
  };
  return solve(m); 
}
