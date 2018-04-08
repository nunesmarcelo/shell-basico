#define _POSIX_SOURCE
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ncurses.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include "tinycthread.h"

/* This is the child thread function */
int topzera(void * aArg)
{
  (void)aArg;

  for (int i = 0; i < 20; ++i)
  {
    printw("Hello world %d!\n",i);
    refresh();
  }
  return 0;
}

int sinalero(void * aArg){
  (void)aArg;

  char buf[32];
  char *end;
  int pid;
  int res;


  while(1){

    move(20, 0);
    clrtoeol();
    mvprintw(20,0,">");
    // fgets(buf, sizeof buf, stdin);
    // buf[strlen(buf) - 1] = 0;
    getstr(buf);
    refresh();

    pid = strtol(buf, &end, 10);  

    if (kill(pid,SIGKILL) != 0){
      mvprintw(21,0,"%s\n",strerror(errno));
     } else {
      mvprintw(21,0, "Process %d killed!\n", pid );
     }

  }
}


/* This is the main program (i.e. the main thread) */
int main(){
  /* Start the child thread */
  thrd_t t, t2;

  // int saida;
  initscr ();
  curs_set (0);

  thrd_create(&t, topzera, (void*)0);
  thrd_join(t,NULL);

  thrd_create(&t2, sinalero, (void*)0);
  thrd_join(t2,NULL);
  endwin();

  return 0;
}