/**
 * detach_process.h - create new child,
 * that is not in calling process group,
 * i.e. its parent is init process
 */

/**
 * forks twicely,
 * detaches second children from proc group.
 * Receives pipe, where the proc send
 * its pid
 */
void detach(int*, void (*f)(int *));

/**
 * waits until second children starts and
 * write to fd
 */
void wait_child(int*, pid_t*);
