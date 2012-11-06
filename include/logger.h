/*
 * =====================================================================================
 *
 *       Filename:  pomme_log.h
 *
 *    Description:   
 *
 *        Version:  1.0
 *        Created:  11/08/2011 10:47:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  drealdal (zhumeiqi), meiqizhu@gmail.com
 *        Company:  Tsinghua University, HPC
 *
 * =====================================================================================
 */
#ifndef _UTIL_LOG_H
#define _UTIL_LOG_H

#include "header.h"
#include "queue.h"

#define LOG_DIR_DEFAULT "./"

#ifndef MAXLOGTRY
#define MAXLOGTRY 3
#endif

#define MAX_LOG_LENGTH 1024
#define MAX_LOG_FILE_LENGTH 100

typedef enum LOG_LEVEL{
	LOG_NULL,
	LOG_ERROR,
	LOG_WARNING,
	LOG_INFO,
	LOG_DEBUG,
	LOG_ALL
}log_level_t;

typedef struct log {

	char log_time[24];
	char message[MAX_LOG_LENGTH];

	struct logger *logger;
	queue_body_t next;

}log_t;

typedef struct logger{
	char *name;
	FILE *file_handle;	

	log_level_t log_level;// level < log_level will be logged
	struct queue_body next;//The logger should put into a queue ,to manage 

}logger_t;

void LOG(char *filename,int line,
		char *message,
		log_level_t level,
		struct logger *logger);
/*
 * register a logger
 */
struct logger * create_logger(
		log_level_t level, char *name);

int init_log();
int stop_logger();

#define LOG_ERROR(message,logger) do{\
	LOG(__FILE__,__LINE__,message,LOG_ERROR,logger);\
}while(0)

#define LOG_WARNING(message,logger) do{\
LOG(__FILE__,__LINE__,message,LOG_WARNING,logger)\
}while(0)

#define LOG_INFO(message,logger) do{\
	LOG(__FILE__,__LINE__,message,POMME_LOG_INFO,logger);\
}while(0)

#define LOG_DEBUG(message,logger) do{\
	LOG(__FILE__,__LINE__,message,POMME_LOG_DEBUG,logger);\
}while(0)


#endif
