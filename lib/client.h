#ifndef _CLIENT_H
#define _CLIENT_H

#include "global.h"

typedef struct {
    char *name;
    char *email;
    char *password;
    char *card_number;
    int card_pin;
    float balance;
} Client;

void CLIENT_freeClient(Client *client);
void CLIENT_register();

#endif