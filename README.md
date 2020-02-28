# cpp-inspectools
A suite of tools to perform different kind of inspections on C++ source and header files.

* [assembly](#assembly)
* [locinclude](#locinclude)
* [locfile](#locfile)

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
