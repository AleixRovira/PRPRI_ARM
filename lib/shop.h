#ifndef _SHOP_H_
#define _SHOP_H_

#include <stdio.h>

struct Shop {
    char name[50];
    char address[100];
    char phone[15];
    char email[50];
    char code[10];
    float latitude;     // [-90.0, 90.0]
    float longitude;    // [-180.0, 180.0]
};

void SHOP_register();

#endif