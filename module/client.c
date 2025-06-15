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
    while (fscanf(file, "%m[^;];%m[^;];%m[^;];%m[^;];%d;%f", &client.name, &client.email, &client.password, &client.card_number, &client.card_pin, &client.balance) == 6)
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
    do {
        printf("\tEnter client email: ");
        scanf("%ms", &client.email);

        if (!GLOBAL_validateEmail(client.email)) {
            printf("\nERROR: Invalid email. Please enter a valid email.\n");
            free(client.email);
            client.email = NULL;
        }

        aux = CLIENT_findClientByEmail(client.email);
        found = 0;
        if (aux.name != NULL) {
            printf("\nERROR: Email already exists. Please enter a different email.\n");
            free(client.email);
            client.email = NULL;
            CLIENT_freeClient(&aux);
            found = 1;
        }
    } while (client.email == NULL || found);

    do {
        printf("\tEnter client password: ");
        scanf("%ms", &client.password);

        if (!GLOBAL_validatePassword(client.password)) {
            printf("\nERROR: Invalid password.\nPassword must be:\n- At least 8 characters long.\n- Contain at least one uppercase letter.\n- Contain at least one lowercase letter.\n- Contain at least one special character.\n");
            free(client.password);
            client.password = NULL;
        }
    } while (client.password == NULL);

    printf("\tEnter client card number: ");
    scanf("%ms", &client.card_number);

    do {
        printf("\tEnter client PIN (4 digits): ");
        scanf("%d", &client.card_pin);

        if (client.card_pin < 1000 || client.card_pin > 9999) {
            printf("\nERROR: Invalid PIN. Please enter a 4-digit PIN.\n");
            client.card_pin = 0;
        }
    } while (client.card_pin == 0);

    do {
        printf("\tEnter client balance (€): ");
        scanf("%f", &client.balance);

        if (client.balance < 0) {
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
}

void CLIENT_menu()
{
    int option = 0;
    while (option != 3)
    {
        printf("\t1. Find near shops\n");
        printf("\t2. Action 2\n");
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
            printf("\nAction 2\n");
            break;
        case 3:
            printf("\nLogging out\n\n");
            break;
        default:
            printf("\nERROR: Invalid option.\n");
        }
    }
}

void CLIENT_login() {
    char *email = NULL;
    char *password = NULL;
    Client client;
    int found = 0;

    do {
        printf("\tEnter client email: ");
        scanf("%ms", &email);

        printf("\tEnter client password: ");
        scanf("%ms", &password);

        client = CLIENT_findClientByEmail(email);

        if (client.name == NULL) {
            printf("\nERROR: Invalid email or password. Please try again.\n");
            CLIENT_freeClient(&client);
        } else if (strcmp(client.password, password) != 0) {
            printf("\nERROR: Invalid email or password. Please try again.\n");
            CLIENT_freeClient(&client);
        } else {
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