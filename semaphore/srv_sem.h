/**
 * srv_sem.h - definitions for
 * semaphores
 */

void print_sem_stat(int);

void wait_sighup();

void sighup_handler(int);

void init_sem(int);

void cleanup_sem(int);


