echowrite.out : echowrite.c randstring.o
	gcc echowrite.c -o echowrite.out randstring.o

randstring.o : randstring.c
	gcc randstring.c -c

mkfile.out : mkfile.c
	gcc mkfile.c -o mkfile.out

clean :
	rm -rf *.out *.o *.txt
