#include "lib/shop.h"
#include <stdio.h>

void menu()
{
    printf("------------------------\n");
    printf("---LS Electronics Hub---\n");
    printf("------------------------\n\n");

    int option = 0;
    while (option != 5)
    {
        printf("\t1. Shop Register\n");
        printf("\t2. User Register\n");
        printf("\t3. Staff Access\n");
        printf("\t4. Client Login\n");
        printf("\t5. Exit\n");
        printf("Option: ");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            printf("\nShop Register:\n");
            SHOP_register();
            break;
        case 2:
            printf("\nYou selected User Register.\n");
            // Call function for user register here
            break;
        case 3:
            printf("\nYou selected Staff Access.\n");
            // Call function for staff access here
            break;
        case 4:
            printf("\nYou selected Client Login.\n");
            // Call function for client login here
            break;
        case 5:
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