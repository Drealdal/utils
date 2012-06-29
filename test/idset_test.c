/*
 * =====================================================================================
 *
 *       Filename:  idset_test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/20/2012 10:38:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "idset.h"

int main(int argc, char *argv[])
{
    int i;
    idsets_t idset;
    debug_setmask(ALL_DEBUG);

    idsets_init(&idset, 0,100,10);
    for( i = 0; i < 100 ; i ++)
    {
	fprintf(stdout, "%d\n",idsets_get(&idset));
    }
    for( i = 0; i < 10; i++ )
    {
	idsets_release(i*10+2,&idset);
    }
    for( i = 0; i < 10 ; i ++)
    {
	fprintf(stdout, "%d\n",idsets_get(&idset));
    }

    return 0;
}
