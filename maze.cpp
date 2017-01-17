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

template <typename T, T... Args>
struct TypedList {};

template <int n>
struct I {
  static const int val = n;
};

using M = List<
  List< I<0>, I<0>, I<0>, I<0> >,
  List< I<0>, I<0>, I<0>, I<0> >,
  List< I<0>, I<0>, I<0>, I<0> >
>;

template <typename X>
constexpr auto find_position(size_t i, X x, List<>){
  return std::optional<size_t>{};
}

template <typename X, typename... T>
constexpr auto find_position(std::size_t i, X x, List<X, T...>){
  return std::optional<size_t>{i};
}

template <typename X, typename T1, typename... T>
constexpr auto find_position(size_t i, X x, List<T1, T...>){
  return find_position(i+1, x, List<T...>{});
}

template <typename X, typename T>
constexpr auto find_position(X x, T t) {
  return find_position(0, x, t);
}

using Position = std::pair<std::size_t, std::size_t>;

template <typename X>
constexpr auto find_position2(size_t i, X x, List<>){
  return std::optional<Position>{};
}

template <typename X, typename... T1, typename... T>
constexpr auto find_position2(size_t i, X x, List<List<T1...>, T...>){
  auto pos = find_position(x, List<T1...>{}); 
  if(pos) {
    return std::make_optional(Position{i, pos.value()});
  }
  else {
    return find_position2(i+1, x, List<T...>{});
  }
}

template <typename X, typename... T>
constexpr auto find_position2(X x, List<T...> t) {
  return find_position2(0, x, t);
}

constexpr auto get(std::size_t i, List<>) {
	return std::optional<int>{};
}

template <typename T1, typename... T>
constexpr auto get(std::size_t i, List<T1, T...>) {
	if(i==0) return std::make_optional(T1::val);
  	else return get(i-1, List<T...>{});
}

template <typename T>
constexpr auto get(std::size_t i, TypedList<T>) {
    return std::optional<T>{};
}

template <typename T, T t1, T... args>
constexpr auto get(std::size_t i, TypedList<T, t1, args...>) {
    if(i==0) return std::make_optional(t1);
    else return get(i-1, TypedList<T, args...>{});
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
  {
    static_assert(get(2, List<I<0>, I<2>, I<1>, I<0>>{}) == std::optional<int>{1}, "");
  }
  
  {
    static_assert(get(2, TypedList<int, 0, 2, 1, 0>{}) == std::optional<int>{1}, "");
  }
  
  return 0;
}
