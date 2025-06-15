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
        printf("TEST_CLIENT_findClientByEmail: PASSED\n");
    } else {
        printf("TEST_CLIENT_findClientByEmail: FAILED\n");
        CLIENT_freeClient(&client);
    }

    client = CLIENT_findClientByEmail("email@gmail.com");
    if (client.name != NULL) {
        printf("TEST_CLIENT_findClientByEmail: PASSED\n");
        CLIENT_freeClient(&client);
    } else {
        printf("TEST_CLIENT_findClientByEmail: FAILED\n");
    }
}

void TEST_CLIENT_getNearShops() {
    float latitude = 40.4168f;
    float longitude = -3.7038f;

    Shop *near_shops = CLIENT_getNearShops(latitude, longitude);
    for(int i = 0; i < 5 &&near_shops[i].name != NULL; i++) {
        printf("Shop %d: %s, Address: %s, Phone: %s, Email: %s, Coordinates: (%.2f, %.2f)\n",
               i + 1, near_shops[i].name, near_shops[i].address, near_shops[i].phone,
               near_shops[i].email, near_shops[i].latitude, near_shops[i].longitude);
    }
}

void TEST_CLIENT_main() {
    TEST_CLIENT_freeClient();
    TEST_CLIENT_findClientByEmail();
    TEST_CLIENT_getNearShops();
}