all: main.o funcs.o
	gcc -o shell_program main.o funcs.o

main.o: main.c funcs.h
	gcc -c main.c

funcs.o: funcs.c funcs.h
	gcc -c funcs.c

run:
	./shell_program

clean:
	rm *.o shell_program
