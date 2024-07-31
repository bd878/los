/**
 * detached.h - detached process definition 
 */

void detached_proc(int *);

void rm_shmem(int);

void rm_sem(int);

int get_or_create_shmem(int);

int get_or_create_sem(int);
