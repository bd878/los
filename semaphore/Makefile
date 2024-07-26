srv_sem_main.out : srv_sem_main.c srv_sem.c
	gcc srv_sem_main.c -o srv_sem_main.out srv_sem.o

sem_proc.out : sem_proc.c srv_sem.o ../shmem/sync_pipe.o
	gcc sem_proc.c -o sem_proc.out srv_sem.o ../shmem/sync_pipe.o

srv_sem.o : srv_sem.c
	gcc srv_sem.c -c


