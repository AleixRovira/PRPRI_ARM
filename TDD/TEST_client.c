#include "../lib/client.h"

void TEST_CLIENT_freeClient() {
    Client client;
    client.name = strdup("Test Name");
    client.email = strdup("test@gmail.com");
    client.password = strdup("Test123_");
    client.card_number = strdup("1234567890123456");
    client.card_pin = 1234;
    client.balance = 100.0f;

    CLIENT_freeClient(&client);
    if (client.name != NULL || client.email != NULL || client.password != NULL || client.card_number != NULL || client.card_pin != 0 || client.balance != 0.0f) {
        printf("TEST_CLIENT_freeClient: FAILED\n");
    } else {
        printf("TEST_CLIENT_freeClient: PASSED\n");
    }
    
}

void TEST_CLIENT_findClientByEmail() {
    Client client = CLIENT_findClientByEmail("test@email.com");
    if (client.name == NULL) {
        printf("TEST_CLIENT_findClientByEmail (nonexistent email): PASSED\n");
    } else {
        printf("TEST_CLIENT_findClientByEmail (nonexistent email): FAILED\n");
        CLIENT_freeClient(&client);
    }

    client = CLIENT_findClientByEmail("email@gmail.com");
    if (client.name != NULL) {
        printf("TEST_CLIENT_findClientByEmail (existing email): PASSED\n");
        CLIENT_freeClient(&client);
    } else {
        printf("TEST_CLIENT_findClientByEmail (existing email): FAILED\n");
    }
}

void TEST_CLIENT_main() {
    TEST_CLIENT_freeClient();
}