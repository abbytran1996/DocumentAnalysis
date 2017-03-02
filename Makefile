# Makefile for analysis

test: analysis.o main.o unit_tests.o
	gcc -g -o test analysis.o main.o unit_tests.o

unit_tests.o: unit_tests.h unit_tests.c analysis.h
	gcc -g -c unit_tests.c

analysis.o: analysis.h analysis.c
	gcc -g -c analysis.c

main.o: analysis.h main.c unit_tests.h
	gcc -g -c main.c

clean:
	rm *.o analysis
