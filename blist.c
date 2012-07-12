
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
#include "blist.h"

static inline void __link_add(link_t *ne, 
	link_t *prev,link_t *next)
{
	prev->next = ne;
	ne->prev = prev;
	ne->next = next;
	next->prev = ne;

}

inline void link_add(link_t *ne, link_t *head)
{
	__link_add(ne,head,head->next);
}

static inline void __link_del(link_t *prev, link_t *next)
{
	prev->next = next;
	next->prev = prev;
}

inline void link_del(link_t *entry)
{
	__link_del(entry->prev, entry->next);
	entry->prev =(link_t *) LIST_POSITION_1;
	entry->next = (link_t *)LIST_POSITION_2;
}

inline void init_link(link_t *head)
{
	if( head == NULL)
	{
		return;
	}

	head->prev = head;
	head->next = head;
}
