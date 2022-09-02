file_chmod : file_chmod.o lib.o
	gcc -o file_chmod.out file_chmod.o lib.o

file_chmod.o : file_chmod.c lib.h
	gcc -c file_chmod.c

file_umask : file_umask.o lib.o
	gcc -o file_umask.out file_umask.o lib.o

file_umask.o : file_umask.c lib.h
	gcc -c file_umask.c

system_no_signal.o : system_no_signal.h system_no_signal.c
	gcc -c system_no_signal.c

ls : ls.o lib.o
	gcc -o ls.out ls.o lib.o

ls.o : ls.c lib.h
	gcc -c ls.c

lib.o : lib.h lib.c pr_exit.c
	gcc -Werror -Wextra -c lib.c pr_exit.c
