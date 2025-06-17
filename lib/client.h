#ifndef _CLIENT_H
#define _CLIENT_H

#include "global.h"
#include "shop.h"
#include "operations.h"
#include "staff.h"

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
Discount *CLIENT_getDiscounts(int *n_discounts);
Shop *CLIENT_getShopsWithDiscounts(Discount *discounts, int n_discounts, int *n_shops);
void CLIENT_viewDiscounts();
void CLIENT_menu();
void CLIENT_login();


#endif