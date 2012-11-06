/*
 * =====================================================================================
 *
 *       Filename:  debug.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/20/2012 11:06:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "debug.h"
int debug_mask = 0;
FILE *debug_fd = NULL;
FILE *error_fd = NULL;

void setDebugFD(FILE *fd)
{
	if( fd == NULL )
	{
		debug(ALL_DEBUG,"NULL FD FOR DEBUG, USING STDOUT");
		return;
	}
	debug_fd = fd;
}

void setErrorFD(FILE *fd)
{
	if( fd == NULL )
	{
		debug(ALL_DEBUG,"NULL FD FOR Error, USING STDOUT");
		return;
	}
	error_fd = fd;
}
