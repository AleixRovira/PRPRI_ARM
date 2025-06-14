#include "lib/shop.h"
#include "lib/staff.h"
#include "lib/client.h"
#include <stdio.h>

void menu()
{
    printf("------------------------\n");
    printf("---LS Electronics Hub---\n");
    printf("------------------------\n\n");

    int option = 0;
    while (option != 6)
    {
        printf("\t1. Shop Register\n");
        printf("\t2. User Register\n");
        printf("\t3. Staff Register\n");
        printf("\t4. Staff Access\n");
        printf("\t5. Client Login\n");
        printf("\t6. Exit\n");
        printf("Option: ");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            printf("\nSHOP REGISTER\n");
            SHOP_register();
            break;
        case 2:
            printf("\nUSER REGISTER\n");
            CLIENT_register();
            break;
        case 3:
            printf("\nSTAFF REGISTER\n");
            STAFF_register();
            break;
        case 4:
            printf("\nSTAFF ACCESS\n");
            STAFF_login();
            break;
        case 5:
            printf("\nCLIENT LOGIN\n");
            CLIENT_login();
            break;
        case 6:
            printf("\nTank you. Exiting the program.\n");
            break;
        default:
            printf("\nERROR: Invalid option.\n");
        }
    }
}

int main()
{

    menu();

    return 0;
}