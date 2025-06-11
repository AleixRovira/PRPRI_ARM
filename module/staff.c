#include "../lib/staff.h"

void STAFF_register()
{
    Staff staff;

    printf("\tEnter staff name: ");
    scanf("%ms", &staff.name);

    printf("\tEnter staff email: ");
    scanf("%ms", &staff.email);

    printf("\tEnter staff password: ");
    scanf("%ms", &staff.password);

    printf("\tEnter shop code: ");
    scanf("%ms", &staff.shop_code);

    
    printf("\nStaff registered successfully:\n");
    printf("Name: %s\n", staff.name);
    printf("Email: %s\n", staff.email);
    printf("Shop Code: %s\n", staff.shop_code);

    free(staff.name);
    free(staff.email);
    free(staff.password);
    free(staff.shop_code);
}