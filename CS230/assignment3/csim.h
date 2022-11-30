//
// Created by Kyumin Park on 2021/03/30.
//

#ifndef CACHELAB_CSIM_H
#define CACHELAB_CSIM_H

#define ADDRESS_LENGTH 64

/* Type: Memory address */
typedef unsigned long long int mem_addr_t;

/* Type: Cache line
   LRU is a counter used to implement LRU replacement policy  */
typedef struct cache_line {
    char valid;
    mem_addr_t tag;
    unsigned long long int lru;
} cache_line_t;

typedef cache_line_t* cache_set_t;
typedef cache_set_t* cache_t;

/* Configurations */
typedef struct cache_config {
    int s; /* set index bits */
    int b; /* block offset bits */
    int E; /* associativity */

    /* Derived from command line args */
    int S; /* number of sets */
    int B; /* block size (bytes) */
} cache_config_t;

typedef struct access_record {
    int l1_hit;
    int l1_miss;
    int l1_eviction;
    int l2_hit;
    int l2_miss;
    int l2_eviction;
} access_record_t;

void initCache();
access_record_t accessData(mem_addr_t addr);
void freeCache();

#endif //CACHELAB_CSIM_H
