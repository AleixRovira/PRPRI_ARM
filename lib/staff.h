#ifndef _STAFF_H_
#define _STAFF_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "shop.h"

typedef struct
{
    char *name;
    char *email;
    char *password;
    char *shop_code;
} Staff;

typedef struct
{
    char *name;
    char *category;
    float price;
    int quantity;
    char *description;
    char *shop_code;
} Product;

void STAFF_freeStaff(Staff *staff);
Staff STAFF_findStaffByEmail(char *email);
void STAFF_register();
void STAFF_addProduct(Staff staff);
void STAFF_menu(Staff staff);
void STAFF_login();

#endif