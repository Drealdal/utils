/*
 * =====================================================================================
 *
 *       Filename:  bitmap.h
 *
 *    Description:  the implement of bitmap
 *
 *        Version:  1.0
 *        Created:  07/11/2012 12:37:58 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef __BITMAP_H
#define __BITMAP_H
#include "debug.h"
#include <string.h>

#ifdef __cplusplys
extern "C" {
#endif
typedef struct bitmap bitmap;
struct bitmap
{
    int size;
    unsigned char *data;
    int ( *set)(bitmap *,int);
    int ( *unset)(bitmap *, int);
    int ( *get)(bitmap *,int);
};
int bitmap_init(bitmap *map,int size); 
void bitmap_distroy(bitmap *map);

#ifdef __cplusplys
}
#endif

#endif

