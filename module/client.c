#define _GNU_SOURCE

#include "../lib/client.h"

void CLIENT_freeClient(Client *client)
{
    free(client->name);
    free(client->email);
    free(client->password);
    free(client->card_number);

    // Optional: set pointers to NULL to avoid double free
    client->name = NULL;
    client->email = NULL;
    client->password = NULL;
    client->card_number = NULL;
    client->card_pin = 0;
    client->balance = 0.0f;
}

Client CLIENT_findClientByEmail(char *email)
{
    FILE *file = fopen("files/clients.txt", "r");
    if (!file)
    {
        return (Client){NULL, NULL, NULL, NULL, 0, 0.0f};
    }

    Client client = {NULL, NULL, NULL, NULL, 0, 0.0f};
    while (fscanf(file, " %m[^;];%m[^;];%m[^;];%m[^;];%d;%f", &client.name, &client.email, &client.password, &client.card_number, &client.card_pin, &client.balance) == 6)
    {
        if (strcmp(client.email, email) == 0)
        {
            fclose(file);
            return client;
        }
        CLIENT_freeClient(&client);
    }

    fclose(file);
    return (Client){NULL, NULL, NULL, NULL, 0, 0.0f};
}

void CLIENT_register()
{

    Client client;

    printf("\tEnter client name: ");
    scanf("%ms", &client.name);

    Client aux;
    int found;
    do
    {
        printf("\tEnter client email: ");
        scanf("%ms", &client.email);

        if (!GLOBAL_validateEmail(client.email))
        {
            printf("\nERROR: Invalid email. Please enter a valid email.\n");
            free(client.email);
            client.email = NULL;
        }

        aux = CLIENT_findClientByEmail(client.email);
        found = 0;
        if (aux.name != NULL)
        {
            printf("\nERROR: Email already exists. Please enter a different email.\n");
            free(client.email);
            client.email = NULL;
            CLIENT_freeClient(&aux);
            found = 1;
        }
    } while (client.email == NULL || found);

    do
    {
        printf("\tEnter client password: ");
        scanf("%ms", &client.password);

        if (!GLOBAL_validatePassword(client.password))
        {
            printf("\nERROR: Invalid password.\nPassword must be:\n- At least 8 characters long.\n- Contain at least one uppercase letter.\n- Contain at least one lowercase letter.\n- Contain at least one special character.\n");
            free(client.password);
            client.password = NULL;
        }
    } while (client.password == NULL);

    printf("\tEnter client card number: ");
    scanf("%ms", &client.card_number);

    do
    {
        printf("\tEnter client PIN (4 digits): ");
        scanf("%d", &client.card_pin);

        if (client.card_pin < 1000 || client.card_pin > 9999)
        {
            printf("\nERROR: Invalid PIN. Please enter a 4-digit PIN.\n");
            client.card_pin = 0;
        }
    } while (client.card_pin == 0);

    do
    {
        printf("\tEnter client balance (€): ");
        scanf("%f", &client.balance);

        if (client.balance < 0)
        {
            printf("\nERROR: Invalid balance. Balance must be 0 or positive.\n");
            client.balance = -1.0f;
        }
    } while (client.balance < 0);

    char *buffer = NULL;
    asprintf(&buffer, "%s;%s;%s;%s;%d;%.2f", client.name, client.email, client.password, client.card_number, client.card_pin, client.balance);
    GLOBAL_printLineInFile("files/clients.txt", buffer);
    free(buffer);

    CLIENT_freeClient(&client);
    printf("\nClient registered successfully!\n");
}

Shop *CLIENT_getNearShops(float latitude, float longitude, int *n_shops)
{
    FILE *file = fopen("files/shops.txt", "r");
    if (!file)
    {
        printf("\nERROR: Could not open shops file.\n");
        *n_shops = 0;
        return NULL;
    }

    int max_shops = 100;
    Shop *shops = malloc(sizeof(Shop) * max_shops);
    Coordinates *coords = malloc(sizeof(Coordinates) * max_shops);

    *n_shops = 0;
    Shop shop;
    while (fscanf(file, " %m[^;];%m[^;];%m[^;];%m[^;];%m[^;];%f;%f",
                  &shop.code, &shop.name, &shop.address, &shop.phone, &shop.email, &shop.latitude, &shop.longitude) == 7)
    {
        if (*n_shops >= max_shops)
            break;
        shops[*n_shops].code = strdup(shop.code);
        shops[*n_shops].name = strdup(shop.name);
        shops[*n_shops].address = strdup(shop.address);
        shops[*n_shops].phone = strdup(shop.phone);
        shops[*n_shops].email = strdup(shop.email);
        shops[*n_shops].latitude = shop.latitude;
        shops[*n_shops].longitude = shop.longitude;
        coords[*n_shops].latitude = shop.latitude;
        coords[*n_shops].longitude = shop.longitude;
        (*n_shops)++;
        SHOP_freeShop(&shop);
    }
    fclose(file);

    if (*n_shops == 0)
    {
        free(shops);
        free(coords);
        return NULL;
    }

    char *selected = calloc(*n_shops, sizeof(char));
    Shop *nearest_shops = malloc(sizeof(Shop) * 5);

    Coordinates ref = {latitude, longitude};
    int found = 0;
    while (found < 5 && found < *n_shops)
    {
        int nearest = -1;
        for (int j = 0; j < *n_shops; j++)
        {
            if (selected[j])
                continue;
            if (nearest == -1)
            {
                nearest = j;
            }
            else
            {
                nearest = OP_findNearest(nearest, coords[nearest], j, coords[j], ref);
            }
        }
        if (nearest != -1)
        {
            nearest_shops[found].code = strdup(shops[nearest].code);
            nearest_shops[found].name = strdup(shops[nearest].name);
            nearest_shops[found].address = strdup(shops[nearest].address);
            nearest_shops[found].phone = strdup(shops[nearest].phone);
            nearest_shops[found].email = strdup(shops[nearest].email);
            nearest_shops[found].latitude = shops[nearest].latitude;
            nearest_shops[found].longitude = shops[nearest].longitude;
            selected[nearest] = 1;
            found++;
        }
    }

    for (int i = 0; i < *n_shops; i++)
        SHOP_freeShop(&shops[i]);
    free(shops);
    free(coords);
    free(selected);

    *n_shops = found;
    return nearest_shops;
}

void CLIENT_findNearShops()
{
    float latitude, longitude;

    do
    {
        printf("\tEnter your latitude (-90.0 to 90.0): ");
        scanf("%f", &latitude);
        if (latitude < -90.0f || latitude > 90.0f)
        {
            printf("\nERROR: Latitude must be between -90.0 and 90.0. Please enter a valid latitude.\n");
        }
    } while (latitude < -90.0f || latitude > 90.0f);

    do
    {
        printf("\tEnter your longitude (-180.0 to 180.0): ");
        scanf("%f", &longitude);
        if (longitude < -180.0f || longitude > 180.0f)
        {
            printf("\nERROR: Longitude must be between -180.0 and 180.0. Please enter a valid longitude.\n");
        }
    } while (longitude < -180.0f || longitude > 180.0f);

    int n_shops;
    Shop *near_shops = CLIENT_getNearShops(latitude, longitude, &n_shops);

    if (n_shops == 0)
    {
        printf("\nERROR: No shops found near your location.\n");
        return;
    }

    printf("\nNEAR SHOPS:\n");
    for (int i = 0; i < n_shops; i++)
    {
        printf("\nShop %d:\n", i + 1);
        printf("\tName: %s\n", near_shops[i].name);
        printf("\tAddress: %s\n", near_shops[i].address);
        printf("\tPhone: %s\n", near_shops[i].phone);
        printf("\tEmail: %s\n", near_shops[i].email);
        printf("\tCoordinates: (%.2f, %.2f)\n\n", near_shops[i].latitude, near_shops[i].longitude);
        SHOP_freeShop(&near_shops[i]);
    }
    free(near_shops);
    near_shops = NULL;
}

Discount *CLIENT_getDiscounts(int *n_discounts)
{
    FILE *file = fopen("files/discounts.txt", "r");
    if (!file)
    {
        printf("\nERROR: Could not open discounts file.\n");
        *n_discounts = 0;
        return NULL;
    }

    *n_discounts = 0;
    Discount discount;
    Discount *discounts = NULL;
    while (fscanf(file, " %m[^;];%m[^;];%m[^;];%m[^;];%ms", &discount.discount_code, &discount.product_code, &discount.shop_code, &discount.start_date, &discount.end_date) == 5)
    {
        discounts = realloc(discounts, sizeof(Discount) * (*n_discounts + 1));
        discounts[*n_discounts].product_code = strdup(discount.product_code);
        discounts[*n_discounts].shop_code = strdup(discount.shop_code);
        discounts[*n_discounts].discount_code = strdup(discount.discount_code);
        discounts[*n_discounts].start_date = strdup(discount.start_date);
        discounts[*n_discounts].end_date = strdup(discount.end_date);

        (*n_discounts)++;
        DISCOUNT_freeDiscount(&discount);
    }
    fclose(file);
    return discounts;
}

Shop *CLIENT_getShopsWithDiscounts(Discount *discounts, int n_discounts, int *n_shops)
{
    if (n_discounts == 0)
    {
        *n_shops = 0;
        return NULL;
    }

    Shop *shops = NULL;
    *n_shops = 0;

    for (int i = 0; i < n_discounts; i++)
    {
        int already_added = 0;
        for (int j = 0; j < *n_shops; j++)
        {
            if (strcmp(shops[j].code, discounts[i].shop_code) == 0)
            {
                already_added = 1;
                break;
            }
        }
        if (already_added)
            continue;

        FILE *file = fopen("files/shops.txt", "r");
        if (!file)
        {
            printf("\nERROR: Could not open shops file.\n");
            for (int k = 0; k < *n_shops; k++)
                SHOP_freeShop(&shops[k]);
            free(shops);
            return NULL;
        }

        Shop shop;
        int found = 0;
        while (fscanf(file, " %m[^;];%m[^;];%m[^;];%m[^;];%m[^;];%f;%f",
                      &shop.code, &shop.name, &shop.address, &shop.phone, &shop.email, &shop.latitude, &shop.longitude) == 7)
        {
            if (strcmp(shop.code, discounts[i].shop_code) == 0)
            {
                shops = realloc(shops, sizeof(Shop) * (*n_shops + 1));
                shops[*n_shops] = shop;
                (*n_shops)++;
                found = 1;
                break;
            }
            SHOP_freeShop(&shop);
        }
        fclose(file);
        if (!found)
        {
            SHOP_freeShop(&shop);
        }
    }

    return shops;
}

void CLIENT_viewDiscounts()
{
    int n_discounts = 0;
    Discount *discounts = CLIENT_getDiscounts(&n_discounts);

    if (n_discounts == 0)
    {
        printf("\nNo discounts found.\n");
        return;
    }

    int n_shops = 0;
    Shop *shops_with_discounts = CLIENT_getShopsWithDiscounts(discounts, n_discounts, &n_shops);

    if (n_shops == 0)
    {
        printf("\nNo shops found with discounts.\n");
        free(discounts);
        return;
    }

    printf("\nDISCOUNTS:\n");
    for (int i = 0; i < n_shops; i++)
    {
        printf("\nSHOP %d:\n", i + 1);
        for (int j = 0; j < n_discounts; j++)
        {
            if (strcmp(shops_with_discounts[i].code, discounts[j].shop_code) == 0)
            {
                printf("\tDISCOUNT %d:\n", j + 1);
                printf("\tDiscount Code: %s\n", discounts[j].discount_code);
                printf("\tProduct Code: %s\n", discounts[j].product_code);
                printf("\tStart Date: %s\n", discounts[j].start_date);
                printf("\tEnd Date: %s\n\n", discounts[j].end_date);
            }
        }
    }

    for (int i = 0; i < n_discounts; i++)
    {
        DISCOUNT_freeDiscount(&discounts[i]);
    }
    free(discounts);

    for (int i = 0; i < n_shops; i++)
    {
        SHOP_freeShop(&shops_with_discounts[i]);
    }
    free(shops_with_discounts);
    discounts = NULL;
}

void CLIENT_menu()
{
    int option = 0;
    while (option != 3)
    {
        printf("\t1. Find near shops\n");
        printf("\t2. View discounts\n");
        printf("\t3. Logout\n");
        printf("Option: ");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            printf("\nFIND NEAR SHOPS\n");
            CLIENT_findNearShops();
            break;
        case 2:
            printf("\nVIEW DISCOUNTS\n");
            CLIENT_viewDiscounts();
            break;
        case 3:
            printf("\nLogging out\n\n");
            break;
        default:
            printf("\nERROR: Invalid option.\n");
        }
    }
}

void CLIENT_login()
{
    char *email = NULL;
    char *password = NULL;
    Client client;
    int found = 0;

    do
    {
        printf("\tEnter client email: ");
        scanf("%ms", &email);

        printf("\tEnter client password: ");
        scanf("%ms", &password);

        client = CLIENT_findClientByEmail(email);

        if (client.name == NULL)
        {
            printf("\nERROR: Invalid email or password. Please try again.\n");
            CLIENT_freeClient(&client);
        }
        else if (strcmp(client.password, password) != 0)
        {
            printf("\nERROR: Invalid email or password. Please try again.\n");
            CLIENT_freeClient(&client);
        }
        else
        {
            found = 1;
            printf("\nLogin successful! Welcome, %s.\n", client.name);
        }

        free(email);
        free(password);
        email = NULL;
        password = NULL;

    } while (!found);

    CLIENT_freeClient(&client);

    CLIENT_menu();
}