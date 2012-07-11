/*
 * =====================================================================================
 *
 *       Filename:  bitmap.c
 *
 *    Description:  the implement of bitmap
 *
 *        Version:  1.0
 *        Created:  07/11/2012 12:35:48 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "bitmap.h"
static int set(bitmap *,int);
static int get(bitmap *,int); 
static int unset(bitmap *map, int n);

int bitmap_init(bitmap *map, int size)
{
    if( map == NULL )
    {
	debug(DEBUG_UTILS,"INIT NULL MAP");
	return -1;
    }
    memset(map, 0 , sizeof(bitmap));
    map->size = size;

    map->data =(unsigned char *)
       	malloc(size/8 + (size%8 != 0 ));
    if( map->data == NULL )
    {
	debug(DEBUG_UTILS,"malloc error");
	return -1;
    }
    memset(map->data, 0, size/8 + (size%8 == 0) );
    map->set = set;
    map->unset = unset;
    map->get = get;
    return 0;
}
void bitmap_distroy(bitmap *map)
{
    if( map == NULL )
    {
	debug(DEBUG_UTILS,"DISTROY NULL MAP");
	return;
    }
    free(map->data);
}
static int set(bitmap *map, int n )
{
    if( n > map->size || n < 0)
    {
	debug(DEBUG_UTILS,"Set Larger than map size:%d",n);
	return -1;
    }

    map->data[n/8] |= (1 << (n%8));
    return 0;
}
static int unset(bitmap *map, int n)
{
    if( n > map->size || n < 0)
    {
	debug(DEBUG_UTILS,"Unset Larger than map size:%d",n);
	return -1;
    }
    map->data[n/8] &=~(1<<(n%8));
    return 0;
}
static int get(bitmap *map, int n)
{
    if ( n > map->size || n < 0)
    {
	debug(DEBUG_UTILS,"Get Larger than map size:%d",n);
	return -1;
    }
    
    return ( (map->data[n/8] & ( 1 << (n%8))) != 0);
}

