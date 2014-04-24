all:
	gcc -std=c99 -Wall -o u2048 u2048.c
debug:
	gcc -std=c99 -g -Wall -o u2048 u2048.c
clean:
	rm -rf *.out *.o *.dSYM u2048
