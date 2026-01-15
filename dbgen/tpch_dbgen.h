#ifndef TPCH_DBGEN_H
#define TPCH_DBGEN_H

/*
 * Single embeddable header for TPC-H dbgen
 * Designed for C++ projects that want to use dbgen as a library
 *
 * This header wraps the original dbgen headers and provides a C++ compatible interface.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Include original dbgen headers */
#include "config.h"
#include "dss.h"
#include "dsstypes.h"

/* Table identifiers - save values and undefine to avoid C++ conflicts */
#define DBGEN_PART      0
#define DBGEN_PSUPP     1
#define DBGEN_SUPP      2
#define DBGEN_CUST      3
#define DBGEN_ORDER     4
#define DBGEN_LINE      5
#define DBGEN_NATION    8
#define DBGEN_REGION    9

#ifdef __cplusplus
/* Undefine macros that conflict with C++ code (e.g., in template parameters) */
#undef PART
#undef PSUPP
#undef SUPP
#undef CUST
#undef ORDER
#undef LINE
#undef NATION
#undef REGION
#endif

/* Public API Functions */
/* Row generation functions are already declared in dsstypes.h */

/* Additional utility functions from tpch_init.c */
void load_dists(void);
void dbgen_reset_seeds(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* TPCH_DBGEN_H */
