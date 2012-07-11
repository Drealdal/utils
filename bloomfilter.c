/*
 * =====================================================================================
 *
 *       Filename:  bloomfilter.c
 *
 *    Description:  the implement of bloomfilter
 *
 *        Version:  1.0
 *        Created:  07/11/2012 01:41:49 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "bloomfilter.h"

static int set(bfilter *, unsigned char *, int );
static int check(bfilter *, unsigned char *, int );
static int add_func(bfilter *,BLOOM_FILTER_HASH);

int bfilter_init(bfilter *filter,
       	int size,
	int fnum)
{
    if( filter == NULL )
    {
	debug(DEBUG_UTILS, "INTI NULL filter");
	return -1;
    }

    memset(filter, 0, sizeof(bfilter));
    bitmap_init(&filter->map, size);

    filter->fnum = fnum;
    filter->hash = (BLOOM_FILTER_HASH *)malloc
	(fnum*sizeof(BLOOM_FILTER_HASH));
    if( filter->hash == NULL )
    {
	debug(DEBUG_UTILS,"Malloc Error");
	return -1;
    }
    filter->set = set;
    filter->check = check;
    filter->add_func = add_func;
    return 0;
}

static int set(bfilter *filter, unsigned char *data, int len )
{
    int i = 0;

    if( filter == NULL )
    {
	debug(DEBUG_UTILS,"set null filter");
	return -1;
    }

    if( filter->cfnum == 0 )
    {
	debug(DEBUG_UTILS,"no hash function");
	return -1;
    }
    for( i = 0 ; i < filter->cfnum; i++ )
    {
	int r = filter->hash[i](data,len);
	filter->map.set(&filter->map, r%filter->map.size);
    }	

    return 0;
}

static int check(bfilter *filter, unsigned char *data, int len )
{
    int i = 0;

    if( filter == NULL )
    {
	debug(DEBUG_UTILS,"set null filter");
	return -1;
    }

    if( filter->cfnum == 0 )
    {
	debug(DEBUG_UTILS,"no hash function");
	return -1;
    }
    for( i = 0 ; i < filter->cfnum; i++ )
    {
	int r = filter->hash[i](data,len);
	if (
		!filter->map.get(&filter->map, 
		   r%filter->map.size) )
	{
	    return 0;
	}
    }	
    return 1;
}
static int add_func(bfilter * filter, BLOOM_FILTER_HASH func)
{
    if( filter == NULL )
    {
	debug(DEBUG_UTILS,"operate on null filter");
	return -1;
    }
    if( func == NULL )
    {
	debug(DEBUG_UTILS,"function should not be null");
	return -1;
    }

    if( filter->cfnum >= filter->fnum )
    {
	debug(DEBUG_UTILS,"too many hash functions cfnum:%d fnum:%d",filter->cfnum,filter->fnum-1);
	return -1;
    }
    filter->hash[filter->cfnum++] = func;
    return 0;
}
