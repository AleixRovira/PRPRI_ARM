#include "../lib/shop.h"

void TEST_SHOP_findShopByCode()
{
    Shop shop = SHOP_findShopByCode("12345");
    if (shop.name == NULL)
    {
        printf("TEST_SHOP_findShopByCode: PASSED\n");
        free(shop.name);
        free(shop.address);
        free(shop.phone);
        free(shop.email);
        free(shop.code);
    }
    else
    {
        printf("TEST_SHOP_findShopByCode: FAILED\n");
    }

    shop = SHOP_findShopByCode("S001");
    if (shop.name != NULL)
    {
        printf("TEST_SHOP_findShopByCode: PASSED\n");
        free(shop.name);
        free(shop.address);
        free(shop.phone);
        free(shop.email);
        free(shop.code);
    }
    else
    {
        printf("TEST_SHOP_findShopByCode: FAILED\n");
    }
}

int main()
{
    TEST_SHOP_findShopByCode();
    return 0;
}