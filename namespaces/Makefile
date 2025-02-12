.PHONY: main
main : main.c
	gcc -Wall main.c -o main.out -lc

.PHONY: clone_ns
clone_ns : clone_ns.c test_binary.c
	gcc -Wall test_binary.c -o test_binary.out
	gcc -Wall clone_ns.c -o clone_ns.out -lc

.PHONY: page_offset
page_offset : page_offset_limit.c
	gcc -Wall page_offset_limit.c -o page_offset.out -lc
