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

void TEST_GLOBAL_validatePassword()
{
    if (GLOBAL_validatePassword("Valid1@Password"))
    {
        printf("TEST_GLOBAL_validatePassword: PASSED\n");
    }
    else
    {
        printf("TEST_GLOBAL_validatePassword: FAILED\n");
    }

    if (GLOBAL_validatePassword("invalidpassword"))
    {
        printf("TEST_GLOBAL_validatePassword: FAILED\n");
    }
    else
    {
        printf("TEST_GLOBAL_validatePassword: PASSED\n");
    }
}

void TEST_GLOBAL_validateEmail()
{
    if (GLOBAL_validateEmail("email@gmail.com"))
    {
        printf("TEST_GLOBAL_validateEmail: PASSED\n");
    }
    else
    {
        printf("TEST_GLOBAL_validateEmail: FAILED\n");
    }
    
    if (GLOBAL_validateEmail("invalid-email"))
    {
        printf("TEST_GLOBAL_validateEmail: FAILED\n");
    }
    else
    {
        printf("TEST_GLOBAL_validateEmail: PASSED\n");
    }
}

void TEST_GLOBAL_main()
{
    TEST_GLOBAL_printLineInFile();
    TEST_GLOBAL_validatePassword();
    TEST_GLOBAL_validateEmail();
}