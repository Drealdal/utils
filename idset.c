/*
 * =====================================================================================
 *
 *       Filename:  idset.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/20/2012 10:00:57 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "idset.h"
#include "debug.h"
#define DEBUG_IDSET 1
int set_print(FILE *fd, int *set, int n)
{
#ifdef DEBUG
	int i = 0;
	for( i = 0; i < n ; i ++) 
	{
		fprintf(fd, "\t%d",set[i]);
	}
	fprintf(fd, "\n");
#endif
	return 0;
}

int bit_print(FILE *fd, unsigned char *bits, int n )
{
    int i ;
    for( i = 0 ; i < n ; i++)
    {
	if( (bits[i/8]&(1<<(i%8))) != 0 )
	{
	    fprintf(fd,"1");
	}
	else{
	    fprintf(fd,"0");
	}
	if(i %8 == 7) fprintf(fd, " ");
    }
    fprintf(fd,"\n");
    return 0;
}

static int idset_init_empty(int size, int **set)
{
    int i , ret = 0;
    assert( *set == NULL );
    *set = (int *)malloc((size+1)*sizeof(int));
    assert( *set != NULL );

    for( i = 0 ; i < size; i++ )
    {
	(*set)[i] = i;
    }
    (*set)[size] = -1;
    return ret;
} 
static int idset_init_full(int size, int **set)
{
    int i , ret = 0;
    assert( *set == NULL );
    *set = (int *)malloc((size+1)*sizeof(int));
    assert( *set != NULL );

    for( i = 0 ; i < size; i++ )
    {
	(*set)[i] = -1;
    }
    (*set)[size] = -1;
    return ret;
}
static int parrent(int i, int *set)
{
    while( i != set[i] && i != -1)
    {
	i = set[i]; 
    }	
    return i;
}
/*  pi-->pj */
static void combine(int i, int j , int *set)
{
    int pi, pj;
    pi = parrent(i, set);
    pj = parrent(j, set);
    set[i] = set[j];
    return;
}
static void take(int i , int *set)
{
    assert( i == set[i] );
    combine(i, i+1,set);
}
static int get_id(int *set, int size)
{
    int i = 0,ret;
    while( ( ret = parrent(i,set) )== -1 && i < size)
    {
	i++;
    }
    if( i == size ) return -1;
    take( ret , set);
    return ret;
}
int idsets_init(idsets_t *set, 
	long long begin, 
	long long end,
	int sizeperset)
{
    int ret = 0, i = 0;

    assert( NULL != set );
    set->begin = begin;
    set->end = end;
    set->sizeperset = sizeperset;

    set->cur_has_empty = 0;

    set->count = (end-begin)/sizeperset;
    if((end-begin)%sizeperset != 0 )
    {
	set->count ++;
    }
    int tln = set->count/8;
    if( set->count %8 == 0 )
	tln++;
    set->hasEmpty = (unsigned char *)malloc(tln);
    assert(set->hasEmpty != NULL );

    set->parrent = (int **)malloc(set->count*sizeof(int));

    assert(set->parrent != 0);

    for( i = 0; i < set->count; i ++ )
    {
	set->hasEmpty[i/8] |= 1 << i%8;
    }

    memset(set->parrent, 0, sizeof(int)*set->count);

    return ret;
}
int idsets_get(idsets_t *set)
{
    int ret = 0, i = 0;
    int pi = 0, pj = 0;

    //bit_print(stdout,set->hasEmpty,set->count);
    for( i = 0 ; i < set->count; i++ )
    {
	pi = i/8;
	pj = i%8;
	if( (set->hasEmpty[pi]&(1<<pj)) != 0 )
	{
	    break;
	}
    }

    if( i == set->count )
    {
	debug(ALL_DEBUG,"full idsets");
	return -1;
    }

    int size = set->sizeperset;
    if( NULL == set->parrent[i] )
    {
	if( (set->end - set->begin ) % set->sizeperset != 0 )
	{
	    size = (set->end - set->begin )%set->sizeperset;
	}
	idset_init_empty(size,&set->parrent[i]);
    }

    if( (ret = get_id(set->parrent[i],size)) == -1 )
    {
	set->hasEmpty[pi] &= ~(1<<pj);
	free(set->parrent[i]);
	set->parrent[i] = NULL;
	ret = idsets_get(set);
    }else{
	ret += i*set->sizeperset;
    }
    
    return ret;
}
int idsets_release( int id, idsets_t *set )
{
    int setRank,setId,size;
    if( id > set->end || id < set->begin)
    {
	debug(DEBUG_IDSET,"illegal id to release");
	return -1;
    }

    id = id - set->begin;
    setRank = id/set->sizeperset;
    setId = id%set->sizeperset;
    size = set->sizeperset;
    if( setRank == set->count - 1 && 
	   ( (set->end - set->begin)%set->sizeperset )!= 0)
    {
	size = (set->end - set->begin) %set->sizeperset; 
    }

    if( set->parrent[setRank] == NULL )
    {
	idset_init_full(size, &set->parrent[setRank]);
    }
    if( set->parrent[setRank][setId] == setId)
    {
	debug(DEBUG_IDSET,"Release untaken id");
	return -1;
    }
    set->parrent[setRank][setId] = setId;
    if(( set->hasEmpty[setRank/8] & 
		( 1 << setRank%8)) == 0 )
    {
	set->hasEmpty[setRank/8] |= 1<<setRank%8;
    }

    return 0;
}


