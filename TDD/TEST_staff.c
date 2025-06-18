#include "../lib/staff.h"

void TEST_STAFF_freeStaff() {
    Staff staff;
    staff.name = strdup("Test Staff");
    staff.email = strdup("Test Email");
    staff.password = strdup("Test Password");
    staff.shop_code = strdup("S001");
    STAFF_freeStaff(&staff);
    if (staff.name == NULL && staff.email == NULL && staff.password == NULL && staff.shop_code == NULL) {
        printf("TEST_STAFF_freeStaff: PASSED\n");
    } else {
        printf("TEST_STAFF_freeStaff: FAILED\n");
    }
}

void TEST_PRODUCT_freeProduct() {
    Product product;
    product.code = strdup("P001");
    product.name = strdup("Test Product");
    product.category = strdup("Test Category");
    product.price = 10.0f;
    product.quantity = 5;
    product.description = strdup("Test Description");
    product.shop_code = strdup("S001");
    PRODUCT_freeProduct(&product);
    if (product.code == NULL && product.name == NULL && product.category == NULL && product.description == NULL && product.shop_code == NULL && product.price == 0.0f && product.quantity == 0) {
        printf("TEST_PRODUCT_freeProduct: PASSED\n");
    } else {
        printf("TEST_PRODUCT_freeProduct: FAILED\n");
    }
}

void TEST_DISCOUNT_freeDiscount() {
    Discount discount;
    discount.product_code = strdup("P001");
    discount.shop_code = strdup("S001");
    discount.discount_code = strdup("D001");
    discount.start_date = strdup("2023-01-01");
    discount.end_date = strdup("2023-12-31");
    DISCOUNT_freeDiscount(&discount);
    if (discount.product_code == NULL && discount.shop_code == NULL && discount.discount_code == NULL && discount.start_date == NULL && discount.end_date == NULL) {
        printf("TEST_DISCOUNT_freeDiscount: PASSED\n");
    } else {
        printf("TEST_DISCOUNT_freeDiscount: FAILED\n");
    }
}

void TEST_STAFF_findStaffByEmail() {
    Staff staff = STAFF_findStaffByEmail("test@email.com");
    if (staff.name == NULL) {
        printf("TEST_STAFF_findStaffByEmail: PASSED\n");
    } else {
        printf("TEST_STAFF_findStaffByEmail: FAILED\n");
        STAFF_freeStaff(&staff);
    }

    staff = STAFF_findStaffByEmail("email@gmail.com");
    if (staff.name != NULL) {
        printf("TEST_STAFF_findStaffByEmail: PASSED\n");
        STAFF_freeStaff(&staff);
    } else {
        printf("TEST_STAFF_findStaffByEmail: FAILED\n");
    }
}

void TEST_PRODUCT_findProductByCode() {
    Product product = PRODUCT_findProductByCode("Test Product", "S001");
    if (product.code == NULL) {
        printf("TEST_PRODUCT_findProductByCode: PASSED\n");
    } else {
        printf("TEST_PRODUCT_findProductByode: FAILED\n");
        PRODUCT_freeProduct(&product);
    }

    product = PRODUCT_findProductByCode("P001", "S001");
    if (product.code == NULL) {
        printf("TEST_PRODUCT_findProductByCode: PASSED\n");
    } else {
        printf("TEST_PRODUCT_findProductByCode: FAILED\n");
        PRODUCT_freeProduct(&product);
    }

    product = PRODUCT_findProductByCode("P001", "code");
    if (product.code != NULL) {
        printf("TEST_PRODUCT_findProductByCode: PASSED\n");
        PRODUCT_freeProduct(&product);
    } else {
        printf("TEST_PRODUCT_findProductByCode: FAILED\n");
    }
}

void TEST_STAF_getProductsByShop() {
    int count = 0;
    Product *products = STAFF_getProductsByShop("S001", &count);
    if (products != NULL && count > 0) {
        printf("TEST_STAF_getProductsByShop: PASSED\n");
        for (int i = 0; i < count; i++) {
            PRODUCT_freeProduct(&products[i]);
        }
        free(products);
    } else {
        printf("TEST_STAF_getProductsByShop: FAILED\n");
    }
}

void TEST_STAFF_checkIfFileExists() {
    char *filename = "files/staff.txt";
    if (STAFF_checkIfFileExists(filename)) {
        printf("TEST_STAFF_checkIfFileExists: PASSED\n");
    } else {
        printf("TEST_STAFF_checkIfFileExists: FAILED\n");
    }
}

void TEST_STAFF_main() {

    TEST_STAFF_freeStaff();
    TEST_PRODUCT_freeProduct();
    TEST_DISCOUNT_freeDiscount();
    TEST_STAFF_findStaffByEmail();
    TEST_PRODUCT_findProductByCode();
    TEST_STAF_getProductsByShop();
    TEST_STAFF_checkIfFileExists();
}