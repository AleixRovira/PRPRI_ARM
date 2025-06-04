#all: prpr

#Final code
operations.o: module/operations.c
	gcc -c module/operations.c -ggdb

#TDD
TEST_operations.o: TDD/TEST_operations.c
	gcc -c TDD/TEST_operations.c -ggdb

TEST_operations: operations.o TEST_operations.o
	gcc operations.o TEST_operations.o -o test.exe -ggdb
	rm *.o

#Clean
clean:
	rm -f *.o
	rm -f *.exe