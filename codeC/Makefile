all: main

main: main.o avl.o io.o
	gcc -o main main.o avl.o io.o

main.o: main.c
	gcc -Wall -Wextra -pedantic -std=c99 -c main.c -o main.o

avl.o: avl.c
	gcc -Wall -Wextra -pedantic -std=c99 -c avl.c -o avl.o

io.o: io.c
	gcc -Wall -Wextra -pedantic -std=c99 -c io.c -o io.o

clean:
	rm -f *.o main
