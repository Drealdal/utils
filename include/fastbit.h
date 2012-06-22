/*
 * =====================================================================================
 *
 *       Filename:  fastbitmap.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/21/2012 01:19:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _FASTBITMAP_H
#define _FASTBITMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "const.h"
#include "debug.h"


#ifdef __cplusplus
extern "C" {
#endif
#define FB_LEN 32
/*  the length of each leaf */
#define DEBUG_FASTBIT 1<<2
typedef struct fastbit fastbit_t;
struct fastbit
{
    int totallen;// the length cur struct represent
    int max_depth;
    int depth;
    unsigned char bits[FB_LEN/8];
    /*  sub bitmap */
    fastbit_t **sbit; 
};
int fb_init(fastbit_t *fb, int maxdepth,
       	int depth, int set_1);

#ifdef __cplusplus
}
#endif

#endif
