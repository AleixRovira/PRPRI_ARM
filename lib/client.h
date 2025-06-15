#ifndef _CLIENT_H
#define _CLIENT_H

#include "global.h"
#include "shop.h"
#include "operations.h"

typedef struct {
    char *name;
    char *email;
    char *password;
    char *card_number;
    int card_pin;
    float balance;
} Client;

void CLIENT_freeClient(Client *client);
Client CLIENT_findClientByEmail(char *email);
void CLIENT_register();
Shop *CLIENT_getNearShops(float latitude, float longitude);
void CLIENT_findNearShops();
void CLIENT_menu();
void CLIENT_login();


#endif