SUBDIRS = libcgutils-0.1

subdirs:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

create_cgroup.out : subdirs create_cgroup.c
	gcc -Wall create_cgroup.c -o create_cgroup.out -lpthread ./libcgutils-0.1/libcgutils.so

main_parse_cgroup.out : subdirs main_parse_cgroup.c compiler.h
	gcc -Wall -Wl,-rpath,./libcgutils-0.1/ main_parse_cgroup.c -o main_parse_cgroup.out ./libcgutils-0.1/libcgutils.so compiler.h

zombie.out : zombie.c
	gcc zombie.c -o  zombie.out -lcap

test_move.out : test_move.c
	gcc -Wall test_move.c -o test_move.out
