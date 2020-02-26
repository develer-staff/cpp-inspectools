#include <array>
#include <numeric>

int main() {
  constexpr std::array<int, 9> values{1, 2, 3, 4, 5, 6, 7, 8, 9};
  return std::accumulate(values.cbegin(), values.cend(), 0);
}
