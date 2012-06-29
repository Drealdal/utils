/*
 * =====================================================================================
 *
 *       Filename:  idset.h
 *
 *    Description:  id set
 *
 *        Version:  1.0
 *        Created:  06/20/2012 09:57:07 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */

#ifndef _IDSET_H
#define _IDSET_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "debug.h"
/*  size per set */
typedef struct idsets
{
    int sizeperset;
    int cur_has_empty;
    int count;
    long long begin;
    long long end;
    int **parrent;
    unsigned char *hasEmpty;
    long long (*get)();
}idsets_t;
/**
 * @brief idset_init 
 * @param set : the set to init
 * @param begin: the begin of the id
 * @param end: the end of the id
 * @param sizeperset: the size of per set
 * @return 
 */
int idsets_init(idsets_t *set, 
	long long begin, 
	long long end,
	int sizeperset);
int idsets_get(idsets_t *set);
int idsets_release( int id, idsets_t *set );

#endif
