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

struct fastbit
{
    int totallen;// the length cur struct represent
    unsigned char bits[32];

}fastbit_t;

#endif
