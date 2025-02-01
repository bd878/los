#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>

/*
 * Outputs current process's cgroup.
 * Moves process in new cgroup.
 */

void free_buffer(char **buffer)
{
	printf("free buffer\n");
	free(*buffer);
}

void cleanup_file(FILE **fp)
{
	printf("close file\n");
	fclose(*fp);
}

int print_cgroup()
{
	size_t len = 80;
	ssize_t readn;
	char *buffer __attribute__((__cleanup__(free_buffer))) = malloc(len);
	FILE *fp __attribute__((__cleanup__(cleanup_file)));
	char pathname[20];

	if (sprintf(pathname, "/proc/%d/cgroup", getpid()) < 0)
		err(EXIT_FAILURE, "sprintf");

	fp = fopen(pathname, "r");
	if (fp == NULL) 
		err(EXIT_FAILURE, "fopen");

	readn = getline(&buffer, &len, fp);
	if (readn < 0)
		err(EXIT_FAILURE, "getline");
	printf("readn=%ld, len=%ld\n", readn, len);

	printf("%s\n", buffer);

	return 0;
}

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
		printf("children cgroup:\n");
		print_cgroup();
		(void) pthread_mutex_unlock(&mutex);

		printf("child waits\n");
		if (sem_wait(sem) < 0)
			err(EXIT_FAILURE, "sem_post");

		printf("child exits\n");
		return 1;
	}
	/* parent */

	printf("pid=%d, cid=%d\n", getpid(), cid);
	
	(void) pthread_mutex_lock(&mutex);
	printf("parent cgoup:\n");
	print_cgroup();
	(void) pthread_mutex_unlock(&mutex);

	printf("parent waits\n");
	waitpid(-1, NULL, 0);
	printf("parent exits\n");

	(void) pthread_mutex_destroy(&mutex);

	return 0;
}
