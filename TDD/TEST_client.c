#include "../lib/client.h"

void TEST_CLIENT_freeClient() {
    Client client;
    client.name = strdup("Test Name");
    client.email = strdup("test@gmail.com");
    client.password = strdup("Test123_");
    client.card_number = strdup("1234567890123456");
    client.balance = 100.0f;

    CLIENT_freeClient(&client);
    if (client.name != NULL || client.email != NULL || client.password != NULL || client.card_number != NULL || client.balance != 0.0f) {
        printf("TEST_CLIENT_freeClient: FAILED\n");
    } else {
        printf("TEST_CLIENT_freeClient: PASSED\n");
    }
    
}

void TEST_CLIENT_main() {
    TEST_CLIENT_freeClient();
}