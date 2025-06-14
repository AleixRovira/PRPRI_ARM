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

void STAFF_freeStaff(Staff *staff);
Staff STAFF_findStaffByEmail(char *email);
void STAFF_register();
void STAFF_menu();
void STAFF_login();

#endif