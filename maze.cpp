#include <cstddef>
#include <type_traits>
#include <optional>

enum {
  Free = 0,
  Wall = 1,
  Start = 2,
  Finish = 3
};

template <typename... T>
struct List {};

template <int n>
struct I {
  static const int val = n;
};

using M = List<
  List< I<0>, I<0>, I<0>, I<0> >,
  List< I<0>, I<0>, I<0>, I<0> >,
  List< I<0>, I<0>, I<0>, I<0> >
>;

template <size_t i, typename X>
constexpr auto find_position(X x, List<>){
  return std::optional<size_t>{};
}

template <size_t i, typename X, typename... T>
constexpr auto find_position(X x, List<X, T...>){
  return std::optional<size_t>{i};
}

template <size_t i, typename X, typename T1, typename... T>
constexpr auto find_position(X x, List<T1, T...>){
  return find_position<i+1, X>(x, List<T...>{});
}

template <typename X, typename T>
constexpr auto find_position(X x, T t) {
  return find_position<0>(x, t);
}

using Position = std::pair<std::size_t, std::size_t>;

template <size_t i, typename X>
constexpr auto find_position2(X x, List<>){
  return std::optional<Position>{};
}

template <size_t i, typename X, typename... T1, typename... T>
constexpr auto find_position2(X x, List<List<T1...>, T...>){
  auto pos = find_position(x, List<T1...>{}); 
  if(pos) {
    return std::make_optional(Position{i, pos.value()});
  }
  else {
    return find_position2<i+1, X>(x, List<T...>{});
  }
}

template <typename X, typename... T>
constexpr auto find_position2(X x, List<T...> t) {
  return find_position2<0>(x, t);
}


int main() {
  {
    static_assert(find_position(I<1>{}, List<I<0>, I<2>, I<1>, I<0>>{}) == std::optional<size_t>{2}, "");
  }
  {
    using M1 = List<
      List< I<0>, I<0>, I<0>, I<0> >,
      List< I<0>, I<0>, I<1>, I<0> >,
      List< I<0>, I<0>, I<0>, I<0> >
    >;
    static_assert(find_position2(I<1>{}, M1{}) == std::make_optional(Position(1,2)), "");
  }
  
  return 0;
}
