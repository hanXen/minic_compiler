all : mini_c

mini_c: mini_c.o
	gcc -g -o mini_c mini_c.o 

mini_c.o:
	gcc -g -c -o mini_c.o main.c 

clean:
	rm -f mini_c
	rm -f *.o

