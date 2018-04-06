#define _POSIX_SOURCE
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
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

char buf[32] = "\0";
char *end;
int pid;
int res;
struct stat sts;

int sinalero(void * aArg)
{
  (void)aArg;

  while(1){
    printf(">");
    
    scanf("%32[0-9]c\n", buf);
    // if(fgets(buf, sizeof buf, stdin) != NULL){ //Lê o comando do teclado
    //   pid = strtol(buf, &end, 10);
    // } else {
    //   fprintf(stderr, "fgets error: %s\n", strerror(errno) );
    // }



    char process[32] = "";
    strcpy(process, "/proc/");
    strcpy(process, buf);
    strcpy(process, "\0");

    // fprintf(stderr, "sai %s\n", process );
    // exit(0);
    // if (stat(process, &sts) == 0 && S_ISDIR(sts.st_mode)) {
  // process doesn't exist
//       fprintf(stderr, "sai\n" );
//       exit(0);
// }

    fprintf(stderr, "buffer: %s\n", process );
    if (buf == "/proc/")
    {
      /* code */
    }


    if (kill(pid,SIGKILL) != 0){
     fprintf(stderr,"asdfasdf %s\n",strerror(errno));
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