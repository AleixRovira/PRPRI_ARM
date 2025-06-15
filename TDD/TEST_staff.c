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

void TEST_PRODUCT_findProductByName() {
    Product product = PRODUCT_findProductByName("Test Product", "S001");
    if (product.name == NULL) {
        printf("TEST_PRODUCT_findProductByName: PASSED\n");
    } else {
        printf("TEST_PRODUCT_findProductByName: FAILED\n");
        PRODUCT_freeProduct(&product);
    }

    product = PRODUCT_findProductByName("name", "S001");
    if (product.name == NULL) {
        printf("TEST_PRODUCT_findProductByName: PASSED\n");
    } else {
        printf("TEST_PRODUCT_findProductByName: FAILED\n");
        PRODUCT_freeProduct(&product);
    }

    product = PRODUCT_findProductByName("name", "code");
    if (product.name != NULL) {
        printf("TEST_PRODUCT_findProductByName: PASSED\n");
        PRODUCT_freeProduct(&product);
    } else {
        printf("TEST_PRODUCT_findProductByName: FAILED\n");
    }
}

void TEST_STAFF_main() {

    TEST_STAFF_freeStaff();
    TEST_STAFF_findStaffByEmail();
    TEST_PRODUCT_findProductByName();
}