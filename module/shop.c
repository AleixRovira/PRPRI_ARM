#define _GNU_SOURCE

#include "../lib/shop.h"

void SHOP_freeShop(Shop *shop)
{
    free(shop->code);
    free(shop->name);
    free(shop->address);
    free(shop->phone);
    free(shop->email);

    // Opcional: poner los punteros a NULL para evitar dobles free
    shop->code = NULL;
    shop->name = NULL;
    shop->address = NULL;
    shop->phone = NULL;
    shop->email = NULL;
    shop->latitude = 0.0f;
    shop->longitude = 0.0f;
}

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
        SHOP_freeShop(&shop);
    }

    fclose(file);
    return (Shop){NULL, NULL, NULL, NULL, NULL, 0.0f, 0.0f};
}

void SHOP_register()
{
    Shop shop;

    Shop aux;
    int found;
    do
    {
        printf("\tEnter shop code: ");
        scanf("%ms", &shop.code);
        aux = SHOP_findShopByCode(shop.code);
        found = 0;
        if (aux.name != NULL)
        {
            printf("\nERROR: Shop code already exists. Please enter a different code.\n");
            free(shop.code);
            shop.code = NULL;
            SHOP_freeShop(&aux);
            found = 1;
        }
    } while (found);

    printf("\tEnter shop name: ");
    scanf("%ms", &shop.name);

    printf("\tEnter shop address: ");
    scanf(" %m[^\n]", &shop.address);

    printf("\tEnter shop phone: ");
    scanf("%ms", &shop.phone);

    do
    {
        printf("\tEnter shop email: ");
        scanf("%ms", &shop.email);
        if (!GLOBAL_validateEmail(shop.email))
        {
            printf("\nERROR: Invalid email. Please enter a valid email.\n");
            free(shop.email);
            shop.email = NULL;
        }
    } while (shop.email == NULL);

    do
    {
        printf("\tEnter shop latitude (-90.0 to 90.0): ");
        scanf("%f", &shop.latitude);
        if (shop.latitude < -90.0f || shop.latitude > 90.0f)
        {
            printf("\nERROR: Latitude must be between -90.0 and 90.0. Please enter a valid latitude.\n");
        }
    } while (shop.latitude < -90.0f || shop.latitude > 90.0f);

    do
    {
        printf("\tEnter shop longitude (-180.0 to 180.0): ");
        scanf("%f", &shop.longitude);
        if (shop.longitude < -180.0f || shop.longitude > 180.0f)
        {
            printf("\nERROR: Longitude must be between -180.0 and 180.0. Please enter a valid longitude.\n");
        }
    } while (shop.longitude < -180.0f || shop.longitude > 180.0f);

    char *buffer = NULL;
    asprintf(&buffer, "%s;%s;%s;%s;%s;%.6f;%.6f", shop.code, shop.name, shop.address, shop.phone, shop.email, shop.latitude, shop.longitude);
    GLOBAL_printLineInFile("files/shops.txt", buffer);
    free(buffer);

    SHOP_freeShop(&shop);

    printf("\nShop registered successfully!\n");
}