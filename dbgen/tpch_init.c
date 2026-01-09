/*
 * tpch_init.c - Initialization stubs for embedded dbgen usage
 *
 * Provides distribution loading and RNG state management functions
 * needed for C++ embedding of dbgen.
 */

#include "dss.h"
#include "dsstypes.h"
#include <string.h>

/* External declarations from dss.h (included above) */
extern seed_t Seed[];

/*
 * load_dists() - Load all required distributions from dists.dss
 *
 * Loads distribution tables needed for TPC-H data generation.
 * Looks for dists.dss in DSS_PATH directory (defaults to current directory).
 *
 * This is called once during initialization before any generation begins.
 */
void load_dists(void)
{
    char *dpath;

    /* Get distribution file path from environment or use current directory */
    dpath = getenv(DIST_TAG);
    if (dpath == NULL || *dpath == '\0') {
        dpath = DIST_DFLT;
    }

    if (verbose > 0) {
        fprintf(stderr, "Loading distributions from %s\n", dpath);
    }

    /* Load all required distributions */
    read_dist(dpath, "nations", &nations);
    read_dist(dpath, "regions", &regions);
    read_dist(dpath, "o_priority_set", &o_priority_set);
    read_dist(dpath, "l_instruct_set", &l_instruct_set);
    read_dist(dpath, "l_smode_set", &l_smode_set);
    read_dist(dpath, "l_category_set", &l_category_set);
    read_dist(dpath, "l_rflag_set", &l_rflag_set);
    read_dist(dpath, "c_mseg_set", &c_mseg_set);
    read_dist(dpath, "p_types_set", &p_types_set);
    read_dist(dpath, "p_cntr_set", &p_cntr_set);

    if (verbose > 0) {
        fprintf(stderr, "Distributions loaded successfully\n");
    }
}

/*
 * dbgen_reset_seeds() - Reset RNG state for reproducible generation
 *
 * Clears the RNG usage counters to prepare for fresh data generation.
 * Should be called before generating each table.
 */
void dbgen_reset_seeds(void)
{
    int i;

    /* Reset usage counters for all RNG streams */
    for (i = 0; i <= MAX_STREAM; i++) {
        Seed[i].usage = 0;
    }
}
