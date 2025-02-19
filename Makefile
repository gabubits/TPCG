all: main

run:
	./trab

main: trabalho.o
	gcc trabalho.o -o trab -lglut -lGLU -lGL -lm

trabalho.o:
	gcc -c trabalho.c -lglut -lGLU -lGL -lm

c:
	rm -f *.o trab trab.exe