/* 
 * csim.c - A cache simulator that can replay traces from Valgrind
 *     and output statistics such as number of hits, misses, and
 *     evictions.  The reference replacement policy is LRU.
 *
 * Implementation and assumptions:
 *  1. Each load/store can cause at most one cache miss. 
 *  2. All implementations not using cache concept will get no point.
 *  3. If you fail to pass these cache validation steps, then you will get no point. We check cache validity in two aspects:
 *     1) Cold miss must not exceed available amount (proportional to cache size).
 *     2) When L1 cache miss ocurred, you must access L2 cache.
 *  4. If you miss to manage memory allocation (e.g. not freeing malloc’d memory), then you will get -10pt penalty.
 *
 */
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include "csim.h"

/* The cache we are simulating */
cache_t l1_cache;
cache_t l2_cache;
mem_addr_t l1_set_index_mask;
mem_addr_t l2_set_index_mask;

extern cache_config_t l1_config;
extern cache_config_t l2_config;

unsigned long long int lru_counter_l1 = 1;
unsigned long long int lru_counter_l2 = 1;
unsigned long long int lru_min1 = 1;
unsigned long long int lru_min2 = 1;



/*
 * initCache - Allocate memory, write 0's for valid and tag and LRU
 * also computes the set_index_mask
 */
void initCache()
{   
    l1_set_index_mask = pow(2, l1_config.s)-1;
    l2_set_index_mask = pow(2, l2_config.s)-1;
    l1_cache = (cache_t)malloc(sizeof( cache_set_t)*(l1_set_index_mask+1));
    l2_cache = (cache_t)malloc(sizeof( cache_set_t)*(l2_set_index_mask+1));
    for(int i=0; i<l1_set_index_mask+1; i++){
        l1_cache[i] = ( cache_set_t)malloc(sizeof( cache_line_t) * l1_config.E);
    }
    
    for(int i=0; i<l1_set_index_mask+1; i++){
        for(int j=0; j<l1_config.E; j++){
            l1_cache[i][j].valid = 0;
            l1_cache[i][j].tag = 0;
            l1_cache[i][j].lru = 0;
        }
    }
    
    
    for(int n=0; n<l2_set_index_mask+1; n++){
        l2_cache[n] = ( cache_set_t)malloc(sizeof( cache_line_t) * l2_config.E);
    }
    
    for(int n=0; n<l2_set_index_mask+1; n++){
        for(int j=0; j<l2_config.E; j++){
            l2_cache[n][j].valid = 0;
            l2_cache[n][j].tag = 0;
            l2_cache[n][j].lru = 0;
        }
        
    }
   
}

/* 
 * accessData - Access data at memory address addr.
 *   If it is already in L1 cache, increast L1 hit count
 *   If it is not in L1 cache, bring it in cache, increase L1 miss count.
 *   Also increase L1 eviction count if a line is evicted.
 *   If data is not in L1 cache, search for L2 cache.
 *   If it is in L2 cache, increast L2 hit count
 *   If it is not in L2 cache, bring it in cache, increase L2 miss count.
 *   Also increase L2 eviction count if a line is evicted.
 */
access_record_t accessData(mem_addr_t addr)
{
    access_record_t access_record = {0, 0, 0, 0, 0, 0};
    mem_addr_t tarVal1 = 0, tarVal2 = 0; 
    unsigned long long int index1=0, index2 = 0;
    int ishit1 = 0;
    int ishit2 = 0;
    
    tarVal1 = addr>>(l1_config.b+l1_config.s);
    index1 = ((addr>>l1_config.b) & l1_set_index_mask);
    tarVal2 = addr>>(l2_config.b+l2_config.s);
    index2 = ((addr>>l2_config.b) & l2_set_index_mask);
    
    for(int i=0; i<l1_config.E;i++){
        if(l1_cache[index1][i].valid != 0){
            if(l1_cache[index1][i].tag == tarVal1 ){
                l1_cache[index1][i].lru = lru_counter_l1;
                lru_counter_l1 ++;
                ishit1 = 1;
            } 
            
        }
    }
    
    //l1 hit
    if(ishit1 == 1){
        access_record.l1_hit ++;
    }
    
    //l1 miss
    else{
        access_record.l1_miss++;
        
        for(int i=0; i<l2_config.E;i++){
            if(l2_cache[index2][i].valid != 0){
                if(l2_cache[index2][i].tag == tarVal2 ){
                    l2_cache[index2][i].lru = lru_counter_l2;
                    lru_counter_l2 ++;
                    ishit2 = 1;
                    access_record.l2_hit++;
               
                } 
            
            }
        }
        
        //l2 hit
        if(ishit2 == 1){
            int nline=0;
            lru_min1 = l1_cache[index1][nline].lru;
            
            for(int i=0; i<l1_config.E; i++){
                if(l1_cache[index1][i].lru < lru_min1){
                    nline = i;
                    lru_min1 = l1_cache[index1][i].lru;
                }
            }
            
            //if l1 is full > increase eviction count
            if(l1_cache[index1][nline].valid == 1){ 
                access_record.l1_eviction++;
            }
            
            l1_cache[index1][nline].valid = 1;
            l1_cache[index1][nline].tag = tarVal1;
            l1_cache[index1][nline].lru = lru_counter_l1;
            lru_counter_l1 ++;
            
        }
        
        //l2 miss
        else{
            access_record.l2_miss++;
            int nline2 = 0;
            lru_min2 = l2_cache[index2][nline2].lru;
            
            for(int i=0; i<l2_config.E; i++){
                if(l2_cache[index2][i].lru < lru_min2){
                    nline2 = i;
                    lru_min2 = l2_cache[index2][i].lru;
                }
            }
            
            //if l2 is full > increase eviction count
            if(l2_cache[index2][nline2].valid == 1){ 
                access_record.l2_eviction++;
            }
            
            l2_cache[index2][nline2].valid = 1;
            l2_cache[index2][nline2].tag = tarVal2;
            l2_cache[index2][nline2].lru = lru_counter_l2;
            lru_counter_l2 ++;
            
            // save at l1
            int nline=0;
            lru_min1 = l1_cache[index1][nline].lru;
            
            for(int i=0; i<l1_config.E; i++){
                if(l1_cache[index1][i].lru < lru_min1){
                    nline = i;
                    lru_min1 = l1_cache[index1][i].lru;
                }
            }
            
            //if l1 is full > increase eviction count
            if(l1_cache[index1][nline].valid == 1){ 
                access_record.l1_eviction++;
            }
            
            l1_cache[index1][nline].valid = 1;
            l1_cache[index1][nline].tag = tarVal1;
            l1_cache[index1][nline].lru = lru_counter_l1;
            lru_counter_l1 ++;
            
        }
    
    }
   

    return access_record;
}

/*
 * freeCache - free allocated memory
 */
void freeCache()
{   
    
   for(int i=0; i<l1_set_index_mask+1;i++){
        free(l1_cache[i]);
    }
   free(l1_cache);

    for(int i=0; i<l2_set_index_mask+1;i++){
        free(l2_cache[i]);
    }
   free(l2_cache);

}

