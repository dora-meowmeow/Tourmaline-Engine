\page cmake CMake flags for when compiling Tourmaline

When compiling Tourmaline, you can use the following CMake flags to affect how Tourmaline is compiled:

- `TOURMALINE_WITH_TESTS` (default = `OFF`)
  - Compiles and runs the unit tests after CMake finishes building Tourmaline.
  - This will create an executable named `TourmalineTests` in `bin` folder.
- `TOURMALINE_WITH_BENCHMARKS` (default = `ON`)
  - Compiles and runs the benchmarks alongside with unit tests.
  - Set this to `OFF` to skip benchmarks.
\note Benchmarks will only run, if `TOURMALINE_WITH_TESTS` is also set to `ON`.
