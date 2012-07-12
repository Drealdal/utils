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
#ifndef POMME_QUEUE_H
#define POMME_QUEUE_H
#include "blist.h"
#include "debug.h"
#include <string.h>
#include <pthread.h>

#define QUEUE_HEAD_NULL ((void *)0x00100100)
#define QUEUE_TAIL_NULL ((void *)0x00100200)

#define is_head_null(queue) queue->head==QUEUE_HEAD_NULL
#define is_tail_null(queue) queue->tail==QUEUE_TAIL_NULL

typedef struct queue_head queue_t;
typedef struct queue_body queue_body_t;

typedef struct queue_head
{
	queue_body_t *head;
	queue_body_t *tail;
	/*
	 * matrix
	 */
	
	int maxLength;// the max allowed queue length, 
	int curLength;// the current queue length
	char *name;
	pthread_mutex_t mutex;
	/* whether the struct is malloc by self */
	int need_free; 

}queue_t;

typedef struct queue_body{
	struct queue_body *next;
}queue_body_t;

#define is_full_queue(queue) (queue->curLength >= queue->maxLength)
#define is_empty_queue(queue) (queue->curLength == 0)

#define lock_queue(queue) pthread_mutex_lock(&(queue->mutex))
#define unlock_queue(queue) pthread_mutex_unlock(&(queue->mutex))

#define queue_entry(ptr,type,member) list_entry(ptr,type,member)
/*
 * @queue , address of queue pointer ,
 *  if the queue is null ,The system will malloc for it.
 * @name  , will be copied to make sure that the
 * name will not be freed util the queue is free
 */
int queue_init(queue_t **queue,
	char *name,int maxLength);

int queue_distroy(queue_t *queue);

int queue_push_back(queue_t *queue, 
	queue_body_t *toadd);

queue_body_t * queue_get_front(queue_t *queue); 

int queue_cpy_del(queue_t *from , queue_t *to);

#endif
