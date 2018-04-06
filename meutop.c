#define _POSIX_SOURCE
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "tinycthread.h"

/* This is the child thread function */
int topzera(void * aArg)
{
  (void)aArg;

  for (int i = 0; i < 10; ++i)
  {
    printf("Hello world %d!\n",i);
  }
  return 0;
}

char buf[32];
char *end;
int pid;
int res;

int sinalero(void * aArg)
{
  (void)aArg;

  while(1){
    printf(">");
    fgets(buf, sizeof buf, stdin);
    // buf[strlen(buf) - 1] = 0;
    pid = strtol(buf, &end, 10);  

    if (kill(pid,SIGKILL) != 0){
      fprintf(stderr,"%s\n",strerror(errno));
     } else {
      fprintf(stderr, "Process %d killed!\n", pid );
     }
  }
  return 0;
}


/* This is the main program (i.e. the main thread) */
int main()
{
  /* Start the child thread */
  thrd_t t, t2;

  // int saida;

  thrd_create(&t, topzera, (void*)0);
  thrd_join(t,NULL);

  thrd_create(&t2, sinalero, (void*)0);
  thrd_join(t2,NULL);
  

  return 0;
}