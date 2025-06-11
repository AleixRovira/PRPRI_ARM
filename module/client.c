#include "../lib/client.h"

void CLIENT_freeClient(Client *client) {
    free(client->name);
    free(client->email);
    free(client->password);
    free(client->card_number);

    // Optional: set pointers to NULL to avoid double free
    client->name = NULL;
    client->email = NULL;
    client->password = NULL;
    client->card_number = NULL;
    client->balance = 0.0f;
}

void CLIENT_register() {

    Client client;

    printf("\tEnter client name: ");
    scanf("%ms", &client.name);

    printf("\tEnter client email: ");
    scanf("%ms", &client.email);

    printf("\tEnter client password: ");
    scanf("%ms", &client.password);

    printf("\tEnter client card number: ");
    scanf("%ms", &client.card_number);

    printf("\tEnter client balance: ");
    scanf("%f", &client.balance);

    CLIENT_freeClient(&client);
    printf("\nClient registered successfully!\n");
}