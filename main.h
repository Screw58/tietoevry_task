
/* ============================== includes ============================== */
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>


struct data_sys
{
	char name[10];
	int user;
	int nice;
	int system;
	int idle;
	int iowait;
	int irq;
	int softirq;
	int steal;
	int guest;
	int guest_nice;
};

/* ======================== function prototypes ======================== */

extern void * Read_function(void *arg);
extern void * Analyse_function(void *arg);
extern void * Print_function(void *arg);
void threads_init(void);
char* strtoke(char *str, const char *delim);
void calc_function(void);
