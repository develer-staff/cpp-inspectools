# cpp-inspectools
A suite of tools to perform different kind of inspections on C++ source and header files.

## Rationale
C++ is considered a slow language to compile and a fast language to run, even though runtime performance can be hindered by an excessive or a wrong use of abstractions and generalized development.
Given the right amount of effort however, C++ can actually be "a fast language to compile and a fast language to run", period.
The purpose of these tools is to give developers the chance of taking more informed decisions, faster.

## Usage by examples
The `examples/` folders contains the C++ files discussed here.

For explanation sake, let's suppose I ignore the [triangular number formula](http://wikipedia.org/wiki/Triangular_number) and that I need to use an array in order to calculate the sum of numbers from 1 to 9.
A basic implementation with no include dependencies would look like [examples/c_array.cpp](examples/c_array.cpp):
```
int main() {
  constexpr int values[]{1, 2, 3, 4, 5, 6, 7, 8, 9};
  int sum = 0;
  for (int value : values)
    sum += value;
  return sum;
}
```
and compiles fine:
```
$ gcc examples/c_array.cpp -O3 -std=c++2a -Wall -Wextra -Wpedantic
$
```
Now it's time to run clang-tidy using the cppcoreguidelines checks:
```
$ clang-tidy --checks=cppcoreguidelines-* examples/c_array.cpp -- -std=c++2a
1 warning generated.
/home/user/cpp-inspectools/examples/c_array.cpp:2:13: warning: do not declare C-style arrays, use std::array<> instead [cppcoreguidelines-avoid-c-arrays]
  constexpr int values[]{1, 2, 3, 4, 5, 6, 7, 8, 9};
            ^
$
```
Seems a legit advice, that's the code updated:
```
