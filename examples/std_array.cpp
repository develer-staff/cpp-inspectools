#include <array>

int main() {
  constexpr std::array<int, 9> values{1, 2, 3, 4, 5, 6, 7, 8, 9};
  int sum = 0;
  for (int value : values)
    sum += value;
  return sum;
}
