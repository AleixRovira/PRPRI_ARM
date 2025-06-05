#define _GNU_SOURCE

#include "../lib/shop.h"

Shop SHOP_findShopByCode(char *code)
{
    FILE *file = fopen("files/shops.txt", "r");
    if (!file)
    {
        return (Shop){NULL, NULL, NULL, NULL, NULL, 0.0, 0.0};
    }

    Shop shop = {NULL, NULL, NULL, NULL, NULL, 0.0f, 0.0f};
    while (fscanf(file, "%m[^;];%m[^;];%m[^;];%m[^;];%m[^;];%f;%f", &shop.code, &shop.name, &shop.address, &shop.phone, &shop.email, &shop.latitude, &shop.longitude) == 7)
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
    return (Shop){NULL, NULL, NULL, NULL, NULL, 0.0f, 0.0f};
}

void SHOP_register()
{
    Shop shop;

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

    printf("\tEnter shop name: ");
    scanf("%ms", &shop.name);

    printf("\tEnter shop address: ");
    scanf("%ms", &shop.address);

    printf("\tEnter shop phone: ");
    scanf("%ms", &shop.phone);

    printf("\tEnter shop email: ");
    scanf("%ms", &shop.email);

    do {
        printf("\tEnter shop latitude (-90.0 to 90.0): ");
        scanf("%f", &shop.latitude);
        if (shop.latitude < -90.0f || shop.latitude > 90.0f)
        {
            printf("\nERROR: Latitude must be between -90.0 and 90.0.\n");
        }
    } while (shop.latitude < -90.0f || shop.latitude > 90.0f);

    do {
        printf("\tEnter shop longitude (-180.0 to 180.0): ");
        scanf("%f", &shop.longitude);
        if (shop.longitude < -180.0f || shop.longitude > 180.0f)
        {
            printf("\nERROR: Longitude must be between -180.0 and 180.0.\n");
        }
    } while (shop.longitude < -180.0f || shop.longitude > 180.0f);

    char *buffer = NULL;
    asprintf(&buffer, "%s;%s;%s;%s;%s;%.6f;%.6f", shop.code, shop.name, shop.address, shop.phone, shop.email, shop.latitude, shop.longitude);
    GLOBAL_printLineInFile("files/shops.txt", buffer);
    free(buffer);
    
    free(shop.code);
    free(shop.name);
    free(shop.address);
    free(shop.phone);
    free(shop.email);

    printf("\nShop registered successfully!\n");
}