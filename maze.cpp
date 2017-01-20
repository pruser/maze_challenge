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

template <int N>
struct I {
  static const int val = N;
};

template<int I1, int I2>
constexpr bool operator==(const I<I1>&, const I<I2>&) {
  return I1 == I2;
}

using M = List<
  List< I<0>, I<0>, I<0>, I<0> >,
  List< I<0>, I<0>, I<0>, I<0> >,
  List< I<0>, I<0>, I<0>, I<0> >
>;

template <typename X, typename T>
constexpr bool contains(X x, TypedList<T>) {
  return false;
}

template <typename X, typename T, T t1, T... args>
constexpr bool contains(X x, TypedList<T, t1, args...>) {
  if(x == t1) return true;
  else return contains(x, TypedList<T, args...>{});
}

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

template <typename T>
constexpr auto _get(std::size_t i, TypedList<T>) {
  return std::optional<T>{};
}

template <typename T, T t1, T... args>
constexpr auto _get(std::size_t i, TypedList<T, t1, args...>) {
  if(i==0) return std::make_optional(t1);
  else return _get(i-1, TypedList<T, args...>{});
}

template <typename T, T... args>
constexpr auto get(std::size_t i, TypedList<T, args...> t) {
  return _get(i, t);
}

template <std::size_t i, typename T, T t, T... args>
struct _get_impl_typedlist {
  static const T value = _get_impl_typedlist<i-1, T, args...>::value;
};

template <typename T, T t, T... args>
struct _get_impl_typedlist<0, T, t, args...> {
  static const T value = t;
};

template <std::size_t I, typename T, T... args>
constexpr auto get(TypedList<T, args...> t) {
  static_assert(I <= sizeof...(args), "");
  return _get_impl_typedlist<I, T, args...>::value;
}


template <std::size_t I, typename T1, typename... T>
struct _get_impl_list {
  using Type = typename _get_impl_list<I-1, T...>::Type;
};

template <typename T1, typename... T>
struct _get_impl_list<0, T1, T...> {
  using Type = T1;
};

template <std::size_t I, typename... T>
constexpr auto get(List<T...> t) {
  static_assert(I <= sizeof...(T), "");
  return typename _get_impl_list<I, T...>::Type{};
}

template <std::size_t Y, std::size_t X, typename T>
constexpr auto get(T t) {
  return get<X>(get<Y>(t));
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
    static_assert(contains(1, TypedList<int, 0, 2, 1, 0>{}) == true, "");
  }
  
  {
    static_assert(contains(3, TypedList<int, 0, 2, 1, 0>{}) == false, "");
  }
  
  {
    static_assert(get<2>(List<I<0>, I<2>, I<1>, I<0>>{}) == I<1>{}, "");
  }
  
  {
    static_assert(get<2>(TypedList<int, 0, 2, 1, 0>{}) == 1, "");
  }
  
  {
    static_assert(get(2, TypedList<int, 0, 2, 1, 0>{}) == std::optional<int>{1}, "");
  }
  
  {
    using M1 = List<
      TypedList<int, 0,0,0,0 >,
      TypedList<int, 0,0,1,0 >,
      TypedList<int, 0,0,0,0 >
    >;
   	static_assert(get<1,2>(M1{}) == 1, "");
  }
  return 0;  
}
