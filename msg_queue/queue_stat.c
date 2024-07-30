/**
 * queue_stat.c - stat message queue
 */
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

struct msgbuf {
  long mtype;
  char mtext[80];
};

void show_queue_stat(int);
void rm_queue(int);
void rm_shm(int);

int main() {
  int me, qid, cid, shmid, psize;
  size_t msgsize;
  struct msgbuf msg;
  char* textstr;

  me = getpid();
  psize = getpagesize();

  if ((shmid = shmget(IPC_PRIVATE, psize*2, IPC_CREAT | IPC_EXCL | 0666)) < 0){ 
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  if ((qid = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0666)) < 0) {
    perror("msgget");
    exit(EXIT_FAILURE);
  }

  //show_queue_stat(qid);

  if ((cid = fork()) < 0) {
    rm_queue(qid);
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (cid == 0) { /* child */
    struct msgbuf msg2;
    me = getpid();

    printf("[%d] waiting for message\n", me);
    if ((msgsize = msgrcv(qid, &msg2, sizeof(msg2.mtext), 1, 0)) < 0) {
      perror("msgrcv");
      exit(EXIT_FAILURE);
    }

    printf("[%d] child msgsize: %d, shmid: %s\n", me, msgsize, msg2.mtext);
    exit(EXIT_SUCCESS);
  }

  printf("[%d] parent shmid: %d\n", me, shmid);

  msg.mtype = 1;
  snprintf(msg.mtext, sizeof(msg.mtext), "%d", shmid);

  if (msgsnd(qid, &msg, sizeof(msg.mtext), 0) < 0) {
    rm_queue(qid);
    rm_shm(shmid);
    perror("msgsnd");
    exit(EXIT_FAILURE);
  }

  printf("[%d] waiting child exit: %d\n", me, cid);
  if (waitpid(cid, NULL, 0) < 0) {
    rm_queue(qid);
    rm_shm(shmid);
    perror("waitpid");
    exit(EXIT_FAILURE);
  }

  printf("[%d] child exited: %d\n", me, cid);

  rm_queue(qid);
  rm_shm(shmid);

  return 0;
}

void show_queue_stat(int qid) {
  struct msqid_ds buf;

  if (msgctl(qid, IPC_STAT, &buf) < 0) {
    rm_queue(qid);
    perror("msgctl");
    exit(EXIT_FAILURE);
  }

  printf("permission mode: %d\n", buf.msg_perm.mode);
  printf("send time: %s", ctime((time_t *)(&buf.msg_stime)));
  printf("create time: %s", ctime((time_t *)(&buf.msg_ctime)));
  printf("receive time: %s", ctime((time_t *)(&buf.msg_rtime)));
  printf("bytes in a queue: %ld\n", buf.msg_cbytes);
  printf("messages in a queue: %ld\n", buf.msg_qnum);
  printf("max bytes in a queue: %ld\n", buf.msg_qbytes);
}

void rm_queue(int qid) {
  int e = errno;
  msgctl(qid, IPC_RMID, NULL);
  if (errno > 0)
    perror("msgctl");
  errno = e;
}

void rm_shm(int shmid) {
  int e = errno;
  shmctl(shmid, IPC_RMID, NULL);
  if (errno > 0)
    perror("shmctl");
  errno = e;
}

