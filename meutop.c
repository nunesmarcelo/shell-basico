#define _POSIX_SOURCE
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ncurses.h>
#include <dirent.h>
#include <pwd.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include "tinycthread.h"
#include "my_input.h"


thrd_t t, t2;
int x,y;

int sinalero(void * aArg){
  (void)aArg;

  char buf[32];
  char *end;
  int pid;

 while(1){

    move(22,0);
    clrtoeol();
    mvprintw(22,0,">");
    move(22,1);

    getstr(buf);
    refresh();

    pid = strtol(buf, &end, 10);  

    if (kill(pid,SIGKILL) != 0){
      mvprintw(23,0,"%s\n",strerror(errno));
     } else {
      mvprintw(23,0, "Process %d killed!\n", pid );
     }
    refresh();
 }
}


int topzera(void * aArg)
{
  (void)aArg;

  DIR *d;
  struct dirent *dir;
  FILE *fp;
  char diretorio[100];
  struct passwd *pwd;
      


  curs_set (0);

  while(1){
    d = opendir("/proc/");

    if(d){
      int i=0;

      mvprintw(0,0,"PID    | User    |    PROCNAME    | Estado |\n");
      mvprintw(1,0,"-------|---------|----------------|--------|\n");
      //ABRIR OS DIRETÓRIOS DENTRO DE PROC   
      while ((dir = readdir(d)) != NULL && i<20) {

        //DESCONSIDERE O PROC/. E PROC/..
        if(strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0 ){
          continue;
        }

        
        
        //FORMATA A STRING PARA " /PROC/NOME_DO_PROCESSO/STATUS " PARA QUE O FP ABRA ESSE ARQUIVO.
        strcpy(diretorio,"/proc/");
        strcat(diretorio,dir->d_name);
        strcat(diretorio,"/status");

        //ABRE O DIRETÓRIO COM A STRING FORMATADA PARA LEITURA
        fp = fopen(diretorio,"r");

        //SE ELE TIVER PEGO UMA PASTA QUE NÃO É UM PROCESSO
        if (fp == NULL)
        {
          //SE NÃO CONSEGUIR ABRIR, É PORQ ESTÁ NAS OUTRAS PASTAS DO DIRETORIO (FP,SWAPS,STATS) 
          //E QUEREMOS ABRIR PROCESSOS,ENTÃO PASSE DIRETO.
          continue;
        }

        //RECEBE O QUE PRECISAMOS DO ARQUIVO, "NAOUSAREMOS" É UMA VARIAVEL QUE APENAS PULA O PONTEIRO NO 
        //ARQUIVO, E AS OUTRAS SÃO AS VARIÁVEIS QUE QUEREMOS USAR.
        char naoUsaremos[50],procnome[50],uid[50],pid[50],state[50];
          fscanf(fp,"%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                 naoUsaremos,procnome,
                 naoUsaremos,state,
                 naoUsaremos,naoUsaremos,naoUsaremos,
                 naoUsaremos,naoUsaremos,
                 naoUsaremos,naoUsaremos,
                 naoUsaremos,pid,
                 naoUsaremos,naoUsaremos,
                 naoUsaremos,naoUsaremos,
                 naoUsaremos,naoUsaremos,
                 uid

                 );
          
          fclose(fp);
          //formatação dos espaços para montar a tabela:
          //      "  PID    | User    | PROCNAME | Estado |  "
          //É O PADRÃO DE IMPRESSÃO, ENTÃO PREENCHEMOS COM ESPAÇOS.
          //IMPRIME ID DO PROCESSO
          mvprintw(i+2,0,"%-7s",pid);

          //Achar o dono do processo através do UID
          //int uid2 = atoi(uid);
          uid_t uid3 = (uid_t)atoi(uid);
          pwd = getpwuid(uid3);

          //IMPRIME DONO DO PROCESSO
          mvprintw(i+2,7,"|%-9s",pwd->pw_name);

          if(strcmp(pwd->pw_name,"root") == 0){
            continue;
          }

          //IMPRIME NOME DO PROCESSO
          mvprintw(i+2,17,"|%-16s",procnome);

          //IMPRIME STATE DO PROCESSO
          mvprintw(i+2,34,"|%-8s",state);
          mvprintw(i+2,43,"|");
          refresh();


        //INCREMENTA I PARA CONTAR ATE 20.
        i++;
      }
      
      sleep(5);
    }
    closedir(d);
  }
}

int main(){

  initscr();
  cbreak();
  curs_set (0);

  thrd_create(&t, topzera, (void*)0);
  thrd_create(&t2, sinalero, (void*)0);

  thrd_join(t2,NULL);
  thrd_join(t,NULL);

  endwin();
  refresh();

  return 0;
}