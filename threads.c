
#include "main.h"



float CpuPercentage=0;
int controller = 0;

extern int ENDING;
struct data_sys actual = {0};
struct data_sys prev = {0};

struct data_sys * actual_ptr = NULL;
struct data_sys * prev_ptr = NULL;

int * yolopointer = &actual.user;
int * yolopointer2 = &prev.user;


void threads_init(void)
{
  pthread_t thread_Reader, thread_Analyzer, Thread_Printer;

  if( pthread_create(&thread_Reader, NULL, Read_function, NULL) != 0 )
        printf("Failed to create thread_Reader\n");
  if( pthread_create(&thread_Analyzer, NULL, Analyse_function, NULL) != 0 )
        printf("Failed to create thread_Analyzer\n");
   sleep(3);
  if( pthread_create(&Thread_Printer, NULL, Print_function, NULL) != 0 )
        printf("Failed to create Thread_Printer\n");
}

//===================================================================================== THREAD 1 FUNC ============================================================//

void * Read_function(void *arg)
{

//   float CpuPercentage=0;
   while(1)
   {
     while(1)
     {
        if(controller == 3)
           break;
     }


   calc_function();
     
   //actual_ptr = &actual;
   //prev_ptr = &prev;
  
   //yolopointer = &actual.user;
   //yolopointer2 = &prev.user;
  

    
   for(int transfer = 0; transfer < 11 ; transfer++)
   {
  	*(int *)(&prev.user + transfer) = *(int *)(&actual.user + transfer);
  	//*(yolopointer2+transfer) = *(yolopointer + transfer);
   }
  
  
   controller = 1;
   if(ENDING)
   pthread_exit(NULL);
   }
}
//===================================================================================== THREAD 2 FUNC ============================================================//

void * Analyse_function(void *arg)
{
while(1)
{
  while(1)
  {
    if(controller == 1)
      break;
  }
   printf("\n=============================================\n");
   printf("The %s usage percentage is equal to %f\n", actual.name, CpuPercentage);
   printf("=============================================\n");
  sleep(1);
  controller = 0;
  if(ENDING)
   pthread_exit(NULL);
 }
}
//===================================================================================== THREAD 3 FUNC ============================================================//
 void * Print_function(void *arg)
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
  controller = 3;
  if(ENDING)
  pthread_exit(NULL);
}
}







void calc_function(void)
{
   int PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total, totald, idled;
   PrevIdle=Idle=PrevNonIdle=NonIdle=PrevTotal=Total=totald=idled=0;
   
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
}










