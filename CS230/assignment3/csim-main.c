/*
 * csim-main.c - A main function for cache simulator that can replay traces from Valgrind
 *     and output statistics such as number of hits, misses, and
 *     evictions.  Cache initialization, free and main functions are provided here
 *
 * The function printMultiSummary() is given to print output.
 * This function is provided to print the number of hits, misses and evictions.
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include "summary.h"
#include "csim.h"

//#define DEBUG_ON

/* configuration and scoreboard */
typedef struct csim_score {
    /* Counters used to record cache statistics */
    int miss_count;
    int hit_count;
    int eviction_count;
} csim_score_t;

cache_config_t l1_config = {0, 0, 0, 0, 0};
cache_config_t l2_config = {0, 0, 0, 0, 0};
static csim_score_t l1_score = {0, 0, 0};
static csim_score_t l2_score = {0, 0, 0};

/* Globals set by command line args */
int verbosity = 0;  /* print trace if set */
char* trace_file = NULL;


/*
 * accessAddr - Wrapper for accessData implementation
 */

void accessAddr(mem_addr_t addr)
{
    access_record_t result = accessData(addr);

    l1_score.hit_count += result.l1_hit;
    l1_score.miss_count += result.l1_miss;
    l1_score.eviction_count += result.l1_eviction;
    l2_score.hit_count += result.l2_hit;
    l2_score.miss_count += result.l2_miss;
    l2_score.eviction_count += result.l2_eviction;

    if(verbosity){
        if(result.l1_hit)
            printf("L1 hit ");
        if(result.l1_miss)
            printf("L1 miss ");
        if(result.l1_eviction)
            printf("L1 eviction ");
        if(result.l2_hit)
            printf("L2 hit ");
        if(result.l2_miss)
            printf("L2 miss ");
        if(result.l2_eviction)
            printf("L2 eviction ");
    }
}


/*
 * replayTrace - replays the given trace file against the cache
 */
void replayTrace(char* trace_fn)
{
    char buf[1000];
    mem_addr_t addr=0;
    unsigned int len=0;
    FILE* trace_fp = fopen(trace_fn, "r");

    if(!trace_fp){
        fprintf(stderr, "%s: %s\n", trace_fn, strerror(errno));
        exit(1);
    }

    while( fgets(buf, 1000, trace_fp) != NULL) {
        if(buf[1]=='S' || buf[1]=='L' || buf[1]=='M') {
            sscanf(buf+3, "%llx,%u", &addr, &len);

            if(verbosity)
                printf("%c %llx,%u ", buf[1], addr, len);

            accessAddr(addr);

            /* If the instruction is R/W then access again */
            if(buf[1]=='M')
                accessAddr(addr);

            if (verbosity)
                printf("\n");
        }
    }

    fclose(trace_fp);
}

/*
 * printUsage - Print usage info
 */
void printUsage(char* argv[])
{
    printf("Usage: %s [-hv] -s1 <num> -E1 <num> -s2 <num> -E2 <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s1 <num>  Number of set index bits in L1 cache.\n");
    printf("  -E1 <num>  Number of lines per set in L1 cache.\n");
    printf("  -s2 <num>  Number of set index bits in L2 cache.\n");
    printf("  -E2 <num>  Number of lines per set in L2 cache.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s1 4 -E1 1 -s2 8 -E2 2 -b 8 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s1 8 -E1 2 -s2 8 -E2 8 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}

/*
 * main - Main routine
 */
int main(int argc, char* argv[])
{
    int i = 1;

    while (i < argc){
        if (!strcmp(argv[i], "-s1"))
            l1_config.s = atoi(argv[i + 1]);
        else if (!strcmp(argv[i], "-E1"))
            l1_config.E = atoi(argv[i + 1]);
        else if (!strcmp(argv[i], "-s2"))
            l2_config.s = atoi(argv[i + 1]);
        else if (!strcmp(argv[i], "-E2"))
            l2_config.E = atoi(argv[i + 1]);
        else if (!strcmp(argv[i], "-b")){
            l1_config.b = atoi(argv[i + 1]);
            l2_config.b = atoi(argv[i + 1]);
        }
        else if (!strcmp(argv[i], "-t"))
            trace_file = argv[i + 1];
        else if (!strcmp(argv[i], "-v"))
            verbosity = 1;
        else if (!strcmp(argv[i], "-h")){
            printUsage(argv);
            exit(0);
        }
        else {
            printUsage(argv);
            exit(1);
        }
        i += 2;
    }

    /* Make sure that all required command line args were specified */
    if (l1_config.s == 0 || l1_config.E == 0 || l1_config.b == 0 ||
    l2_config.s == 0 || l2_config.E == 0 || l2_config.b == 0 || trace_file == NULL) {
    printf("%s: Missing required command line argument\n", argv[0]);
    printUsage(argv);
    exit(1);
    }

    /* Compute S, E and B from command line args */
    l1_config.S = (unsigned int) pow(2, l1_config.s);
    l1_config.B = (unsigned int) pow(2, l1_config.b);
    l2_config.S = (unsigned int) pow(2, l2_config.s);
    l2_config.B = (unsigned int) pow(2, l2_config.b);

    /* Initialize cache */
    initCache();

#ifdef DEBUG_ON
    printf("DEBUG: S1:%u E1:%u B1:%u S2:%u E2:%u B2:%u trace:%s\n",
           l1_config.S, l1_config.E, l1_config.B,
           l2_config.S, l2_config.E, l2_config.B, trace_file);
#endif

    replayTrace(trace_file);

    /* Free allocated memory */
    freeCache();

    /* Output the hit and miss statistics for the autograder */
    summaryLatency(l1_score.hit_count, l1_score.miss_count, l1_score.eviction_count,
                   l2_score.hit_count, l2_score.miss_count, l2_score.eviction_count);
    return 0;
}