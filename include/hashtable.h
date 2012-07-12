/*
 * =====================================================================================
 *
 *       Filename:  hash.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/13/2011 05:30:47 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _HASHTABLE_H
#define _HASHTABLE_H
/*
 * rIif the structure table this flag,We can use this mem with copy
 *
 */
#define HASH_NEED_FREE 1   //The hash_t is alloc by the sys
#include "blist.h"
#include "debug.h"
#include "hash.h"

typedef struct hash_node
{
	uint32_t key_len;
	uint32_t  value_len;
	void *key;
	void *value;
	pomme_link_t link;
}hash_node_t;

typedef int(* HASH_FUNC)(void *data, uint32_t);
typedef int(* HASH_CMP)(void *data,uint32_t);

typedef struct hashtable
{
	uint32_t size;// the init size of hash table
	uint32_t flags;//
	pomme_link_t *table;//where the data goes

	HASH_FUNC hash_func;
	HASH_CMP cmp_func;
}hashtable_t;

/*
 * if hash == NULL ,the init function will Malloc for it then init the data
 * init will malloc mem for table
 * h_func: the hash_function used by this hashtable
 * c_func: the cmpare function used by this hash table
 */
int pomme_hash_init(int size, int(*h_func)(void *,u_int32),int(*c_func)(void *,void *),hashtable_t **hash);

/*-----------------------------------------------------------------------------
 *  distory a hash structure will free all the data in the hash table,includeing
 *  data in pomme_data_t structure
 *-----------------------------------------------------------------------------*/
int pomme_hash_distroy(hashtable_t **hash);
/*
 * add a key/data pair into the hashTable
 */
int pomme_hash_put(hashtable_t *hash, pomme_data_t *key, pomme_data_t *data);
int pomme_hash_put_2(hashtable_t *hash, void *key, u_int32 key_len, void *value, u_int32 value_len);
/*
 * get the data of key from hash table
 * the application will repsonse for the mem for data
 */
int pomme_hash_get(hashtable_t *hash, pomme_data_t *key, pomme_data_t *data);
/*
 * del data from hash table ,return success if not exist
 */
int pomme_hash_del(hashtable_t *hash,pomme_data_t *key);

/*-----------------------------------------------------------------------------
 *  hash a data of length into int
 *-----------------------------------------------------------------------------*/
int str_hash(void *str,u_int32 str_len);

/*-----------------------------------------------------------------------------
 *  cmp to void * data , they are cmpared BYTE WISE
 *-----------------------------------------------------------------------------*/

/**
 * @brief pomme_hash_int_int : init an hash of <int, int> pair
 *
 * @param size
 * @param hash
 *
 * @return 
 */
int pomme_hash_int_int(int size, hashtable_t **hash);

int pomme_hash_int_longlong(int szie, hashtable_t **hash);
int pomme_hash_str_uint64(int size, hashtable_t **hash);

int cmp_int(void *a, void *b);
int hash_int(void *a,u_int32 len);
int cmp_longlong(void *a, void *b);

#endif
