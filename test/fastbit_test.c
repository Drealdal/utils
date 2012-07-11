/*
 * =====================================================================================
 *
 *       Filename:  fastbit_test.c
 *
 *    Description:  test the implement of fastbit
 *
 *        Version:  1.0
 *        Created:  06/21/2012 07:07:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "fastbit.h"
int main(int argc, char *argv [] )
{

    int  i = 0;

    int n = atoi(argv[1]);
    int n2 = atoi(argv[2]);
    debug_setmask(DEBUG_FASTBIT);

    fastbit_t fb;

    if( fb_init(&fb, max_height(),0,0) < 0 )
    {
	debug(DEBUG_FASTBIT,"init failure");
	return RET_FAILURE;
    }

    for( i = 0; i < n ; i++ )
    {
	printf("%d\n",fb_set_first0_1(&fb));
    }	
    char * a = (char *)malloc(100);
    memset(a , 0, sizeof(100));


    for( i = 0 ; i < n2 ; i++ )
    {
	fb_setn1_0(&fb, i);
    }	
    for( i = 0; i < n ; i++ )
    {
	printf("%d\n",fb_set_first0_1(&fb));
    }	

    return RET_SUCCESS;
}

