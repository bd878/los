#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

/* Clones process,
 * makes child a zombie intentionally
 * just to see what will happen
 * with corresponding cgroup
 */

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main()
{
	pid_t cid;
	sem_t *sem;

	sem = sem_open("/sem", O_CREAT, 0770, 0);
	if (sem == SEM_FAILED)
		handle_error("sem_open");

	if ((cid = fork()) == 0) {
		/* child */
		int val;

		sem_t *csem;
		csem = sem_open("/sem", O_CREAT);
		if (csem == SEM_FAILED)
			handle_error("sem_open"); 

		if (sem_post(csem) < 0)
			handle_error("sem_post");
		if (sem_getvalue(csem, &val) < 0)
			handle_error("sem_getvalue");
		printf("child: sem value: %d\n", val);
		printf("child exits\n");
		exit(EXIT_SUCCESS);
	} else if (cid < 0) {
		handle_error("fork");
	}

	int val;

	/* parent */
	printf("pid=%d, cid=%d\n", getpid(), cid);
	if (sem_getvalue(sem, &val) < 0)
		handle_error("sem_getvalue");
	printf("parent: sem value: %d\n", val);
	if (sem_wait(sem) < 0)
		handle_error("sem_wait");

	printf("parent blocked\n");
	if (sem_wait(sem) < 0)
		handle_error("sem_wait");
	exit(EXIT_SUCCESS);
}
