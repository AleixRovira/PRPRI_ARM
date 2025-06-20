#include "../lib/shop.h"

void TEST_SHOP_freeShop()
{
    Shop shop;
    shop.name = strdup("Test Shop");
    shop.address = strdup("123 Test St");
    shop.phone = strdup("123-456-7890");
    shop.email = strdup("Test Email");
    shop.code = strdup("S001");
    shop.latitude = 41.38879;
    shop.longitude = 2.15899;
    SHOP_freeShop(&shop);
    if (shop.name == NULL && shop.address == NULL && shop.phone == NULL && shop.email == NULL && shop.code == NULL && shop.latitude == 0.0f && shop.longitude == 0.0f)
    {
        printf("TEST_SHOP_freeShop: PASSED\n");
    }
    else
    {
        printf("TEST_SHOP_freeShop: FAILED\n");
    }
}

void TEST_SHOP_findShopByCode()
{
    Shop shop = SHOP_findShopByCode("12345");
    if (shop.name == NULL)
    {
        printf("TEST_SHOP_findShopByCode: PASSED\n");
        SHOP_freeShop(&shop);
    }
    else
    {
        printf("TEST_SHOP_findShopByCode: FAILED\n");
    }

    shop = SHOP_findShopByCode("S001");
    if (shop.name != NULL)
    {
        printf("TEST_SHOP_findShopByCode: PASSED\n");
        SHOP_freeShop(&shop);
    }
    else
    {
        printf("TEST_SHOP_findShopByCode: FAILED\n");
    }
}

void TEST_SHOP_main()
{
    TEST_SHOP_freeShop();
    TEST_SHOP_findShopByCode();
}