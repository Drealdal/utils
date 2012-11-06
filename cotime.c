/*
 * =====================================================================================
 *
 *       Filename:  cotime.c
 *
 *    Description:  collect time infomation
 *
 *        Version:  1.0
 *        Created:  04/20/2012 11:29:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "cotime.h"
#include <assert.h>

int cotime_init(cotime_t *time_sec,char *name)
{
    assert( time_sec != NULL);
    memset(time_sec, 0 , sizeof(time_sec));
    time_sec->name = name;
    time_sec->time_usec = 0;
	time_sec->count = 0;
    return 0;
}

int cotime_begin(cotime_t *time_sec)
{
    assert( time_sec != NULL );
    if(time_sec->is_started == 1)
    {
		return 0;
    }
    time_sec->is_started = 1;
    gettimeofday(&time_sec->begin, NULL);
    return 0;
}
int cotime_end(cotime_t *time_sec)
{
    assert( time_sec != NULL);
    assert( time_sec->is_started == 1);
	time_sec->count++;
    gettimeofday(&time_sec->end, NULL);
    time_sec->is_started = 0;

    time_sec->time_usec += (time_sec->end.tv_sec - time_sec->begin.tv_sec)*1000000+time_sec->end.tv_usec - time_sec->begin.tv_usec;
    return 0;
}
int cotime_usec( cotime_t *time_sec )
{
	assert( time_sec != NULL );
	return time_sec->time_usec;
}
int cotime_count( cotime_t *time_sec )
{

	assert( time_sec != NULL );
	return time_sec->count;
}
double cotime_average(cotime_t *time_sec)
{
	assert( time_sec != NULL );
	assert( time_sec->count > 0 );
	return (time_sec->time_usec*1.0)/(time_sec->count);

}
void cotime_print(cotime_t *time_sec, FILE * fd)
{
   fprintf(fd, "Total Time %s: %.6lf ms\n", time_sec->name, (time_sec->time_usec*1.0)/1000.0); 
   if( time_sec->count > 1 )
   {
	   fprintf(fd, "Average Time %s: %.6lf ms\n",time_sec->name, cotime_average(time_sec)/1000.0);
   }
}
