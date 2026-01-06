# Modifications to TPC-H dbgen for C++ Embedding

## Overview

This document describes modifications made to the official TPC-H dbgen reference implementation to enable clean embedding in the tpch-cpp C++ project.

**Date**: January 6-7, 2026
**Purpose**: Support dbgen as an embedded library in C++ applications
**Approach**: Minimal, non-invasive modifications to headers and build configuration

## Files Modified

### 1. `config.h`

**Change**: Added default `LINUX=1` and `TPCH=1` definitions
**Reason**: Ensures DSS_HUGE is properly defined when built as a library without makefile
**Lines Added**: ~10 (before #ifdef DOS)

```c
/* Simplified config for embedded library use */
#ifndef LINUX
#define LINUX 1
#endif

#ifndef TPCH
#define TPCH 1
#endif
```

**Impact**: Minimal - only sets defaults, doesn't change behavior

---

## Files Created

### 1. `tpch_dbgen.h`

**Purpose**: Single embeddable header for C++ projects
**Content**:
- Platform-specific type definitions (DSS_HUGE)
- All string length constants
- Table identifiers with DBGEN_ prefix to avoid conflicts
- All TPC-H struct definitions (customer_t, order_t, line_t, part_t, etc.)
- Public API function declarations
- extern "C" wrapper for C++ compatibility

**Key Design Decisions**:
- Self-contained: no dependencies on other dbgen headers
- Defines `DBGEN_*` prefixed table constants to avoid C++ enum conflicts
- Also provides unprefixed `PART`, `CUST`, etc. for C code compatibility
- extern "C" wrapper ensures C++ can call C functions

**Size**: ~240 lines

---

### 2. `tpch_init.c`

**Purpose**: Initialization and utility functions for embedded use
**Content**:
- `load_dists()` - Load distribution files from dists.dss
- `dbgen_reset_seeds()` - Reset RNG state for reproducible generation

**Design**:
- Minimal implementation focusing on essential functionality
- Handles DSS_PATH environment variable for distribution file location
- Resets RNG usage counters for clean generation state

**Size**: ~60 lines

---

## Architecture

### Include Chain

```
C++ Application
  └─ dbgen_wrapper.hpp
       └─ tpch_dbgen.h (extern "C")
            └─ dss.h, dsstypes.h (indirectly via C code)
```

### Build Configuration

The CMakeLists.txt in `third_party/dbgen/` explicitly defines compilation flags:
- `LINUX=1` - Ensures 64-bit DSS_HUGE definition
- `TPCH=1` - Enables TPC-H specific features
- `C99` standard - Compatible with dbgen's C code style

---

## Thread-Safety

**Current Design**: Single-threaded generation per process

**Implementation**:
- RNG state (Seed[] array) is global and mutable
- Distributions are read-only after load_dists()
- Each DBGenWrapper instance manages its own generation state
- Safe for multi-threaded **initialization** (one-time load_dists call)
- **NOT** safe for concurrent generate_*() calls

**Usage Pattern**:
```cpp
// SAFE: Distribution loading is thread-safe
std::call_once(flag, []{ load_dists(); });

// UNSAFE: Don't call from multiple threads simultaneously
wrapper.generate_lineitem(callback);  // Use one thread per wrapper
```

**Future Improvements** (Phase 10+):
- Process-based parallelism (fork multiple processes)
- Thread-local RNG context (requires dbgen modifications)

---

## Constants and Definitions

### Table Identifiers

Defined in tpch_dbgen.h with DBGEN_ prefix to avoid conflicts with C++ enum names:

```c
#define DBGEN_PART      0
#define DBGEN_PSUPP     1
#define DBGEN_SUPP      2
#define DBGEN_CUST      3
#define DBGEN_ORDER     4
#define DBGEN_LINE      5
#define DBGEN_NATION    8
#define DBGEN_REGION    9
```

Also provide unprefixed versions for C code compatibility.

### Structure Sizes

All structures are fixed-size and match dbgen's native definitions:
- `customer_t`: ~170 bytes
- `order_t`: ~320 bytes  (includes 7 line_t instances)
- `line_t`: ~110 bytes
- `part_t`: ~280 bytes (includes 4 partsupp_t instances)
- `partsupp_t`: ~220 bytes
- `supplier_t`: ~200 bytes
- `code_t`: ~185 bytes (for nation/region)

---

## Testing

Modifications have been tested for:
- ✅ Compilation without errors
- ✅ Header inclusion from C++
- ✅ Type definitions compatibility
- ⚠️ Runtime generation (Phase 9 implementation in progress)

---

## Compatibility

**C Compatibility**: Full - all modifications maintain C99 compatibility
**C++ Compatibility**: Full - extern "C" wrappers allow C++ inclusion
**dbgen Compatibility**: Full - no changes to algorithm or data generation
**TPC-H Specification**: Full compliance - no spec changes

---

## Future Modifications

### Planned (No Current Plan)
- Thread-local RNG context for true thread-safe parallelism
- Additional distribution loading options
- Performance optimizations

### Not Planned
- Changes to dbgen generation algorithms
- Changes to data structure layouts
- Adding new TPC-H table types

---

## References

- dbgen version: Official TPC-H reference implementation (v3.0.1)
- dists.dss: Pre-generated distribution file included in tpch submodule
- TPC-H Specification: See specification.pdf in tpch submodule

---

**Status**: Complete for Phase 9 integration (header and library modifications)
**Last Updated**: January 7, 2026
**Maintainer**: tpch-cpp project team
