/*
 * =====================================================================================
 *
 *       Filename:  bloomfilter_test.c
 *
 *    Description:  test the implement of bloome filter
 *
 *        Version:  1.0
 *        Created:  07/11/2012 02:21:34 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "bloomfilter.h"
#include "hash.h"
#include <stdlib.h>
#include <time.h>
/*  using char * as key, */
unsigned int bloom_simple_hash(unsigned char *str, int len)
{
    return simple_hash((char *)str);
}

unsigned int bloom_RS_hash(unsigned char *str, int len)
{
    return RS_hash((char *)str);
}

unsigned int bloom_JS_hash(unsigned char *str,int len)
{
    return JS_hash((char *)str);
}
#define DEBUG_TEST (1<<31)
#define GEN_NUM 10000
#define TEST_NUM 10000000 
int main(int argc, char *argv[])
{
    int ret = 0,i,j;
    int count1 = 0,count2 = 0;
    bfilter filter;
    debug_setmask(ALL_DEBUG);
    if( (ret = bfilter_init(&filter, 1000000,3)) < 0 )
    {
	debug(DEBUG_TEST,"Iinit fail");
	return ret;
    }

    filter.add_func(&filter,bloom_simple_hash);
    filter.add_func(&filter,bloom_RS_hash);
    filter.add_func(&filter,bloom_JS_hash);

    srand(time(NULL));

    for( i = 0; i < GEN_NUM; ++i )
    {
	unsigned char a[10];
	for( j = 0 ; j < 9; j++ )
	{
	    a[j] = rand()%26+'a';
	}
	a[j] = 0;
	filter.set(&filter,  a, 10);
	if( filter.check(&filter, a, 10 ) == 1)
	{
	    count1++;
	}
    }	
    for( i = 0; i < TEST_NUM; ++i )
    {
	unsigned char a[10];
	for( j = 0 ; j < 9; j++ )
	{
	    a[j] = rand()%26+'a';
	}
	a[j] = 0;
	if( filter.check(&filter, a, 10 ) == 1)
	{
	    count2++;
	}
    }
    fprintf(stdout,"First Round hit:%d Secont Round hit:%d\n",count1,count2);


    return 0;
}
