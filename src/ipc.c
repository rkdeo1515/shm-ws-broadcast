#include "ipc.h"

#include <stdio.h>

/* --- semaphore --- */
void createsem(int key, int *semid) {
  // Create semaphore
  if ((*semid = semget((key_t)key, 3, IPC_CREAT | 0666)) == -1) {
    printf("Create semaphore..\n");
    exit(1);
  }

  // set value of semaphore
  union semun semarg;
  semarg.val = 3;
  if (semctl(*semid, 0, SETVAL, semarg) == -1) {
    printf("Set value of semaphore failed..\n");
    exit(1);
  }
}
void deletesem(int *semid) {
  union semun sem_del;
  if (semctl(*semid, 0, IPC_RMID, sem_del) == -1) {
    printf("remove semaphore failed..\n");  // exit(1);
  }
}

int v(int semid) {
  struct sembuf pbuf;
  pbuf.sem_num = 0;
  pbuf.sem_op = 1;
  pbuf.sem_flg = SEM_UNDO;
  if (semop(semid, &pbuf, 1) == -1) {
    printf("semaphore V operation(get) failed..\n");
    exit(EXIT_FAILURE);
  }
  return (0);
}

int p(int semid) {
  struct sembuf pbuf;
  pbuf.sem_num = 0;
  pbuf.sem_op = -1;
  pbuf.sem_flg = SEM_UNDO;
  if (semop(semid, &pbuf, 1) == -1) {
    printf("semaphore P operation(release) failed..\n");
    exit(EXIT_FAILURE);
  }
  return (0);
}

/* --- shared memory -- */
void createshm(int key, int *shmid, void **shmpt, int size) {
  // Create shared memory
  if ((*shmid = shmget((key_t)key, size, IPC_CREAT | 0666)) == -1) {
    printf("Create shared memory failed..\n");
    exit(1);
  }
  // Attach shared memory
  if ((*shmpt = shmat(*shmid, (void *)0, 0)) == (void *)-1) {
    printf("Attach shared memory failed..\n");
    exit(1);
  }
}

void writeshm(int semid, char *shmpt, char *buf, int len) {
  p(semid);
  memcpy(shmpt, buf, len);
  v(semid);
}

void readshm(int semid, char *shmpt, char *buf, int len) {
  p(semid);
  memcpy(buf, shmpt, BUF_SIZE);
  v(semid);
}

void clearshm(int semid, char *shmpt, int len) {
  p(semid);
  memset(shmpt, 0, len);
  v(semid);
}

void deleteshm(int *shmid, void *shmpt) {
  // Clear shared memory
  // clearshm(*shmid, shmpt);

  // Detache shared memory
  if (shmdt(shmpt) == -1) {
    printf("Detach shared memory failed..\n");
    exit(1);
  }
  // Remove shared memory
  if (shmctl(*shmid, IPC_RMID, 0)) {
    printf("Remove shared memory failed");
    exit(1);
  }
}
