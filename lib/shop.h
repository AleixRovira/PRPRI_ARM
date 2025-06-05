#ifndef _SHOP_H_
#define _SHOP_H_

#include <stdio.h>
#include <stdlib.h>

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

Shop SHOP_findShopByCode(char *code);
void SHOP_register();

#endif