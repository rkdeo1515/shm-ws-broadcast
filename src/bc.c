// gcc -o brdg brdg.c ipc.c -I /usr/local/include/wsserver -L /usr/local/lib/ -lws
#include <stdio.h>
#include <signal.h>
#include "ipc.h"
#include "ws.h"

#define BUFSIZE 1024
#define SEMKEY 1000
#define SHMKEY 1001
#define WSPORT 3030

int semid;
int shmid;
void *shmpt;

int quit = 0;

int wsid=0;
ws_cli_conn_t *wscon[10];

void siginthandler(int signo) {
  fprintf(stdout, "QUIT SIGNAL..\n");
  quit = 1;
}

void onopenCB(ws_cli_conn_t *client)
{
    wscon[wsid++] = client;
    char *cli;
    cli = ws_getaddress(client);
    printf("Connection opened, addr: %s, id: %d\n", cli, wsid);
}

void oncloseCB(ws_cli_conn_t *client)
{
    int i;
    char *cli;
    cli = ws_getaddress(client);
    printf("Connection closed, addr: %s\n", cli);
    for(i=0;i<=wsid;i++)
    if(wscon[i]==client)
      wscon[i]=0;
}

void onmessageCB(ws_cli_conn_t *client,
    const unsigned char *msg, uint64_t size, int type)
{
  int i=0;
  for(i=0;i<=wsid;i++)
    if(wscon[i]!=client)
      ws_sendframe(wscon[i], msg, size, type);
}

int main(int argc, char *argv[]) {

  int i;
  char buffer[BUFSIZE];
 
  fprintf(stdout, "SEM_KEY :%d\n", SEMKEY);
  fprintf(stdout, "SHM_KEY :%d\n", SHMKEY);

  createsem(SEMKEY, &semid);
  createshm(SHMKEY, &shmid, &shmpt, BUFSIZE);
  
  clearshm(semid,shmpt, BUFSIZE);
  memset(buffer,0,BUFSIZE);

  struct ws_events evs;
  evs.onopen    = &onopenCB;
  evs.onclose   = &oncloseCB;
  evs.onmessage = &onmessageCB;

  ws_socket(&evs, WSPORT, 1, 1000);

  signal(SIGINT, siginthandler);

  while (!quit) {
    readshm(semid,shmpt, buffer, BUFSIZE);
  
    if(buffer[0]){
      fprintf(stdout, "SHM:%s", buffer);
      if(wsid)
        for(i=0;i<=wsid;i++)
          if(wscon[i])
            ws_sendframe_txt(wscon[i], buffer);

      clearshm(semid, shmpt, BUFSIZE);
    }
      
  }
  deletesem(&semid);
  deleteshm(&shmid, shmpt);
  return 0;
}
