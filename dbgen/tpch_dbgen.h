#ifndef TPCH_DBGEN_H
#define TPCH_DBGEN_H

/*
 * Single embeddable header for TPC-H dbgen
 * Designed for C++ projects that want to use dbgen as a library
 *
 * This header is self-contained and requires no other dbgen headers.
 * It defines all necessary types and provides extern "C" wrapper for C++.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * Platform-specific 64-bit type definition
 * ============================================================================
 */
#ifndef DSS_HUGE
#if defined(__linux__) || defined(LINUX)
#define DSS_HUGE long long int
#define HUGE_FORMAT "%lld"
#define HUGE_DATE_FORMAT "%02lld"
#elif defined(_WIN32) || defined(WIN32)
#define DSS_HUGE __int64
#define HUGE_FORMAT "%I64d"
#define HUGE_DATE_FORMAT "%02I64d"
#else
#define DSS_HUGE long long
#define HUGE_FORMAT "%lld"
#define HUGE_DATE_FORMAT "%02lld"
#endif
#endif /* DSS_HUGE */

/* ============================================================================
 * String length constants from dbgen/shared.h
 * ============================================================================
 */
#define DATE_LEN      13
#define PHONE_LEN     15
#define MAXAGG_LEN    20

/* Customer table */
#define C_NAME_LEN    18
#define C_ADDR_MAX    40
#define C_CMNT_MAX    117

/* Orders table */
#define O_CLRK_LEN    15
#define O_CMNT_MAX    79
#define O_LCNT_MAX    7

/* Lineitem table */
#define L_CMNT_MAX    44

/* Part table */
#define P_NAME_LEN    55
#define P_MFG_LEN     25
#define P_BRND_LEN    10
#define P_TYPE_LEN    25
#define P_CNTR_LEN    10
#define P_CMNT_MAX    23

/* Partsupp table */
#define PS_CMNT_MAX   199
#define SUPP_PER_PART 4

/* Supplier table */
#define S_NAME_LEN    25
#define S_ADDR_MAX    40
#define S_CMNT_MAX    101

/* Nation/Region table */
#define N_CMNT_MAX    152
#define R_CMNT_MAX    152

/* ============================================================================
 * Table identifiers (from dss.h)
 * Note: Prefixed with DBGEN_ to avoid conflicts with C++ enum names
 * ============================================================================
 */
#define DBGEN_PART      0
#define DBGEN_PSUPP     1
#define DBGEN_SUPP      2
#define DBGEN_CUST      3
#define DBGEN_ORDER     4
#define DBGEN_LINE      5
#define DBGEN_NATION    8
#define DBGEN_REGION    9

/* Also provide unprefixed versions for C code compatibility */
#define PART      DBGEN_PART
#define PSUPP     DBGEN_PSUPP
#define SUPP      DBGEN_SUPP
#define CUST      DBGEN_CUST
#define ORDER     DBGEN_ORDER
#define LINE      DBGEN_LINE
#define NATION    DBGEN_NATION
#define REGION    DBGEN_REGION

/* ============================================================================
 * TPC-H Data Structures (from dbgen/dsstypes.h)
 * ============================================================================
 */

typedef struct {
    DSS_HUGE custkey;
    char name[C_NAME_LEN + 3];
    char address[C_ADDR_MAX + 1];
    int alen;
    DSS_HUGE nation_code;
    char phone[PHONE_LEN + 1];
    DSS_HUGE acctbal;
    char mktsegment[MAXAGG_LEN + 1];
    char comment[C_CMNT_MAX + 1];
    int clen;
} customer_t;

typedef struct {
    DSS_HUGE okey;
    DSS_HUGE partkey;
    DSS_HUGE suppkey;
    DSS_HUGE lcnt;
    DSS_HUGE quantity;
    DSS_HUGE eprice;
    DSS_HUGE discount;
    DSS_HUGE tax;
    char rflag[1];
    char lstatus[1];
    char cdate[DATE_LEN];
    char sdate[DATE_LEN];
    char rdate[DATE_LEN];
    char shipinstruct[MAXAGG_LEN + 1];
    char shipmode[MAXAGG_LEN + 1];
    char comment[L_CMNT_MAX + 1];
    int clen;
} line_t;

typedef struct {
    DSS_HUGE okey;
    DSS_HUGE custkey;
    char orderstatus;
    DSS_HUGE totalprice;
    char odate[DATE_LEN];
    char opriority[MAXAGG_LEN + 1];
    char clerk[O_CLRK_LEN + 1];
    long spriority;
    DSS_HUGE lines;
    char comment[O_CMNT_MAX + 1];
    int clen;
    line_t l[O_LCNT_MAX];
} order_t;

typedef struct {
    DSS_HUGE partkey;
    DSS_HUGE suppkey;
    DSS_HUGE qty;
    DSS_HUGE scost;
    char comment[PS_CMNT_MAX + 1];
    int clen;
} partsupp_t;

typedef struct {
    DSS_HUGE partkey;
    char name[P_NAME_LEN + 1];
    int nlen;
    char mfgr[P_MFG_LEN + 1];
    char brand[P_BRND_LEN + 1];
    char type[P_TYPE_LEN + 1];
    int tlen;
    DSS_HUGE size;
    char container[P_CNTR_LEN + 1];
    DSS_HUGE retailprice;
    char comment[P_CMNT_MAX + 1];
    int clen;
    partsupp_t s[SUPP_PER_PART];
} part_t;

typedef struct {
    DSS_HUGE suppkey;
    char name[S_NAME_LEN + 1];
    char address[S_ADDR_MAX + 1];
    int alen;
    DSS_HUGE nation_code;
    char phone[PHONE_LEN + 1];
    DSS_HUGE acctbal;
    char comment[S_CMNT_MAX + 1];
    int clen;
} supplier_t;

typedef struct {
    DSS_HUGE code;
    char *text;
    long join;
    char comment[N_CMNT_MAX + 1];
    int clen;
} code_t;

/* ============================================================================
 * Public API Functions
 * ============================================================================
 */

/* Row generation functions - generate one row of each table */
long mk_cust(DSS_HUGE n_cust, customer_t *c);
long mk_order(DSS_HUGE index, order_t *o, long upd_num);
long mk_part(DSS_HUGE index, part_t *p);
long mk_supp(DSS_HUGE index, supplier_t *s);
int mk_nation(DSS_HUGE i, code_t *c);
int mk_region(DSS_HUGE i, code_t *c);

/* Initialization - must be called before any generation */
void dbgen_reset_seeds(void);
DSS_HUGE set_state(int t, long scale, long procs, long step, DSS_HUGE *e);

/* Distribution loading - required before generation */
void load_dists(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* TPCH_DBGEN_H */
