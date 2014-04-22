all:
	gcc -Wall -o u2048 u2048.c
debug:
	gcc -g -Wall -o u2048 u2048.c
clean:
	rm -rf *.out *.o *.dSYM u2048
