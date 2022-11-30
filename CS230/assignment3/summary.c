/*
 * cachelab.c - Cache Lab helper functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "summary.h"
#include <time.h>

#define L1_LATENCY 4.0
#define L2_LATENCY 10.0
#define MEM_LATENCY 100.0



/*
 * getLatency - Calculate latency based on predefined cache latency and hitrates.
 */
static double getLatency(int l1_hits, int l2_hits, int l1_misses, int l2_misses)
{
    int l1_access = l1_hits + l1_misses;
    int l2_access = l2_hits + l2_misses;

    if (!l1_access || !l2_access){
        printf("You have not implemented yet!\n");
        exit(1);
    }

    double l1_missrate = (double) l1_misses / (double) l1_access;
    double l2_missrate = (double) l2_misses / (double) l2_access;

    double l1_misspenalty = L2_LATENCY + (l2_missrate * MEM_LATENCY);
    double latency_score = L1_LATENCY + l1_missrate * l1_misspenalty;

    return latency_score;
}

/*
 * summaryLatency - Summarize the cache simulation statistics with latency calculation.
 */
void summaryLatency(int l1_hits, int l1_misses, int l1_evictions, int l2_hits, int l2_misses, int l2_evictions)
{
    printf("L1 cache\nhits:%d misses:%d evictions:%d\n", l1_hits, l1_misses, l1_evictions);
    printf("L2 cache\nhits:%d misses:%d evictions:%d\n", l2_hits, l2_misses, l2_evictions);

    double latency = getLatency(l1_hits, l2_hits, l1_misses, l2_misses);
    printf("Latency: %.6f\n", latency);

    FILE* output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d %d %d %d %.6f",
            l1_hits, l1_misses, l1_evictions,
            l2_hits, l2_misses, l2_evictions,
            latency);
    fclose(output_fp);
}
