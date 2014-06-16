all:
	gcc -std=c99 -Wall -o c2048 2048.c
debug:
	gcc -std=c99 -g -Wall -o c2048 2048.c
clean:
	rm -rf *.out *.o *.dSYM c2048
