#define _GNU_SOURCE
#include <err.h>
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <linux/nsfs.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/wait.h>

/**
 * Clones process; 
 * child unshares cgroup from parent,
 * parent "observes" child and
 * outputs its namespace
 */

int main()
{
	int cid;
	sem_t *sem;

	sem = sem_open("/sem", O_CREAT, 0770, 0);
	if (sem == SEM_FAILED)
		err(EXIT_FAILURE, "sem_open");

	if ((cid = fork()) < 0) {
		err(EXIT_FAILURE, "fork");
	} else if (cid == 0) {
		/* child */

		sem_t *sem2;
		sem2 = sem_open("/sem", O_CREAT);
		if (sem2 == SEM_FAILED)
			err(EXIT_FAILURE, "sem_open");

		if (unshare(CLONE_NEWUSER) < 0)
			err(EXIT_FAILURE, "unshare");

		int fd = open("/proc/self/ns/user", O_RDONLY);
		if (fd < 0)
			err(EXIT_FAILURE, "open");

		uid_t uid;
		ioctl(fd, NS_GET_OWNER_UID, &uid);
		if (uid < 0)
			err(EXIT_FAILURE, "ioctl");

		printf("uns uid=%d\n", uid);

		if (sem_post(sem2) < 0)
			err(EXIT_FAILURE, "sem_post");

		exit(EXIT_SUCCESS);
	}
	/* parent */

	printf("pid=%d, cid=%d\n", getpid(), cid);
	printf("parent waits\n");
	if (sem_wait(sem) < 0)
		err(EXIT_FAILURE, "sem_wait");

	waitpid(-1, NULL, 0);
	printf("parent exists\n");
	exit(EXIT_SUCCESS);
}
