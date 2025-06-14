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

        // Validar formato del email
        if (!GLOBAL_validateEmail(client.email)) {
            printf("\nERROR: Invalid email. Please enter a valid email.\n");
            free(client.email);
            client.email = NULL;
        }

        // Verificar si el email ya existe
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

        // Validar formato de la contraseña
        if (!GLOBAL_validatePassword(client.password)) {
            printf("\nERROR: Invalid password.\nPassword must be:\n- At least 8 characters long.\n- Contain at least one uppercase letter.\n- Contain at least one lowercase letter.\n- Contain at least one special character.\n");
            free(client.password);
            client.password = NULL;
        }
    } while (client.password == NULL);

    printf("\tEnter client card number: ");
    scanf("%ms", &client.card_number);

    printf("\tEnter client card PIN: ");
    scanf("%d", &client.card_pin);

    printf("\tEnter client balance: ");
    scanf("%f", &client.balance);

    CLIENT_freeClient(&client);
    printf("\nClient registered successfully!\n");
}