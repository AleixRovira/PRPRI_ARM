#include "../lib/shop.h"

void SHOP_register()
{
    Shop shop;

    printf("\tEnter shop name: ");
    scanf("%ms", &shop.name);

    printf("\tEnter shop address: ");
    scanf("%ms", &shop.address);

    printf("\tEnter shop phone: ");
    scanf("%ms", &shop.phone);

    printf("\tEnter shop email: ");
    scanf("%ms", &shop.email);

    printf("\tEnter shop code: ");
    scanf("%ms", &shop.code);

    printf("%s\n%s\n%s\n%s\n%s\n", shop.name, shop.address, shop.phone, shop.email, shop.code);

    free(shop.name);
    free(shop.address);
    free(shop.phone);
    free(shop.email);
    free(shop.code);

    printf("\nShop registered successfully!\n");
}