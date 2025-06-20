#ifndef _SHOP_H_
#define _SHOP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

typedef struct
{
    char *name;
    char *address;
    char *phone;
    char *email;
    char *code;
    float latitude;  // [-90.0, 90.0]
    float longitude; // [-180.0, 180.0]
} Shop;

void SHOP_freeShop(Shop *shop);
Shop SHOP_findShopByCode(char *code);
void SHOP_register();

#endif