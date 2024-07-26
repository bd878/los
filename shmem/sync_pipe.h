/**
 * sync_pipe.h - synchronize parent
 * and child via pipe
 */

void
OPEN_PIPE(int*);

void
WAIT_PARENT(int*);

void
WAIT_CHILD(int*);

void
TELL_PARENT(int*);

void
TELL_CHILD(int*);
