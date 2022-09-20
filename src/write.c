#include <stdio.h>
#include <signal.h>
#include "ipc.h"

#define BUFSIZE 1024
#define SEMKEY 1000
#define SHMKEY 1001

int semid;
int shmid;
void *shmpt;

int quit = 0;

void siginthandler(int signo) {
  fprintf(stdout, "QUIT SIGNAL..\n");
  quit = 1;
}

int main(int argc, char *argv[]) {

  char buffer[BUFSIZE];
 
  fprintf(stdout, "SEM_KEY :%d\n", SEMKEY);
  fprintf(stdout, "SHM_KEY :%d\n", SHMKEY);

  createsem(SEMKEY, &semid);
  createshm(SHMKEY, &shmid, &shmpt, BUFSIZE);
  
  signal(SIGINT, siginthandler);

  while (!quit) {
    int cnt=0;
    char *ptr=buffer;
    
    fprintf(stdout, "> ");
    
    while ((*ptr++ = getchar()) != '\n')
      cnt++;
    *ptr='\0';

    writeshm(semid,shmpt, buffer, cnt+1);
  }
  deletesem(&semid);
  deleteshm(&shmid, shmpt);
  return 0;
}
