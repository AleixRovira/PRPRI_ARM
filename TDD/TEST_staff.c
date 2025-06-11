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

void TEST_STAFF_main() {

    TEST_STAFF_freeStaff();
    TEST_STAFF_findStaffByEmail();
}