/**
 * detach_process.h - create new child,
 * that is not in calling process group,
 * i.e. its parent is init process
 */

/**
 * forks twicely,
 * detaches second children from proc group.
 * Returns direct child id
 */
int detach(int*, void (*f)(int *));

int wait_child(int);

