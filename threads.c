/*
 * =====================================================================================
 *
 *       Filename:  threads.c
 *
 *    Description:  the implement of thread_pool
 *
 *        Version:  1.0
 *        Created:  03/19/2012 12:35:44 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "threads.h"
#include "queue.h"

static int start(tpool_t *ptp); 
static int stop(tpool_t *);

static int add_worker(tpool_t *, 
		worker_t *wr);
static void * thread_routine(void *worker);

static int add_thread(tpool_t *ptp);
static int remove_thread(tpool_t *ptp);

static int extend_pool(tpool_t *ptp, int n);

int tp_init(tpool_t *ptp,int num,
		int w_num,int cur_num)
{
	int ret = 0;

	assert( ptp != NULL);
	memset( ptp, 0, sizeof(tpool_t));

	ptp->max_thread_num = num;
	ptp->max_waiting = w_num;

	ptp->cur_thread_num = cur_num;
	ptp->tids = (pthread_t *)malloc(sizeof(pthread_t)*num);

	if(ptp->tids == NULL)
	{
		debug(ALL_DEBUG,"malloc err");
		ret = MEM_ERROR;
		goto err;
	}

	if ( ( ret = queue_init( &ptp->workers,
					"pomme thread pool",w_num)) < 0 )
	{
		debug(ALL_DEBUG,"queue init failure");
		goto queue_err;
	}

	if( ( ret = queue_init( &ptp->finished,
					"pomme thread finish", num)) < 0 )
	{
		debug(ALL_DEBUG,"queue init failure");
		goto finish_err;
	}


	ptp->add_worker = &add_worker;
	ptp->start = &start;

	ptp->stop = &stop;
	ptp->add_thread = &add_thread;

	ptp->remove_thread = &remove_thread;
	ptp->extend_pool = &extend_pool;

	ptp->thread_routine = &thread_routine;
	if( (ret =  sem_init(&ptp->sem,0,0) ) < 0)
	{
		debug(ALL_DEBUG,"sem init fail:%s",strerror(ret));
		goto sem_err;
	}
	ptp->inited = 1;

	return ret;
sem_err:
	queue_distroy(ptp->finished);
finish_err:
	queue_distroy(ptp->workers);
queue_err:
	free(ptp->tids); 
err:
	return ret;
}


static int start(tpool_t *ptp)
{
	if( ptp->inited != 1 )
	{
		debug(ALL_DEBUG,"Not init thread pool");
		return NULL_POINTER; 
	}
	int ret = 0;
	int i;
	for( i = 0; i < ptp->cur_thread_num; i++ )
	{
		rarg_t *arg = (rarg_t *)
			malloc(sizeof(rarg_t));
		arg->rank = i;
		arg->ptp = ptp;

		if( ( ret = pthread_create(ptp->tids+i, NULL,
						ptp->thread_routine, arg )) < 0 )
		{
			debug(ALL_DEBUG,
					"create thread %d fail %s",ret, strerror(ret));
			goto err;
		}
	}	

	for(; i < ptp->max_thread_num;i++)
	{
		thread_ids_t *ids =(thread_ids_t *) malloc(sizeof(thread_ids_t));	
		assert( ids != NULL );

		memset(ids, 0, sizeof(thread_ids_t));
		ids->rank = i;

		queue_push_back(ptp->finished,&ids->next); 
	}
err:
	return ret;
}

int tp_distroy(tpool_t *ptp)
{
	int ret = 0;
	assert( ptp != NULL );
	if( ptp->stoped != 1)
	{
		ptp->stop(ptp);
	}

	queue_distroy(ptp->workers);
	queue_distroy(ptp->finished);

	free(ptp->tids);

	ret = sem_destroy(&ptp->sem);

	memset(ptp, 0, sizeof(ptp));

	return ret;
}

static int add_worker(tpool_t *ptp, worker_t *wr)
{

	int ret = 0;

	assert( wr != NULL );
	memset(&wr->next, 0, sizeof(queue_body_t));

	if( (ret = queue_push_back( 
					ptp->workers, &wr->next) ) < 0 )
	{
		ret = THREAD_POOL_FULL;
		goto err;
	}
	sem_post(&ptp->sem);
err:
	return ret;
}

static void * thread_routine(void *arg)
{
	rarg_t *info = (rarg_t *)arg; 
	info->tid = pthread_self();

	tpool_t *ptp = info->ptp;
	while(1)
	{
		sem_wait(&ptp->sem);	

		queue_body_t *wr = queue_get_front(ptp->workers);
		worker_t *w = queue_entry(wr,worker_t,next);
		/* The sem make this assert true */
		assert( w!=NULL );
		if(w->process == NULL )
		{
			/* exit from the thread pool */
			ptp->tids[info->rank] = 0;
			thread_ids_t *ids = (thread_ids_t *)malloc(sizeof(thread_ids_t));

			assert( ids != NULL );

			memset(ids, 0, sizeof(thread_ids_t));
			ids->rank = info->rank;
			queue_push_back(ptp->finished,&ids->next); 

			free(w);

			break;
		}
		w->process(w->arg);	

		free(w->arg);
		free(w);
	}
	free(info);
	//TODO  use lock?
	return NULL; 
}
static int remove_thread(tpool_t *ptp)
{
	int ret = 0;
	worker_t *wr = (worker_t *) malloc(sizeof(worker_t));
	assert( wr != NULL );

	memset( wr, 0, sizeof(worker_t));
	ptp->add_worker(ptp, wr);
	/* only the one thread will do these things */
	ptp->cur_thread_num--;

	return ret;
}

static int add_thread(tpool_t *ptp)
{
	int ret = 0;
	if( ptp->cur_thread_num >= ptp->max_thread_num )
	{
		debug(ALL_DEBUG,"exceed max");
		return TOO_MANY_THREAD; 
	}
   	queue_body_t *id = queue_get_front( ptp->finished );
   	thread_ids_t *ids = queue_entry(id, thread_ids_t, next);

  	assert( ids != NULL );
   	rarg_t arg;
   	arg.rank = ids->rank; 

	if( ( ret = pthread_create(ptp->tids+ids->rank, 
					NULL, ptp->thread_routine, &arg )) < 0 ) 
	{
	   	debug(ALL_DEBUG,"create thread %d fail %s",ret, strerror(ret)); 
	}else{
	   	ptp->cur_thread_num++;
   	}

   	free(ids);

	return ret;
}

static int extend_pool(tpool_t *ptp, int n)
{
	int ret = 0,i;
	int num = n > 0 ? n : 0-n;
	for( i = 0 ; i < num ; i++ )
	{
		if(n>0)
		{
			ret = ptp->add_thread(ptp);
		}else{
			ret = ptp->remove_thread(ptp);
		}
		if( ret < 0 )
		{
			debug(ALL_DEBUG,"%s %d thread fail",
					n>0?"add":"remove",i);
			break;
		}
	}
	return ret;
}

static int stop(tpool_t *ptp)
{
	int ret = 0,i; 
	if(( ret = extend_pool(ptp, 
					0-ptp->cur_thread_num)) < 0 )
	{
		debug(ALL_DEBUG,"extend pool fail");
	}
	for( i = 0; i < ptp->max_thread_num ; i++ )
	{
		if( ptp->tids[i] != 0 )
		{
			pthread_join(ptp->tids[i],NULL);
		}
	} 
	ptp->stoped = 1;
	return 0; 
}
