#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>

#include "cgroup_utils.h"

/*
 * Outputs current process's cgroup.
 * Moves process in new cgroup.
 */

int main()
{
	sem_t *sem;
	int cid;

	pthread_mutexattr_t attr; 
	pthread_mutex_t mutex;

	(void) pthread_mutexattr_init(&attr);
	(void) pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	(void) pthread_mutex_init(&mutex, &attr);
	(void) pthread_mutexattr_destroy(&attr);

	sem = sem_open("/cgroup_sem", O_CREAT, 0770, 0);
	if (sem == SEM_FAILED)
		err(EXIT_FAILURE, "sem_open");

	if ((cid = fork()) < 0) {
		err(EXIT_FAILURE, "fork");
	} else if (cid == 0) {
		/* child */
		(void) pthread_mutex_lock(&mutex);
		printf("children cgroup: %s\n", read_self_cgroup());
		(void) pthread_mutex_unlock(&mutex);

		printf("child create cgroup \"test\"\n");
		(void) mkdir("/sys/fs/cgroup/", 0770);		

		printf("child waits\n");
		if (sem_wait(sem) < 0)
			err(EXIT_FAILURE, "sem_post");

		printf("child exits\n");
		return 1;
	}
	/* parent */

	printf("pid=%d, cid=%d\n", getpid(), cid);
	
	(void) pthread_mutex_lock(&mutex);
	printf("parent cgoup: %s\n", read_self_cgroup());
	(void) pthread_mutex_unlock(&mutex);

	printf("parent waits\n");
	waitpid(-1, NULL, 0);
	printf("parent exits\n");

	(void) pthread_mutex_destroy(&mutex);

	return 0;
}
