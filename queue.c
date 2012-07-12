/*
 * =====================================================================================
 *
 *       Filename:  pomme_queque.h
 *
 *    Description:  the implement of queue 
 *
 *        Version:  1.0
 *        Created:  11/15/2011 05:26:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "queue.h"

int queue_init(queue_t **queue,
	char *name,int maxLength)
{
    queue_t * q_addr = *queue;

    if(q_addr == NULL)
    {
	q_addr =(queue_t *) malloc(sizeof(queue_t));

	if(q_addr == NULL)
	{
	    debug(DEBUG_UTILS,
		    "Malloc Error for queue: %s",name);
	    return -1;
	}

	memset(q_addr,0,sizeof(struct queue_head));
	q_addr->need_free = 1;

    }else{
	memset(q_addr,0,sizeof(struct queue_head));
    }

    q_addr->head = (queue_body_t *) QUEUE_HEAD_NULL;
    q_addr->tail = (queue_body_t *) QUEUE_TAIL_NULL;

    q_addr->maxLength = maxLength;
    int nLen = strlen(name);
    q_addr->name = (char *)malloc(nLen);

    if( q_addr->name == NULL )
    {
	debug(DEBUG_UTILS,
		"Malloc Error for queue:%s",name);
	return -1;
    }

    memcpy(q_addr->name,name,nLen);

    if( pthread_mutex_init(&q_addr->mutex,NULL) != 0 )
    {
	debug(DEBUG_UTILS,
		"Queue %s init Mutex Error\n",name);
	return -1;
    }

    q_addr->curLength = 0;
    *queue = q_addr;

    return 0;
}

int queue_distroy(queue_t *queue)
{
    int ret = 0;
    lock_queue(queue);

    if( !is_empty_queue(queue) )
    {
	debug(DEBUG_UTILS,
		"Distroy not empty queue:%s",queue->name);
	ret = -1;
	goto d_end;
    }

    free(queue->name);

    if( queue->need_free == 1 )
    {
	unlock_queue(queue);
	free(queue);
	return 0;
    }
    queue->tail =(queue_body_t *) QUEUE_TAIL_NULL;
    queue->head =(queue_body_t *) QUEUE_HEAD_NULL;
    queue->maxLength = 0;
d_end:
    unlock_queue(queue);
    return ret;

}

static int __add_queue(queue_t *queue,
	queue_body_t *toadd)
{
    int ret = 0;
    lock_queue(queue);

    if(is_full_queue(queue))
    {
	debug(DEBUG_UTILS,
		"Add to full queue");
	ret = -1;
	goto unl;
    }

    toadd->next = (queue_body_t *)QUEUE_TAIL_NULL;
    if( is_tail_null(queue) )
    {
	queue->tail = toadd;
	queue->head = toadd;

	queue->curLength = 1;
	ret = 0;
	goto unl;
    }

    queue->tail->next = toadd;
    queue->tail = toadd;
    queue->curLength++;

unl:
    unlock_queue(queue);

    return ret;
}
static queue_body_t * __del_queue(queue_head *queue)
{
    struct queue_body * re = NULL;
    lock_queue(queue);
    if( is_empty_queue(queue) )
    {
	debug(DEBUG_UTILS,
		"Queue:%s is empty",queue->name);
	goto out_pos;
    }

    re = queue->head;
    queue->head = queue->head->next;

    if(queue->head == QUEUE_TAIL_NULL)
    {
	queue->tail =(queue_body_t *) QUEUE_TAIL_NULL;
	queue->head =(queue_body_t *)  QUEUE_HEAD_NULL;
    }

    queue->curLength--;
out_pos:
    unlock_queue(queue);
    return re;
}
int queue_push_back(struct queue_head *queue,struct queue_body *toadd)
{
    return __add_queue(queue,toadd);
}
struct  queue_body * queue_get_front(struct queue_head *queue)
{
    return __del_queue(queue);
}
int queue_cpy_del(struct queue_head *from, struct queue_head *to)
{
    lock_queue(from);
    lock_queue(to);

    to->head = from->head;
    to->tail = from->tail;
    from->head = (queue_body_t *) QUEUE_HEAD_NULL;

    from->tail = (queue_body_t *) QUEUE_TAIL_NULL;
    to->curLength = from->curLength;
    from->curLength = 0;

    unlock_queue(to);
    unlock_queue(from);
    return 0;
}
