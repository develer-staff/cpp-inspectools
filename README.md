# cpp-inspectools
A suite of tools to perform different kind of inspections on C++ source and header files.

## Rationale
C++ is considered a slow language to compile and a fast language to run, even though runtime performance can be hindered by an excessive or a wrong use of abstractions and generalized development.
Given the right amount of effort however, C++ can actually be "a fast language to compile and a fast language to run", period.
The purpose of these tools is to give developers the chance of taking more informed decisions, faster.

## Usage by examples
The `examples/` folders contains the C++ files discussed here.

### Writing source code
For explanation sake, let's suppose I ignore the [triangular number formula](http://wikipedia.org/wiki/Triangular_number) and that I need to use an array in order to calculate the sum of numbers from 1 to 9.
A basic implementation with no include dependencies would look like [examples/c_array.cpp](examples/c_array.cpp):
```cpp
int main() {
  constexpr int values[]{1, 2, 3, 4, 5, 6, 7, 8, 9};
  int sum = 0;
  for (int value : values)
    sum += value;
  return sum;
}
```
Checking that it compiles fine:
```shell
$ gcc examples/c_array.cpp -O3 -std=c++2a
$ █
```
Now it's time to run clang-tidy using the cppcoreguidelines checks:
```shell
$ clang-tidy --checks=cppcoreguidelines-* examples/c_array.cpp -- -std=c++2a
1 warning generated.
/home/user/cpp-inspectools/examples/c_array.cpp:2:13: warning: do not declare C-style arrays, use std::array<> instead [cppcoreguidelines-avoid-c-arrays]
  constexpr int values[]{1, 2, 3, 4, 5, 6, 7, 8, 9};
            ^
$ █
```cpp
Seems a legit advice, here's the updated code in [examples/std_array.cpp](examples/std_array.cpp):
```
#include <array>

int main() {
  constexpr std::array<int, 9> values{1, 2, 3, 4, 5, 6, 7, 8, 9};
  int sum = 0;
  for (int value : values)
    sum += value;
  return sum;

}
```
Recompiling:
```shell
$ gcc examples/std_array.cpp -O3 -std=c++2a
$ █
```
There was something strange this time though: the compilation seemed a little bit more choppy, it wasn't as snappy as before...
### Timing compilation times
Let's time it:
```shell
$ time gcc examples/std_array.cpp -O3 -std=c++2a

real    0m0,244s
user    0m0,199s
sys     0m0,049s
$ █
```
What about before?
```shell
$ time gcc examples/c_array.cpp -O3 -std=c++2a

real    0m0,043s
user    0m0,022s
sys     0m0,022s
$ █
```
That's almost a 6x pessimization!

### Looking at GCC internal statistics
GCC might give me some more information using the -ftime-report and -ftime-report-details flags.
Starting from the std::array version:
```shell
$ gcc examples/c_array.cpp -O3 -std=c++2a -ftime-report -ftime-report-details

Time variable                                   usr           sys          wall               GGC
 phase setup                        :   0.01 (  3%)   0.00 (  0%)   0.01 (  2%)    1360 kB (  4%)
 phase parsing                      :   0.31 ( 84%)   0.22 (100%)   0.53 ( 87%)   33185 kB ( 86%)
 phase lang. deferred               :   0.04 ( 11%)   0.00 (  0%)   0.05 (  8%)    3779 kB ( 10%)
 phase opt and generate             :   0.01 (  3%)   0.00 (  0%)   0.01 (  2%)     438 kB (  1%)
 |name lookup                       :   0.05 ( 14%)   0.01 (  5%)   0.15 ( 25%)    1596 kB (  4%)
 |overload resolution               :   0.03 (  8%)   0.01 (  5%)   0.04 (  7%)    2808 kB (  7%)
 preprocessing                      :   0.03 (  8%)   0.10 ( 45%)   0.11 ( 18%)    1397 kB (  4%)
 parser (global)                    :   0.10 ( 27%)   0.05 ( 23%)   0.16 ( 26%)   13838 kB ( 36%)
 `- template instantiation          :   0.00 (  0%)   0.00 (  0%)   0.01 (  2%)     745 kB (  2%)
 `- preprocessing                   :   0.03 (  8%)   0.10 ( 45%)   0.11 ( 18%)    1397 kB (  4%)
 `- parser inl. func. body          :   0.03 (  8%)   0.00 (  0%)   0.04 (  7%)     808 kB (  2%)
 `- parser function body            :   0.02 (  5%)   0.01 (  5%)   0.01 (  2%)     513 kB (  1%)
 `- parser struct body              :   0.00 (  0%)   0.00 (  0%)   0.01 (  2%)     659 kB (  2%)
 parser struct body                 :   0.03 (  8%)   0.01 (  5%)   0.05 (  8%)    5086 kB ( 13%)
 `- template instantiation          :   0.01 (  3%)   0.00 (  0%)   0.00 (  0%)     552 kB (  1%)
 `- parser inl. meth. body          :   0.02 (  5%)   0.00 (  0%)   0.03 (  5%)    2274 kB (  6%)
 `- varconst                        :   0.00 (  0%)   0.00 (  0%)   0.01 (  2%)       0 kB (  0%)
 parser function body               :   0.02 (  5%)   0.01 (  5%)   0.03 (  5%)    1204 kB (  3%)
 parser inl. func. body             :   0.04 ( 11%)   0.01 (  5%)   0.06 ( 10%)    1499 kB (  4%)
 parser inl. meth. body             :   0.03 (  8%)   0.00 (  0%)   0.05 (  8%)    2697 kB (  7%)
 template instantiation             :   0.10 ( 27%)   0.04 ( 18%)   0.11 ( 18%)   11186 kB ( 29%)
 `- template instantiation          :   0.02 (  5%)   0.01 (  5%)   0.05 (  8%)    1767 kB (  5%)
 tree SSA other                     :   0.01 (  3%)   0.00 (  0%)   0.00 (  0%)       0 kB (  0%)
 varconst                           :   0.00 (  0%)   0.00 (  0%)   0.01 (  2%)       4 kB (  0%)
 initialize rtl                     :   0.00 (  0%)   0.00 (  0%)   0.01 (  2%)      12 kB (  0%)
 rest of compilation                :   0.00 (  0%)   0.00 (  0%)   0.00 (  0%)       6 kB (  0%)
 `- initialize rtl                  :   0.00 (  0%)   0.00 (  0%)   0.01 (  2%)      12 kB (  0%)
 TOTAL                              :   0.37          0.22          0.61          38773 kB
$ █
```
Now the old fashioned array one:
```shell
$ gcc examples/c_array.cpp -O3 -std=c++2a -ftime-report -ftime-report-details

Time variable                                   usr           sys          wall               GGC
 phase setup                        :   0.00 (  0%)   0.00 (  0%)   0.01 ( 50%)    1360 kB ( 64%)
 phase opt and generate             :   0.01 (100%)   0.00 (  0%)   0.01 ( 50%)     152 kB (  7%)
 |name lookup                       :   0.00 (  0%)   0.00 (  0%)   0.01 ( 50%)      88 kB (  4%)
 initialize rtl                     :   0.00 (  0%)   0.00 (  0%)   0.01 ( 50%)      12 kB (  1%)
 rest of compilation                :   0.00 (  0%)   0.00 (  0%)   0.00 (  0%)       6 kB (  0%)
 `- initialize rtl                  :   0.00 (  0%)   0.00 (  0%)   0.01 ( 50%)      12 kB (  1%)
 TOTAL                              :   0.01          0.00          0.02           2125 kB
$ █
```
GCC internals report a 37x slowdown on user time and 30x slowdown on wall time for the std::array version, with a 19x higher memory usage.
It's interesting to note that the setup phase seems to be a constant in both reports, so the time/memory it took to compile the actual C-style array code is even smaller.

### Using locfile
Most of the time seems spent in the parsing phase.
What's the actual C++ preprocessed code that goes into the compiler frontend?
```shell
$ ./locfile examples/std_array.cpp --verbose
compiler args: -O3 --std=c++2a
lines of code: 20868
$
$ ./locfile examples/c_array.cpp --verbose
compiler args: -O3 --std=c++2a
lines of code: 7
$ █
```
I surely didn't write 20 thousand lines of code...

### Using locinclude
Since I included only the `array` system header, the impact must come from there.
```shell
$ ./locinclude array --verbose
generated source code:
#include <array>
compiler args: -O3 --std=c++2a
lines of code: 20861
$ █
```
Exactly.
Well, I could've used the `numeric` system header in combination with the `array` one, what would have been the impact in terms of lines of code?

