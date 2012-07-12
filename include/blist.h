/*
 * =====================================================================================
 *
 *       Filename:  linklist.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/08/2011 11:13:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef POMME_LIST_H
#define POMME_LIST_H
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
#define LIST_POSITION_1 ((void *) 0x00100100)
#define LIST_POSITION_2 ((void *) 0x00200200)
typedef struct link_head{
	struct link_head *prev,*next;
}link_t;

inline void link_add(link_t *ne, link_t *head);
inline void link_del(link_t *entry);

#define offset_of(TYPE,MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr,type,member) ({\
		const typeof( ((type *)0)->member) *__mptr = (ptr);\
		(type *)( (char *)__mptr - offset_of(type,member) );})

#define list_entry(ptr, type, member) container_of(ptr, type, member)

#define list_for_each_entry(pos, head,member) for ( pos = list_entry((head)->next, typeof(*pos), member);\
		&(pos->member)!=(head);\
		pos = list_entry(pos->member.next,typeof(*pos),member))

#define INIT_LINK(head,TYPE,MEMBER) ({\
		head->prev = (void *)offset_of(TYPE,MEMBER);\
		head->next = (void *)offset_of(TYPE,MEMBER);\
		})

inline void init_link(link_t *head);


#ifdef __cplusplus
}
#endif

#endif
