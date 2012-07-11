/*
 * =====================================================================================
 *
 *       Filename:  bitmap.c
 *
 *    Description:  test bitmap
 *
 *        Version:  1.0
 *        Created:  07/11/2012 01:09:53 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "bitmap.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int i, size,count;
     size = 1029;

    bitmap map; 
    bitmap_init(&map, size);
    srand(time(NULL));
    for( i = 0; i < 100; ++i )
    {
	map.set(&map, rand()%size);
    }
    for( i = 0 ; i < 100; ++i )
    {
	map.unset(&map, rand()%size);
    }

    count = 0;
    for( i = 0 ; i < size ; ++i )
    {
	if(map.get(&map,i))
	{
	    fprintf(stdout, "%d is set\n",i);
	    count ++;
	}
    }
    fprintf(stdout,"Number of setted %d\n",count);
    return 0;
}
