#include <stdio.h>

void TEST_OP_main();
void TEST_SHOP_main();
void TEST_STAFF_main();
void TEST_GLOBAL_main();
void TEST_CLIENT_main();

int main()
{
    printf("Running tests...\n\n");
    
    TEST_OP_main();
    TEST_SHOP_main();
    TEST_STAFF_main();
    TEST_GLOBAL_main();
    TEST_CLIENT_main();

    printf("\nAll tests completed.\n");
    
    return 0;
}