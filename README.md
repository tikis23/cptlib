# cptlib

c++20 single-header library that provides wrappers around `std::chrono` for simpler use.  
Mainly exists to remove big `std::chrono` calls and `duration_cast`'s, provide easier conversion to floats/ints etc.

## Features
- `cpt::time_duration`
- `cpt::time_point`
- `cpt::clocks::pauseable_clock`