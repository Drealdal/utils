/*
 * =====================================================================================
 *
 *       Filename:  md5_test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/11/2012 05:07:00 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "debug.h"
#include <string.h>
#include "md5.h"
int main(int argc, char *argv[])
{
    int ret = 0;
    const  char *test1 = "The quick brown fox jumps over the lazy dog";
    const  char *md51 = "9e107d9d372bb6826bd81d3542a419d6";
    const  char *test2 = "The quick brown fox jumps over the lazy dog.";
    const  char  *md52 = "e4d909c290d0fb1ca068ffaddf22cbd0"; 
    const  char *empty= "";
    const  char *md5E = "d41d8cd98f00b204e9800998ecf8427e";

    uint8_t dig1[16],dig2[16],dig3[16];
    md5_hash(dig1,(const unsigned char *)test1,strlen(test1));
    md5_hash(dig2,(const unsigned char *)test2,strlen(test2));
    md5_hash(dig3,(const unsigned char *)empty,strlen(empty));
    fprintf(stdout,"Stardard:%s\n",md51);
    fprintf(stdout,"Calculte:");
    md5_print(stdout, dig1);
    fprintf(stdout,"\n");

    fprintf(stdout,"Stardard:%s\n",md52);
    fprintf(stdout,"Calculte:");
    md5_print(stdout, dig2);
    fprintf(stdout,"\n");


    fprintf(stdout,"Stardard:%s\n",md5E);
    fprintf(stdout,"Calculte:");
    md5_print(stdout, dig3);
    fprintf(stdout,"\n");

    return ret;
}
