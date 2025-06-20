#ifndef _STAFF_H_
#define _STAFF_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "shop.h"
#include "operations.h"

typedef struct
{
    char *name;
    char *email;
    char *password;
    char *shop_code;
} Staff;

typedef struct
{
    char *code;
    char *name;
    char *category;
    float price;
    int quantity;
    char *description;
    char *shop_code;
} Product;

typedef struct 
{
    char *product_code;
    char *shop_code;
    char *discount_code;
    char *start_date;
    char *end_date;
} Discount;


void STAFF_freeStaff(Staff *staff);
void PRODUCT_freeProduct(Product *product);
void DISCOUNT_freeDiscount(Discount *discount);
Staff STAFF_findStaffByEmail(char *email);
void STAFF_register();
Product PRODUCT_findProductByCode(char *name, char *shop_code);
void STAFF_addProduct(Staff staff);
void STAFF_updateProductInFile(char *old_code, Product new_product);
void STAFF_updateProduct(Staff staff);
void STAFF_updateShopInFile(Shop shop);
void STAFF_updateShop(Staff staff);
void STAFF_addDiscount(Staff staff);
Discount DISCOUNT_findDiscountByCode(char *discount_code, char *shop_code);
void DISCOUNT_updateDiscountInFile(Discount old, char *new_code);
void STAFF_editDiscount(Staff staff);
void STAFF_deleteDiscount(Staff staff);
Product *STAFF_getProductsByShop(char *shop_code, int *count);
void STAFF_viewStock(Staff staff);
char STAFF_checkIfFileExists(const char *filename);
void STAFF_createOrder(char *code, char *shop_code, char **product_codes, int *quantities, int count);
void STAFF_placeOrder(Staff staff);
void STAFF_updateProductQuantityInFile(char *product_code, int quantity, char *shop_code);
void STAFF_receiveProducts(char *filename, Staff staff);
void STAFF_receiveOrder(Staff staff);
Product *STAFF_getAllProducts(int *count);
void STAFF_showFilteredStock(int option, char *filter, float min_price, float max_price, int min_quantity, int max_quantity);
void STAFF_filterStock();
void STAFF_menu(Staff staff);
void STAFF_login();

#endif