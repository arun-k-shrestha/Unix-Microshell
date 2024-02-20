myshell: myshell.o argparse.o builtin.o
	gcc -g -o myshell myshell.c argparse.c builtin.c

myshell.o: myshell.c argparse.h builtin.h
	gcc -g -c myshell.c

argparse.o: argparse.c argparse.h
	gcc -g -c argparse.c

builtin.o: builtin.c builtin.h
	gcc -g -c builtin.c

clean:
	rm *.o myshell



