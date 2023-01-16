
/*======================= includes =======================*/
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

/*======================= defines =======================*/

#define REFRESH_RATE	1
#define RUNNING_TIME	5

/*===================== prototypes ======================*/
struct data_sys
{	
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

struct obj_type
{
   char *stringol;
   char name[10];
   float result;
   struct data_sys actual;
   struct data_sys prev;
};
/*================== function prototypes ==================*/

extern void * Read_function(void *arg);
extern void * Analyse_function(void *arg);
extern void * Print_function(void *arg);
void threads_init(void);

