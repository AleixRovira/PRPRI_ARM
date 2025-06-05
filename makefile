#all: prpr

#Final code
operations.o: module/operations.c
	gcc -c module/operations.c -ggdb

main.o: main.c
	gcc -c main.c -ggdb

prod: main.o operations.o
	gcc main.o operations.o -o prpr.exe -ggdb -lm
	rm *.o

#TDD
TEST_operations.o: TDD/TEST_operations.c
	gcc -c TDD/TEST_operations.c -ggdb

TEST_operations: operations.o TEST_operations.o
	gcc operations.o TEST_operations.o -o test.exe -ggdb -lm
	rm *.o

test: TEST_operations

#Clean
clean:
	rm -f *.o

clean_all:
	rm -f *.o
	rm -f *.exe