#ifndef __IPC_H__
#define __IPC_H__

//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define BUF_SIZE 128
#define WHOLE_BUF_SIZE 1024
#define _POSIX_SOURCE 1
#define FAILED -1

#define STRLEN 32
#define CFGLEN 3

#if (defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)) || \
    defined(__FreeBSD__) || defined(__APPLE__)
/* union semun is defined by including <sys/sem.h> */
#else
union semun {
  int val;
  struct sem_id_ds *buf;
  unsigned short *array;
};
#endif

/* --- semaphore --- */
void createsem(int key, int *semid);
void deletesem(int *semid);
// int v(int semid);
// int p(int semid);

/* --- shared memory -- */
void createshm(int key, int *shmid, void **shmpt, int size);
void writeshm(int semid, char *shmpt, char *buf, int len);
void readshm(int semid, char *shmpt, char *buf, int len);
void clearshm(int semid, char *shmpt, int len);
void deleteshm(int *shmid, void *shmpt);

#endif