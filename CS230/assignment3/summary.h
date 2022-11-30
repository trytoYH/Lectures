/* 
 * cachelab.h - Prototypes for Cache Lab helper functions
 */

#ifndef CACHELAB_TOOLS_H
#define CACHELAB_TOOLS_H

/* 
 * summaryLatency - This function provides a standard way for your cache
 * simulator * to display its final hit and miss statistics
 */ 
void summaryLatency(int l1_hits,        /* number of  hits */
                    int l1_misses,      /* number of misses */
                    int l1_evictions,   /* number of evictions */
                    int l2_hits,        /* number of  hits */
                    int l2_misses,      /* number of misses */
                    int l2_evictions);  /* number of evictions */


#endif /* CACHELAB_TOOLS_H */
