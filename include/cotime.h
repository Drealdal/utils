/*
 * =====================================================================================
 *
 *       Filename:  cotime.h
 *
 *    Description:  collect time infomation
 *
 *        Version:  1.0
 *        Created:  04/20/2012 11:30:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _COTIME_H
#define _COTIME_H
#include <sys/time.h>
#include <time.h>
#include <memory.h>
#include <sys/types.h>
#include <stdio.h>

typedef struct time_section
{
    char *name;
    long long time_usec;// total time spend on this section
    struct timeval begin,end;
    int is_started;
	int count;// the number of calling end for this time_section
}cotime_t;
int cotime_init(cotime_t *time_sec,char *name);
int cotime_begin(cotime_t *time_sec);
int cotime_end(cotime_t *time_sec);
void cotime_print(cotime_t *time_sec, FILE* fd);


#endif
