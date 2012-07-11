/*
 * =====================================================================================
 *
 *       Filename:  bloomfilter.h
 *
 *    Description:  the implement of bloom filter
 *
 *        Version:  1.0
 *        Created:  07/11/2012 12:06:32 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef __BLOOM_FILTER_
#define __BLOOM_FILTER_
#include "bitmap.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef int (*BLOOM_FILTER_HASH)(void *, int);
typedef struct bloom_filter bfilter;
struct bloom_filter
{
    /**
     * @brief number of hash function
     */
    int fnum;
    int cfnum;
    bitmap map;
    BLOOM_FILTER_HASH *hash;

    int ( *set)(bfilter *, void *,int);
    int ( *check)(bfilter *,void *,int);
    int ( *add_func)(bfilter *, BLOOM_FILTER_HASH);

};
int bfilter_init(bfilter *filter, int size);

#ifdef __cplusplus
}
#endif

#endif
