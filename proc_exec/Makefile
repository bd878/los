launch_prog.out : launch_prog.c print_envs_main.out
	gcc launch_prog.c -o launch_prog.out

print_envs_main.out : print_envs_main.c printenvs.c printenvs.o
	gcc print_envs_main.c -o print_envs_main.out printenvs.o

printenvs.o : printenvs.c printenvs.h
	gcc printenvs.c -c

