#include "../lib/shop.h"

Shop SHOP_findShopByCode(char *code)
{
    FILE *file = fopen("files/shops.txt", "r");
    if (!file)
    {
        return (Shop){NULL, NULL, NULL, NULL, NULL, 0.0, 0.0};
    }

    Shop shop = {NULL, NULL, NULL, NULL, NULL, 0.0f, 0.0f};
    while (fscanf(file, "%ms %ms %ms %ms %ms %f %f", &shop.name, &shop.address, &shop.phone, &shop.email, &shop.code, &shop.latitude, &shop.longitude) == 7)
    {
        if (strcmp(shop.code, code) == 0)
        {
            fclose(file);
            return shop;
        }
        free(shop.name);
        free(shop.address);
        free(shop.phone);
        free(shop.email);
        free(shop.code);
    }

    fclose(file);
    return (Shop){NULL, NULL, NULL, NULL, NULL, 0.0f, 0.0f}; // Return an empty shop if not found
}

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

    Shop aux;
    do
    {
        printf("\tEnter shop code: ");
        scanf("%ms", &shop.code);
        aux = SHOP_findShopByCode(shop.code);
        if (aux.name != NULL)
        {
            printf("\nERROR: Shop code already exists. Please enter a different code.\n");
            free(shop.code);
        }
    } while (aux.name != NULL);

    free(shop.name);
    free(shop.address);
    free(shop.phone);
    free(shop.email);
    free(shop.code);

    printf("\nShop registered successfully!\n");
}