#include "../lib/global.h"

void TEST_GLOBAL_printLineInFile()
{
    if (!GLOBAL_printLineInFile("files/test.txt", "This is a test line."))
    {
        printf("TEST_GLOBAL_printLineInFile: PASSED\n");
    }
    else
    {
        printf("TEST_GLOBAL_printLineInFile: FAILED\n");
    }
}

int main()
{
    TEST_GLOBAL_printLineInFile();
    return 0;
}