# cpp-inspectools
A suite of tools to perform different kind of inspections on C++ source and header files.

* locinclude: measure the preprocessed lines of code of one or more system header includes
* locfile: measure the preprocessed lines of code of the given file

## Rationale
C++ is considered a slow language to compile and a fast language to run, even though runtime performance can be hindered by an excessive or a wrong use of abstractions and generalized coding.\
Given the right amount of effort however, C++ can actually be "a fast language to compile and a fast language to run", period.\
The purpose of these tools is to give developers the chance of taking more informed decisions, faster.

## assembly
```
assembly 0.1
A tool to produce assembly code of the given C++ file.
The tool is best suited to be run from the project root folder, since it automatically adds the include/ directory to look for header files.

USAGE:
  assembly --help
  assembly [GCC OPTIONS] [ASSEMBLY OPTIONS]

OPTIONS:
  --help:            Print this help and exit
  --verbose-cmd:     Show the compiler command
  --verbose-lines:   Show "Lines Of Assembly" acronym (implies --count)
  --compiler=bin:    The binary to run as a compiler, defaults to gcc
  --libc++:          Use libc++ instead of libstdc++ (implies --compiler=clang)
  --include=path:    Add the specified path to include paths,
                     include/ folder is automatically added by default
  --watch:           Use watchexec as a filesystem watcher to automatically
                     update output on changes
  --watch-no-clear:  Like --watch, but without clearing the screen before each run
  --keep-directives: Do not execute the removal of less important directives
  --no-demangling:   Do not apply name demangling on assembly
  --highlight:       Apply source code highlight
  --output=file:     Output to a file the preprocessed and eventually formatted code
  ${ASSEMBLY}:       Environment variable used to override locinclude's default arguments

EXAMPLES:
  assembly src/main.cpp
  assembly src/main.cpp --include=commonlib/include --watch
  assembly src/main.cpp -fno-rtti -fno-exceptions -Ofast -std=c++2a
  assembly test/dataclient_constructor.cpp --libc++
  assembly include/ui/windowing_system.cpp --keep-directives --compiler=clang

DEPENDENCIES:
  wc:                Hard requirement for counting lines of code
  gcc:               Required as a default compiler
  clang:             Required for --compiler=clang or --libc++ flag
  clang-format:      Required for --normalize
  libc++:            Required for --libc++ flag
  watchexec:         Required for --watch or --watch-no-clear flags
  source-highlight:  Required for --highlight flag
```

## locfile
```
locfile 0.3
A tool to measure the actual lines of code of the given C++ file.
The tool is best suited to be run from the project root folder, since it automatically adds the include/ directory to look for header files.
Lines counting takes place after the preprocessor stage where macros are expanded and source code is included-in and cut-out by ifdefs: this is basically what the compiler frontend is going to be fed with.
The lines of code that goes into the compiler frontend can be considered a proxy for compile-times, where the bigger the amount of LOC the greater the time needed to compile.

USAGE:
  locfile --help
  locfile [GCC OPTIONS] [LOCFILE OPTIONS]

OPTIONS:
  --help:           Print this help and exit
  --verbose-cmd:    Show the compiler command
  --verbose-lines:  Show "Lines Of Code" acronym
  --compiler=bin:   The binary to run as a compiler, defaults to gcc
  --libc++:         Use libc++ instead of libstdc++ (implies --compiler=clang)
  --include=path:   Add the specified path to include paths,
                    include/ folder is automatically added by default
  --watch:          Use watchexec as a filesystem watcher to automatically
                    update output on changes
  --watch-no-clear: Like --watch, but without clearing the screen before each run
  --normalize:      Pass through clang-format before counting lines,
                    useful for comparing different files without cheating on
                    code formatting differences
  --keep-comments:  Prevent the preprocessor from stripping away comments
  --keep-markers:   Prevent the preprocessor from stripping away linemarkers
                    preprocessing tokens
  --output=file:    Output to a file the preprocessed and eventually formatted code
  ${LOCFILE}:       Environment variable used to override locinclude's default arguments

EXAMPLES:
  locfile src/main.cpp
  locfile src/main.cpp --include=commonlib/include --watch
  locfile src/main.cpp -fno-rtti -fno-exceptions -Ofast -std=c++2a
  locfile test/dataclient_constructor.cpp --libc++
  locfile include/ui/windowing_system.cpp --keep-comments --compiler=clang

DEPENDENCIES:
  wc:               Hard requirement for counting lines of code
  gcc:              Required as a default compiler
  clang:            Required for --compiler=clang or --libc++ flag
  clang-format:     Required for --normalize
  libc++:           Required for --libc++ flag
  watchexec:        Required for --watch or --watch-no-clear flags
```

## locinclude
```
locinclude 0.3
A tool to measure the lines of code introduced by including one or more C++ system headers.
Lines counting takes place after the preprocessor stage where macros are expanded and source code is included-in and cut-out by ifdefs: this is basically what the compiler frontend is going to be fed with.
The lines of code that goes into the compiler frontend can be considered a proxy for compile-times, where the bigger the amount of LOC the greater the time needed to compile.

USAGE:
  locinclude --help
  locinclude --tabular [GCC OPTIONS] [LOCINCLUDE OPTIONS]
  locinclude [SYSTEM HEADERS] [GCC OPTIONS] [LOCINCLUDE OPTIONS]

OPTIONS:
  --help:           Print this help and exit
  --tabular:        Print a table with common system headers, separated by category
  --verbose-cmd:    Show the compiler command
  --verbose-lines:  Show "Lines Of Code" acronym
  --compiler=bin:   The binary to run as a compiler, defaults to gcc
  --libc++:         Use libc++ instead of libstdc++ (implies --compiler=clang)
  --include=path:   Add the specified path to include paths
  --normalize:      Pass through clang-format before counting lines,
                    useful for comparing different files without cheating on
                    code formatting differences
  --output=file:    Output to a file the preprocessed and eventually formatted code
  --keep-comments:  Prevent the preprocessor from stripping away comments
  --keep-markers:   Prevent the preprocessor from stripping away linemarkers
                    preprocessing tokens
  ${LOCINCLUDE}:    Environment variable used to override locinclude's default arguments

EXAMPLES:
  locinclude --tabular
  locinclude stddef.h
  locinclude cstdlib -std=c++2a
  locinclude stdlib.h --libc++
  locinclude memory utility --keep-comments --compiler=clang
  locinclude array vector utility -O3 -std=c++17

DEPENDENCIES:
  wc:               Hard requirement for counting lines of code
  gcc:              Required as a default compiler
  clang:            Required for --compiler=clang or --libc++ flag
  clang-format:     Required for --normalize
  libc++:           Required for --libc++ flag
```

## Usage by examples
The `examples/` folders contains the C++ files discussed here.

### Writing source code
For explanation sake, let's suppose I ignore the [triangular number formula](http://wikipedia.org/wiki/Triangular_number) and that I need to use an array in order to calculate the sum of numbers from 1 to 9.\
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
/home/user/cpp-inspectools/examples/c_array.cpp:2:13: warning: do not declare C-style arrays,
use std::array<> instead [cppcoreguidelines-avoid-c-arrays]
  constexpr int values[]{1, 2, 3, 4, 5, 6, 7, 8, 9};
            ^
$ █
```
Seems a legit advice, here's the updated code in [examples/std_array.cpp](examples/std_array.cpp):
```cpp
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
GCC might give me some more information using the -ftime-report and -ftime-report-details flags.\
Starting from the std::array version:
```shell
$ gcc examples/std_array.cpp -O3 -std=c++2a -ftime-report -ftime-report-details

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
GCC internals report a 37x slowdown on user time and 30x slowdown on wall time for the std::array version, with a 19x higher memory usage.\
It's interesting to note that the setup phase seems to be a constant in both reports, so the time/memory it took to compile the actual C-style array code is even smaller.

### Using locfile
Most of the time seems spent in the parsing phase.\
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
Well, I was thinking about using `std::accumulate` in combination with `std::array`, but since that would require including the `numeric` header I'm afraid of another impact in compile times.\
Let's check:
```shell
$ ./locinclude numeric --verbose
generated source code:
#include <numeric>
compiler args: -O3 --std=c++2a
lines of code: 4813
$
$ ./locinclude array numeric --verbose
generated source code:
#include <array>
#include <numeric>
compiler args: -O3 --std=c++2a
lines of code: 21523
$ █
```
The `numeric` header alone is almost 5k lines of preprocessed code, however when it is combined with the `array` header the LOC count goes from 20861 of `array` alone to 21523 for the combo: this is due to the headers sharing a portion of headers protected by [include guards](https://en.wikipedia.org/wiki/Include_guard).\
Let's check the compilation times on the `std::array`/`std::accumulate` combo on [examples/std_array_accumulate.cpp](examples/std_array_accumulate.cpp):
```shell

real    0m0,245s
user    0m0,215s
sys     0m0,039s
$ █
```
