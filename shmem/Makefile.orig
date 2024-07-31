get_shmem.out : get_shmem.c
	gcc get_shmem.c -o get_shmem.out

data_seg_end.out : data_seg_end.c
	gcc data_seg_end.c -o data_seg_end.out

write_shmem.out : write_shmem.c sync_pipe.o detach_process.o
	gcc write_shmem.c -o write_shmem.out sync_pipe.o detach_process.o

sync_pipe.o : sync_pipe.h sync_pipe.c
	gcc sync_pipe.c -c

detach_process.o : detach_process.h detach_process.c
	gcc detach_process.c -c
