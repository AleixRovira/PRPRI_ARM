#define _GNU_SOURCE

#include "../lib/staff.h"

void STAFF_freeStaff(Staff *staff)
{
    free(staff->name);
    free(staff->email);
    free(staff->password);
    free(staff->shop_code);

    // Optional: set pointers to NULL to avoid double free
    staff->name = NULL;
    staff->email = NULL;
    staff->password = NULL;
    staff->shop_code = NULL;
}

Staff STAFF_findStaffByEmail(char *email)
{
    FILE *file = fopen("files/staff.txt", "r");
    if (!file)
    {
        return (Staff){NULL, NULL, NULL, NULL};
    }

    Staff staff = {NULL, NULL, NULL, NULL};
    while (fscanf(file, "%m[^;];%m[^;];%m[^;];%m[^;]", &staff.name, &staff.email, &staff.password, &staff.shop_code) == 4)
    {
        if (strcmp(staff.email, email) == 0)
        {
            fclose(file);
            return staff;
        }
        STAFF_freeStaff(&staff);
    }

    fclose(file);
    return (Staff){NULL, NULL, NULL, NULL};
}

void STAFF_register()
{
    Staff staff;

    printf("\tEnter staff name: ");
    scanf("%ms", &staff.name);

    Staff aux;
    int found;
    do
    {
        printf("\tEnter staff email: ");
        scanf("%ms", &staff.email);
        if (!GLOBAL_validateEmail(staff.email))
        {
            printf("\nERROR: Invalid email.  Please enter a valid email.\n");
            free(staff.email);
            staff.email = NULL;
        }

        aux = STAFF_findStaffByEmail(staff.email);
        found = 0;
        if(aux.name != NULL)
        {
            printf("\nERROR: Email already exists. Please enter a different email.\n");
            free(staff.email);
            staff.email = NULL;
            STAFF_freeStaff(&aux);
            found = 1;
        }
    } while (staff.email == NULL || found);

    do
    {
        printf("\tEnter staff password: ");
        scanf("%ms", &staff.password);
        if (!GLOBAL_validatePassword(staff.password))
        {
            printf("\nERROR: Invalid password.\nPassword must be:\n- At least 8 characters long.\n- Contain at least one uppercase letter.\n- Contain at least one lowercase letter.\n- Contain at least one special character.\n");
            free(staff.password);
            staff.password = NULL;
        }
    } while (staff.password == NULL);

    Shop shop;
    do
    {
        printf("\tEnter shop code: ");
        scanf("%ms", &staff.shop_code);
        shop = SHOP_findShopByCode(staff.shop_code);
        if (shop.name == NULL)
        {
            printf("\nERROR: Shop code does not exist. Please enter a valid shop code.\n");
            free(staff.shop_code);
            staff.shop_code = NULL;
        }
    } while (staff.shop_code == NULL);
    SHOP_freeShop(&shop);

    char *buffer = NULL;
    asprintf(&buffer, "%s;%s;%s;%s", staff.name, staff.email, staff.password, staff.shop_code);
    GLOBAL_printLineInFile("files/staff.txt", buffer);
    free(buffer);

    STAFF_freeStaff(&staff);

    printf("\nStaff registered successfully:\n");
}