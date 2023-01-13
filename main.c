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

int controller = 0;
int ENDING = 0;
struct data_sys actual = {0};
struct data_sys prev = {0};
struct data_sys * actual_ptr;
struct data_sys * prev_ptr;

int * yolopointer = &actual.user;
int * yolopointer2 = &prev.user;
//=============================================================================

char* strtoke(char *str, const char *delim)
{
  static char *start = NULL; /* stores string str for consecutive calls */
  char *token = NULL; /* found token */
  /* assign new start in case */
  if (str) start = str;
  /* check whether text to parse left */
  if (!start) return NULL;
  /* remember current start as found token */
  token = start;
  /* find next occurrence of delim */
  start = strpbrk(start, delim);
  /* replace delim with terminator and move start to follower */
  if (start) *start++ = '\0';
  /* done */
  return token;
}

void print(char *p)
{
  printf("%s",p);
}
//===================================================================================== THREAD 1 FUNC ============================================================//
void * threadMethod1(void *arg)
{
   int PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total, totald, idled;
   PrevIdle=Idle=PrevNonIdle=NonIdle=PrevTotal=Total=totald=idled=0;
   float CpuPercentage=0;
   while(1)
   {
     while(1)
     {
        if(controller == 3)
           break;
     }
   print("I am thread 1st\n");
 //  printf("\n user: %d\t nice: %d\t system: %d\t idle: %d\t iowait: %d\t irq: %d\t softirq: %d\t steal: %d\t guest: %d\t guest_nice: %d\n", actual.user, actual.nice, actual.system, actual.idle, actual.iowait, actual.irq, actual.softirq, actual.steal, actual.guest, actual.guest_nice);
  
   PrevIdle = prev.idle - prev.iowait;
   Idle = actual.idle + actual.iowait;
  
   PrevNonIdle = prev.user + prev.nice + prev.system + prev.irq + prev.softirq + prev.steal;
   NonIdle = actual.user + actual.nice +actual.system + actual.irq + actual.softirq + actual.steal;
  
   PrevTotal = PrevIdle + PrevNonIdle;
   Total = Idle + NonIdle;
  
   totald = Total - PrevTotal;
   idled = Idle - PrevIdle;
  
   printf("\ntotald: %d", totald);
   printf("\nidled: %d", idled);
   
   CpuPercentage = (float) (totald - idled)/totald;
  
   actual_ptr = &actual;
   prev_ptr = &prev;
  
   yolopointer = &actual.user;
   yolopointer2 = &prev.user;
  
  
//    printf("\n PREV:  user: %d\t nice: %d\t system: %d\t idle: %d\t iowait: %d\t irq: %d\t softirq: %d\t steal: %d\t guest: %d\t guest_nice: %d\n", prev.user, prev.nice, prev.system, prev.idle, prev.iowait, prev.irq, prev.softirq, prev.steal, prev.guest, prev.guest_nice);
    
   for(int transfer = 0; transfer < 11 ; transfer++)
   {
  	//*(prev_ptr+transfer) = *(actual_ptr + transfer);
  	*(yolopointer2+transfer) = *(yolopointer + transfer);
   }
  
   printf("=============================================\n");
   printf("The %s usage percentage is equal to %f\n", actual.name, CpuPercentage);
   printf("=============================================\n");
   
  
   controller = 1;
   if(ENDING)
   pthread_exit(NULL);
   }
}
//===================================================================================== THREAD 2 FUNC ============================================================//

void * threadMethod2(void *arg)
{
while(1)
{
  while(1)
  {
    if(controller == 1)
      break;
  }
  print("I am thread 2nd\n");
  sleep(1);
  controller = 0;
  if(ENDING)
   pthread_exit(NULL);
 }
}
//===================================================================================== THREAD 3 FUNC ============================================================//
 void * threadMethod3(void *arg)
 {
    char buf[100] ;
    char * Parsepointer;
    int cstr =0;
    while(1)
    {
       while(1)
       {
          if(controller == 0)
             break;
       } 
  
	FILE * fp;
	fp = fopen("/proc/stat", "r");
	
	fgets( buf, sizeof(buf), fp); 
	
	fclose(fp);
	
	Parsepointer = strtoke(buf, " ");

	strcpy(actual.name, Parsepointer);
	
	printf("pa to tera:%s\n", actual.name);
	
	cstr = 0;
	

	while( cstr < 10 )
	{
		
		Parsepointer = strtoke(NULL, " ");
		if(*(Parsepointer) != '\0')
		{
			*(yolopointer + cstr) = atoi(Parsepointer);
			//printf("%d\n", *(yolopointer + cstr));
			cstr++;
		}
		else
		{
		//do nothing
		}
	}
  print("I am thread 3rd\n");
  controller = 3;
  if(ENDING)
  pthread_exit(NULL);
}
}

//==========================================================MAIN==============================================//

int main(void)
{
  pthread_t thread_Reader, thread_Analyzer, Thread_Printer;
  
  int i = 0;

  printf("Before creating the threads\n");
  if( pthread_create(&thread_Reader, NULL, threadMethod1, NULL) != 0 )
        printf("Failed to create thread_Reader\n");
  if( pthread_create(&thread_Analyzer, NULL, threadMethod2, NULL) != 0 )
        printf("Failed to create thread_Analyzer\n");
   sleep(3);
  if( pthread_create(&Thread_Printer, NULL, threadMethod3, NULL) != 0 )
        printf("Failed to create Thread_Printer\n");

/*	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);*/
 sleep(5);
 ENDING = 1;
  sleep(2);
 exit(0);
}
