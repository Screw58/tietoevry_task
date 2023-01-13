
#include "main.h"



int ENDING = 0;


//==========================================================MAIN==============================================//

int main(void)
{
  
  
  int i = 0;

  printf("Before creating the threads\n");
  threads_init();

/*	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);*/
  sleep(5);
  ENDING = 1;
  sleep(2);
 exit(0);
}

//==========================================================HELPER_func==============================================//

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
