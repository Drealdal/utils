/*
 * =====================================================================================
 *
 *       Filename:  pomme_threads.h
 *
 *    Description:  the description of the implements of a thread pool
 *
 *        Version:  1.0
 *        Created:  02/19/2012 05:48:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _POMME_THREADS_H
#define _POMME_THREADS_H
#include "header.h"
#include "queue.h"
#include "debug.h"
typedef struct worker
{
    void (*process)(void *arg);
    void *arg;
    queue_body_t next;
}worker_t;

typedef struct  pomme_thread_pool tpool_t;
struct pomme_thread_pool
{

    int inited; 
    int stoped;

    int max_thread_num;
    int cur_thread_num;
    
    sem_t sem;
    /* lock for global varible */
    pthread_mutex_t g_lock;
    /*The max waiting length of the queue     */

    int max_waiting;

    queue_t *workers;
    queue_t *finished;

    pthread_t *tids;
    int (*add_worker)(tpool_t *ptp,worker_t *wr);

    int (*start)(tpool_t *ptp);
    int (*stop)(tpool_t *ptp);

    /* if n > 0 the number of current thread will +n ,else - */
    int ( *extend_pool)(tpool_t *ptp,int n);

    int (* remove_thread)(tpool_t *ptp);
    int (* add_thread)(tpool_t *ptp);

    void * (*thread_routine)(void *);
};

typedef struct routine_arg
{
    int rank;
    pthread_t tid;
    tpool_t *ptp;
}rarg_t;

typedef struct thread_ids
{
    queue_body_t next;
    int rank;
}thread_ids_t;
/**
 * @brief tp_init : init an pthread pool
 * @param ptp
 * @num: max thread number
 * @max_waiting: the max waiting list
 * @cur_num: the current(init) thread num
 * @return <0 failure, == 0 success 
 */
int tp_init(tpool_t *ptp,
	int num,
	int max_waiting,
	int cur_num);
/**
 * @brief tp_distroy:distroy an thread pool
 * @param ptp
 * @return < 0 failure, == 0 success 
 */
int tp_distroy(tpool_t *ptp);

#endif
