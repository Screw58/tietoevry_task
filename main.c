/*
* file:   main.c
* author: Jacek Twardoch
* date:   12.01.2023 
*
* brief: This program reading, calculating and printing cpu usage information
*/


/*======================= includes =======================*/
#include "main.h"


/*=================== global variables ===================*/

int ENDING = 0;


/*==================== main function ====================*/

int main(void)
{

  printf("Before creating the threads\n");
  threads_init();

//	pthread_join(tid1,NULL);
//	pthread_join(tid2,NULL);
//	pthread_join(tid3,NULL);
	
  sleep(RUNNING_TIME);
  /* ending condition is set */
  ENDING = 1;
  /*wait for threads end work */
  sleep(2);
  
  exit(0);
}



