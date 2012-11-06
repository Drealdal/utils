/*
 * =====================================================================================
 *
 *       Filename:  logger.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/08/2011 10:59:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#include "logger.h"
#include "header.h"

static int stop_log = 0;

log_t * get_log_from_logger(
		struct logger *loger);

log_t * get_log(log_level_t level,
		struct logger *logger);

static void add_log(char *message,
		struct logger *logger);
static void distory_logger(
		struct logger *logger);
static void * write_log(void *);

int global_log_level = LOG_WARNING;

queue_t loggers;
queue_t log_buffer;

queue_t free_slots;
pthread_t log_thread;

void LOG(char *filename,
		int line,
		char *message,
		log_level_t level,
		struct logger *logger)
{
	if(level > global_log_level)
	{
		return;
	}

	if( logger == NULL )
	{
		debug(ALL_DEBUG,"NULL INPUT");
		return;
	}

	char local_mem[MAX_LOG_LENGTH];
	char *prefix = NULL;

	switch(level)
	{
		case LOG_ERROR:
			prefix = "ERROR:";
			break;
		case LOG_WARNING:
			prefix = "WARNING:";
			break;
		case LOG_INFO:
			prefix = "INFO:";
			break;
		case LOG_DEBUG:
			prefix = "DEBUG:";
			break;
		default:
			prefix = "NULL:";
			break;
	}
	snprintf(local_mem,MAX_LOG_LENGTH - 1,
			"%s %s %d %s", prefix, filename, 
			line, message);

	add_log(local_mem,logger);

	return;
}
static void add_log(char *message,
		struct logger *logger)
{
	if( logger == NULL)
	{
		debug(ALL_DEBUG,
				"ERROR:Malloc memeroy for message %s fail\n",
				message);
		return;
	}

	log_t *tadd = NULL;
	int tried = 0;

	while(tadd == NULL)
	{
		struct queue_body * body = 
			queue_get_front(&free_slots);	

		if(body !=NULL)
		{
			tadd = queue_entry(body,struct log,next);
			break;
		}

		tried++;
		if(tried >= MAXLOGTRY)
		{
			break;
		}	

	}
	if( tadd == NULL )
	{
		tadd =(log_t *) malloc(sizeof(log_t));
	}

	if( tadd == NULL )
	{
		debug(ALL_DEBUG,"ERROR:Malloc memeroy for message %s fail\n",message);
		return;
	}
    time_t t = time(NULL);
    
	struct tm *time_now = (struct tm*)localtime(&t);

	sprintf(tadd->log_time,"%d-%d-%d %d:%d:%d ",time_now->tm_year+1900,\
			time_now->tm_mon,\
			time_now->tm_mday,\
			time_now->tm_hour,\
			time_now->tm_min,\
			time_now->tm_sec);
	strcpy(tadd->message , message);

	tadd->logger = logger;

	queue_push_back(&log_buffer,&tadd->next);
	return;

}
/*-----------------------------------------------------------------------------
 * get a free log_t from logger struct
 * @loger 
 * @return , return a free slots or null if no available free slots  
 *-----------------------------------------------------------------------------*/
log_t * get_log_from_logger(struct logger *loger)
{
	return (log_t *)malloc(sizeof(log_t));
}

logger_t *create_logger(log_level_t level,
		char *filename)
{
	logger_t *global_logger = (logger_t *)
		malloc(sizeof(struct logger));

	if( global_logger == NULL)
	{
		return NULL;
	}	

	char *dirname = getenv("LOG_DIR");
	if( dirname == NULL )
	{
		dirname = LOG_DIR_DEFAULT;
	}

	if( filename== NULL )
	{
		debug(ALL_DEBUG,"%s %d: filename is null\n",
				__FILE__,__LINE__);

		filename = "log";
	}

    time_t t = time(NULL);
    
	struct tm *time_now = (struct tm*)localtime(&t);

	char file[MAX_LOG_FILE_LENGTH];
	snprintf(file,MAX_LOG_FILE_LENGTH - 1,
			"%s/%s-%d-%d-%d",dirname,filename,
			time_now->tm_year+1900,\
			time_now->tm_mon,time_now->tm_mday);

	memset(global_logger,0,sizeof(struct logger));

	global_logger->log_level = level;

	FILE * handle = fopen(file,"a+");
	if( NULL == handle )
	{
		debug(ALL_DEBUG,"open logfile failure");
#ifndef IGNORE_LOG_FILE_ERROR
		exit(-1);
#else
		global_logger->log_level = POMME_LOG_NULL; 
#endif
	}

	global_logger->file_handle = handle;
	global_logger->name = (char *)malloc(strlen(filename)+1);

	global_logger->next.next = (struct queue_body *)QUEUE_TAIL_NULL;

	if(global_logger->name == NULL)
	{
		fprintf(stderr,"Create logger fail\n");
		free(global_logger);

		return NULL;
	}

	memcpy(global_logger->name, 
			filename, strlen(filename));

	queue_push_back(&loggers,
		   	&global_logger->next);
	return global_logger;
}
static void distory_logger(struct logger *logger)
{

	if(logger == NULL) return;

	fclose(logger->file_handle);
	free(logger->name);
	free(logger);
}

int init_log()
{
	struct queue_head * buffer = &log_buffer;
	struct queue_head * logger = &loggers;
	struct queue_head * slots = &free_slots;

	queue_init(&buffer,"Log Buffer",1000000);
	queue_init(&slots,"Free Log slots",1000000);

	queue_init(&logger,"Loggers",100);
	pthread_create(&log_thread, NULL,&write_log,NULL);

	return 0;
}

int stop_logger()
{
	stop_log = 1;
	while(stop_log == 1 )
	{
		sleep(1);
	}
	return 0;
}
static void* write_log(void * argc)
{
	struct queue_head local_logs;  
	struct queue_head *buffer = &log_buffer;

	struct queue_head * logger = &loggers;
	struct queue_head *local =  &local_logs;

	struct queue_head * slots = &free_slots;
	queue_init(&local,"Log thread local",1000);

	struct queue_body * body = NULL;
	while( stop_log == 0 )
	{

		local_logs.head = (struct queue_body *) QUEUE_HEAD_NULL;
		local_logs.tail = (struct queue_body *) QUEUE_TAIL_NULL;

		while(is_empty_queue(buffer))
		{
			sleep(1);
			if(stop_log)break;
		}

		queue_cpy_del(buffer,local);

		while(!is_empty_queue(local))
		{
			body = queue_get_front(local);
			struct log * entry = queue_entry(body,struct log,next);

			fprintf(entry->logger->file_handle,
					"%s %s\n",entry->log_time,entry->message);

			body->next  = (struct queue_body *)QUEUE_TAIL_NULL;
			queue_push_back(slots,body);
		}

	}

	while(!is_empty_queue(logger))
	{
		body = queue_get_front(logger);
		logger_t * entry = queue_entry(body,struct logger,next);

		distory_logger(entry);
	}

	stop_log=0;
	return NULL;
}
