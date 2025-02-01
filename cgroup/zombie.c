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
#include <sys/capability.h>

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
		cap_t caps;
		cap_value_t cap;

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

		caps = cap_get_proc();
		if (!caps)
			err(EXIT_FAILURE, "cap_get_proc");

		for (cap = 0; cap <= CAP_LAST_CAP; cap++) {
			cap_flag_value_t flag;

			printf("%s\n", cap_to_name(cap));

			if (cap_get_flag(caps, cap, CAP_PERMITTED, &flag) < 0)
				err(EXIT_FAILURE, "cap_get_flag");

			if (cap_set_flag(caps, CAP_EFFECTIVE, 1, &cap, flag) < 0)
				err(EXIT_FAILURE, "cap_set_flag");
		}

		if (cap_set_proc(caps) < 0)
			err(EXIT_FAILURE, "cap_set_flag");

		if (sem_wait(sem) < 0)
			err(EXIT_FAILURE, "sem_post");

		printf("child exited\n");
		exit(EXIT_SUCCESS);
	}
	/* parent */

	printf("pid=%d, cid=%d\n", getpid(), cid);
	printf("parent waits forever\n");

	waitpid(-1, NULL, 0);
	printf("parent exits\n");
	exit(EXIT_SUCCESS);
}
