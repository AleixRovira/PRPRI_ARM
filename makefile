#all: prpr

#Final code
global.o: module/global.c
	gcc -c module/global.c -ggdb

operations.o: module/operations.c
	gcc -c module/operations.c -ggdb

shop.o: module/shop.c
	gcc -c module/shop.c -ggdb

main.o: main.c shop.o
	gcc -c main.c -ggdb

prod: main.o shop.o operations.o global.o
	gcc main.o shop.o operations.o global.o -o prpr.exe -ggdb -lm
	rm *.o

#TDD
TEST_operations.o: TDD/TEST_operations.c
	gcc -c TDD/TEST_operations.c -ggdb

TEST_shop.o: TDD/TEST_shop.c
	gcc -c TDD/TEST_shop.c -ggdb

TEST_global.o: TDD/TEST_global.c
	gcc -c TDD/TEST_global.c -ggdb

TEST_operations: operations.o TEST_operations.o
	gcc operations.o TEST_operations.o -o test.exe -ggdb -lm
	rm *.o

TEST_shop: shop.o TEST_shop.o
	gcc shop.o TEST_shop.o -o test.exe -ggdb -lm

TEST_global: global.o TEST_global.o
	gcc global.o TEST_global.o -o test.exe -ggdb -lm
	rm *.o

test: TEST_operations TEST_shop TEST_global

#Clean
clean:
	rm -f *.o

clean_all:
	rm -f *.o
	rm -f *.exe