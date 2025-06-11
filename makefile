#all: prpr

#Final code
global.o: module/global.c
	gcc -c module/global.c -ggdb

operations.o: module/operations.c
	gcc -c module/operations.c -ggdb

shop.o: module/shop.c global.o
	gcc -c module/shop.c -ggdb

staff.o: module/staff.c shop.o global.o
	gcc -c module/staff.c -ggdb

main.o: main.c shop.o staff.o
	gcc -c main.c -ggdb

prod: main.o shop.o staff.o operations.o global.o
	gcc main.o shop.o staff.o operations.o global.o -o prpr.exe -ggdb -lm
	rm *.o

#TDD
TEST_operations.o: TDD/TEST_operations.c
	gcc -c TDD/TEST_operations.c -ggdb

TEST_shop.o: TDD/TEST_shop.c
	gcc -c TDD/TEST_shop.c -ggdb

TEST_staff.o: TDD/TEST_staff.c
	gcc -c TDD/TEST_staff.c -ggdb

TEST_global.o: TDD/TEST_global.c
	gcc -c TDD/TEST_global.c -ggdb

TEST_main.o: TDD/TEST_main.c
	gcc -c TDD/TEST_main.c -ggdb

test: TEST_main.o TEST_operations.o TEST_shop.o TEST_staff.o TEST_global.o operations.o shop.o staff.o global.o
	gcc TEST_main.o TEST_operations.o TEST_shop.o TEST_staff.o TEST_global.o operations.o shop.o staff.o global.o -o test.exe -ggdb -lm
	rm *.o
	
#Clean
clean:
	rm -f *.o

clean_all:
	rm -f *.o
	rm -f *.exe