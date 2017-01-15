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


int main() {
  return 0;
}
