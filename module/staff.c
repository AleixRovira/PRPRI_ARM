#include "../lib/staff.h"

void STAFF_register()
{
    Staff staff;

    printf("\tEnter staff name: ");
    scanf("%ms", &staff.name);

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
    }while(staff.email == NULL);

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
    free(shop.name);
    free(shop.address);
    free(shop.phone);
    free(shop.email);
    free(shop.code);

    //Guardar en el fichero

    free(staff.name);
    free(staff.email);
    free(staff.password);
    free(staff.shop_code);

    printf("\nStaff registered successfully:\n");
}