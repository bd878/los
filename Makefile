ls : ls.o lib.o
	gcc -o ls.out ls.o lib.o

ls.o : ls.c lib.h
	gcc -c ls.c

lib.o : lib.h lib.c
	gcc -c lib.c
