/*
* file:   threads.c
* author: Jacek Twardoch
* date:   13.01.2023 
*
* brief: This file contains initialize threads and threads functions 
*/



/*======================== includes ========================*/
#include "main.h"

/*====================== declarations ======================*/
static char* strtoke(char *str, const char *delim);
static void calc_function(struct obj_type * ptr);


/*==================== global variables ====================*/

int controller = 0;          //This variable is used to switching and synchronizing between threads functions
int core_counter = 0;        //This variable count number of cores in /proc/stat file 

struct obj_type * tablica_wsk[10] = {NULL};   //Contains pointers to objects (one core = one object)

extern int ENDING;

/*=================== threads functions ===================*/


/*
* description: Threads initialization
*
* details:  Create the threads and check that the threads have been created correctly
*
*  param[in/out]: none
*  return: none
*/ 
void threads_init(void)
{
  pthread_t thread_Reader, thread_Analyzer, Thread_Printer;

  if( pthread_create(&thread_Reader, NULL, Read_function, NULL) != 0 )
        printf("Failed to create thread_Reader\n");
  if( pthread_create(&thread_Analyzer, NULL, Analyse_function, NULL) != 0 )
        printf("Failed to create thread_Analyzer\n");
  if( pthread_create(&Thread_Printer, NULL, Print_function, NULL) != 0 )
        printf("Failed to create Thread_Printer\n");
}


/*
* description: 
*
* details: This function is executing in thread_1 
*
*  param[in/out]: none
*  return: none
*/ 

void * Read_function(void *arg)
{
   UNUSED(arg);
   	
   char buf[100] ;
   int len = 3;         // "cpu" 
   int string_counter;
   char working_buf[4] ={0};
   
   while(1)
   {
      /* waiting for enter to this thread */
      while(1)
      {
         if(controller == 0)
            break;
      }
        
      core_counter = 0;
        
      /* open the /proc/stat file */
      FILE * fp;
      fp = fopen("/proc/stat", "r");
  

      /* fetch line by line from file */
      while(fgets( buf, sizeof(buf), fp) != NULL)
      {
         
         memset(working_buf,0,sizeof(working_buf));
	 
	 string_counter = 0;
	   
	 /* copy first 3 letters("cpu") from lines to working buffer */
	 for(int iterator = 0; iterator < len; iterator++)
	 {	
	    *(working_buf+iterator) = *(buf+iterator);
	 }
	   
	 /* check if the line of file begin on cpu name */
	 if( !(strcmp(working_buf, "cpu")))
	 {
	     /* if yes, allocate memory and create cpu object */
	     string_counter = strlen(buf);
	     string_counter += 1;
	     tablica_wsk[core_counter] = (struct obj_type*) malloc( sizeof(struct obj_type));
	     tablica_wsk[core_counter]->file_line = NULL;
	     tablica_wsk[core_counter]->file_line = (char*) malloc( string_counter*sizeof(char) );
	     
	     /*copy string from buffer to memory which is pointing by object */   
	     strcpy(tablica_wsk[core_counter]->file_line, buf);

	     /* increase to the next object */
	     core_counter++;
	 }		
      }
     
        /* close the /proc/stat file */
	fclose(fp);


     /*switch to the next thread */  
     controller = 1;
   
     /*check ending condition */
     if(ENDING)
     {
      pthread_exit(NULL);
     }
   }
}


/*
* description: Analyse and prepare data to calculate cpu usage,
*              calculate CPU usage and write result to object 
*              structure
*
* details: This function is executing in thread_2
*
*  param[in/out]: none
*  return: none
*/ 

void * Analyse_function(void *arg)
{
   
   UNUSED(arg);						
   
   char * Parsepointer;
   int cstr =0;
   char parse_buf[100];
   
   while(1)
   {
      /* waiting for enter to this thread */
      while(1)
      { 
         if(controller == 1)
            break;
      }
      
      /* repeat below code for each core */
      for(int num = 0; num < core_counter; num++)
      {
         memset(parse_buf,0,sizeof(parse_buf));
         strcpy(parse_buf , tablica_wsk[num]->file_line);
         /* read and assign core name from string */
         Parsepointer = NULL;   	
         Parsepointer = strtoke(parse_buf, " ");
	 strcpy(tablica_wsk[num]->name, Parsepointer);
	 
	 cstr = 0;
	

	 while( cstr < 10 )
	 {		
		Parsepointer = strtoke(NULL, " ");
		if(*(Parsepointer) != '\0')
		{
			*(int*)(&tablica_wsk[num]->actual.user + cstr)= atoi(Parsepointer);
			cstr++;
		}
		else
		{
		//do nothing
		}  
	 }
		
         /*calculating cpu usage basic on data in the object structure */
         calc_function(tablica_wsk[num]);
     
         /*assign actual data to prev structure */
         for(int transfer = 0; transfer < 11 ; transfer++)
         {
            *(int*)(&tablica_wsk[num]->prev.user + transfer) = *(int*)(&tablica_wsk[num]->actual.user + transfer);
         }
        
      }
   /*switch to the next thread */
   controller = 2;
   
   /*check ending condition */
   if(ENDING)
      pthread_exit(NULL);
   }
 
}




/*
* description: Print CPU usage for each core
*
* details: This function is executing in thread_3 
*
*  param[in/out]: none
*  return: none
*/ 

 void * Print_function(void *arg)
 {
    UNUSED(arg);
    	
    while(1)
    {
       /* waiting for enter to this thread */
       while(1)
       {
          if(controller == 2)
             break;
       } 
       
     
    /* Print information for each core */
    printf("\n=============================================\n");
    for(int num = 0; num < core_counter; num++)
    {       
       printf("The %s usage percentage is equal to %f\n", tablica_wsk[num]->name, tablica_wsk[num]->result);  
       
        /* freeing allocated memory */ 
       //free(tablica_wsk[num]->file_line); 
       free(tablica_wsk[num]);
    }  
    printf("=============================================\n");     
    
    /* refresh rate */
    sleep(REFRESH_RATE);
    
    /*switch to the next thread */	
    controller = 0;
    
    /*check ending condition */
    if(ENDING)
       pthread_exit(NULL);
    }
 }


/*=================== helped functions ===================*/


/*
* description: Calculate usage, basic on the object data
*
* details: This function is called by thread_2 
*
*  param[in/out]: Pointer to the core object for which usage is to be calculated
*  return: none
*/ 
static void calc_function(struct obj_type * ptr)
{
   int PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total, totald, idled;
   PrevIdle=Idle=PrevNonIdle=NonIdle=PrevTotal=Total=totald=idled=0;
   
   PrevIdle = ptr->prev.idle - ptr->prev.iowait;
   Idle = ptr->actual.idle + ptr->actual.iowait;
  
   PrevNonIdle = ptr->prev.user + ptr->prev.nice + ptr->prev.system + ptr->prev.irq + ptr->prev.softirq + ptr->prev.steal;
   NonIdle = ptr->actual.user + ptr->actual.nice + ptr->actual.system + ptr->actual.irq + ptr->actual.softirq + ptr->actual.steal;
  
   PrevTotal = PrevIdle + PrevNonIdle;
   Total = Idle + NonIdle;
  
   totald = Total - PrevTotal;
   idled = Idle - PrevIdle;
  
   ptr->result = (float) (totald - idled)/totald;
}


/*
* description: Parse string and divide it on the tokens
*
* details: This function is an alternative for strtok;
*          compared to strtok, strtoke may return an empty string
*          Source: stackoverflow.com
*  param[in/out]: none
*  return: none
*/ 
static char* strtoke(char *str, const char *delim)
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







