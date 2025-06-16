#define _GNU_SOURCE

#include "../lib/staff.h"

void STAFF_freeStaff(Staff *staff)
{
    free(staff->name);
    free(staff->email);
    free(staff->password);
    free(staff->shop_code);

    staff->name = NULL;
    staff->email = NULL;
    staff->password = NULL;
    staff->shop_code = NULL;
}

void PRODUCT_freeProduct(Product *product)
{
    free(product->code);
    free(product->name);
    free(product->category);
    free(product->description);
    free(product->shop_code);

    product->code = NULL;
    product->name = NULL;
    product->category = NULL;
    product->description = NULL;
    product->shop_code = NULL;
    product->price = 0.0f;
    product->quantity = 0;
}

void DISCOUNT_freeDiscount(Discount *discount)
{
    free(discount->product_code);
    free(discount->shop_code);
    free(discount->discount_code);
    free(discount->start_date);
    free(discount->end_date);

    discount->product_code = NULL;
    discount->shop_code = NULL;
    discount->discount_code = NULL;
    discount->start_date = NULL;
    discount->end_date = NULL;
}

Staff STAFF_findStaffByEmail(char *email)
{
    FILE *file = fopen("files/staff.txt", "r");
    if (!file)
    {
        return (Staff){NULL, NULL, NULL, NULL};
    }

    Staff staff = {NULL, NULL, NULL, NULL};
    while (fscanf(file, " %m[^;];%m[^;];%m[^;];%ms", &staff.name, &staff.email, &staff.password, &staff.shop_code) == 4)
    {
        if (strcmp(staff.email, email) == 0)
        {
            fclose(file);
            return staff;
        }
        STAFF_freeStaff(&staff);
    }

    fclose(file);
    return (Staff){NULL, NULL, NULL, NULL};
}

void STAFF_register()
{
    Staff staff;

    printf("\tEnter staff name: ");
    scanf("%ms", &staff.name);

    Staff aux;
    int found;
    do
    {
        printf("\tEnter staff email: ");
        scanf("%ms", &staff.email);
        if (!GLOBAL_validateEmail(staff.email))
        {
            printf("\nERROR: Invalid email.  Please enter a valid email.\n");
            free(staff.email);
            staff.email = NULL;
        }

        aux = STAFF_findStaffByEmail(staff.email);
        found = 0;
        if (aux.name != NULL)
        {
            printf("\nERROR: Email already exists. Please enter a different email.\n");
            free(staff.email);
            staff.email = NULL;
            STAFF_freeStaff(&aux);
            found = 1;
        }
    } while (staff.email == NULL || found);

    do
    {
        printf("\tEnter staff password: ");
        scanf("%ms", &staff.password);
        if (!GLOBAL_validatePassword(staff.password))
        {
            printf("\nERROR: Invalid password.\nPassword must be:\n- At least 8 characters long.\n- Contain at least one uppercase letter.\n- Contain at least one lowercase letter.\n- Contain at least one special character.\n");
            free(staff.password);
            staff.password = NULL;
        }
    } while (staff.password == NULL);

    Shop shop;
    do
    {
        printf("\tEnter shop code: ");
        scanf("%ms", &staff.shop_code);
        shop = SHOP_findShopByCode(staff.shop_code);
        if (shop.name == NULL)
        {
            printf("\nERROR: Shop code does not exist. Please enter a valid shop code.\n");
            free(staff.shop_code);
            staff.shop_code = NULL;
        }
    } while (staff.shop_code == NULL);
    SHOP_freeShop(&shop);

    char *buffer = NULL;
    asprintf(&buffer, "%s;%s;%s;%s", staff.name, staff.email, staff.password, staff.shop_code);
    GLOBAL_printLineInFile("files/staff.txt", buffer);
    free(buffer);

    STAFF_freeStaff(&staff);

    printf("\nStaff registered successfully!\n");
}

Product PRODUCT_findProductByCode(char *code, char *shop_code)
{
    FILE *file = fopen("files/products.txt", "r");
    if (!file)
    {
        return (Product){NULL, NULL, NULL, 0.0f, 0, NULL, NULL};
    }

    Product product = {NULL, NULL, NULL, 0.0f, 0, NULL, NULL};
    while (fscanf(file, " %m[^;];%m[^;];%m[^;];%f;%d;%m[^;];%ms", &product.code, &product.name, &product.category, &product.price, &product.quantity, &product.description, &product.shop_code) == 7)
    {
        if (strcmp(product.code, code) == 0 && strcmp(product.shop_code, shop_code) == 0)
        {
            fclose(file);
            return product;
        }
        PRODUCT_freeProduct(&product);
    }

    fclose(file);
    return (Product){NULL, NULL, NULL, 0.0f, 0, NULL, NULL};
}

void STAFF_addProduct(Staff staff)
{
    Product product;

    char *input = NULL;
    do
    {
        printf("\tEnter product code: ");
        scanf(" %ms", &input);
        product = PRODUCT_findProductByCode(input, staff.shop_code);
        if (product.code != NULL)
        {
            printf("\nERROR: Product code already exists. Please enter a different code.\n");
            PRODUCT_freeProduct(&product);
        }
        free(input);
        input = NULL;
    } while (product.code == NULL);

    printf("\tEnter product name: ");
    scanf(" %ms", &product.name);

    printf("\tEnter product category: ");
    scanf("%ms", &product.category);

    do
    {
        printf("\tEnter product price (€): ");
        scanf("%f", &product.price);
        if (product.price < 0)
        {
            printf("\nERROR: Invalid price. Price must be 0 or positive.\n");
        }
    } while (product.price < 0);

    do
    {
        printf("\tEnter product quantity: ");
        scanf("%d", &product.quantity);
        if (product.quantity < 0)
        {
            printf("\nERROR: Invalid quantity. Quantity must be 0 or positive.\n");
        }
    } while (product.quantity < 0);

    printf("\tEnter product description: ");
    scanf(" %m[^\n]", &product.description);

    product.shop_code = strdup(staff.shop_code);

    char *buffer = NULL;
    asprintf(&buffer, "%s;%s;%s;%.2f;%d;%s;%s", product.code, product.name, product.category, product.price, product.quantity, product.description, product.shop_code);
    GLOBAL_printLineInFile("files/products.txt", buffer);
    free(buffer);

    PRODUCT_freeProduct(&product);

    printf("\nProduct added successfully!\n");
}

void STAFF_updateProductInFile(char *old_name, Product new_product)
{
    FILE *file = fopen("files/products.txt", "r");
    if (!file)
    {
        printf("\nERROR: Could not open products file for updating.\n");
        return;
    }

    FILE *aux = fopen("files/temp_products.txt", "w");
    if (!aux)
    {
        fclose(file);
        printf("\nERROR: Could not create temporary file for updating products.\n");
        return;
    }

    Product product;
    while (fscanf(file, " %m[^;];%m[^;];%m[^;];%f;%d;%m[^;];%ms", &product.code, &product.name, &product.category, &product.price, &product.quantity, &product.description, &product.shop_code) == 7)
    {
        if (strcmp(product.name, old_name) == 0 && strcmp(product.shop_code, new_product.shop_code) == 0)
        {
            fprintf(aux, "%s;%s;%s;%.2f;%d;%s;%s\n", new_product.code, new_product.name, new_product.category, new_product.price, new_product.quantity, new_product.description, new_product.shop_code);
        }
        else
        {
            fprintf(aux, "%s;%s;%s;%.2f;%d;%s;%s\n", product.code, product.name, product.category, product.price, product.quantity, product.description, product.shop_code);
        }
        PRODUCT_freeProduct(&product);
    }

    fclose(file);
    fclose(aux);

    remove("files/products.txt");
    rename("files/temp_products.txt", "files/products.txt");

    printf("\nProduct updated successfully!\n");
}

void STAFF_updateProduct(Staff staff)
{
    Product product;

    printf("\tEnter product code to update: ");
    scanf("%ms", &product.code);
    product = PRODUCT_findProductByCode(product.code, staff.shop_code);
    if (product.code == NULL)
    {
        printf("\nERROR: Product not found.\n");
        free(product.code);
        product.code = NULL;
        return;
    }

    Product aux;
    printf("\tCurrent product name: %s\n", product.name);
    printf("\tEnter new product name: ");
    scanf("%ms", &aux.name);

    printf("\tCurrent product category: %s\n", product.category);
    printf("\tEnter new product category: ");
    scanf("%ms", &aux.category);

    printf("\tCurrent product price: %.2f\n", product.price);
    do
    {
        printf("\tEnter new product price (€): ");
        scanf("%f", &aux.price);
        if (aux.price < 0)
        {
            printf("\nERROR: Invalid price. Price must be 0 or positive.\n");
        }
    } while (aux.price < 0);

    printf("\tCurrent product quantity: %d\n", product.quantity);
    do
    {
        printf("\tEnter new product quantity: ");
        scanf("%d", &aux.quantity);
        if (aux.quantity < 0)
        {
            printf("\nERROR: Invalid quantity. Quantity must be 0 or positive.\n");
        }
    } while (aux.quantity < 0);

    printf("\tCurrent product description: %s\n", product.description);
    printf("\tEnter new product description: ");
    scanf(" %m[^\n]", &aux.description);

    aux.shop_code = strdup(product.shop_code);

    STAFF_updateProductInFile(product.name, aux);

    PRODUCT_freeProduct(&product);
    PRODUCT_freeProduct(&aux);
}

void STAFF_updateShopInFile(Shop shop)
{
    FILE *file = fopen("files/shops.txt", "r");
    if (!file)
    {
        printf("\nERROR: Could not open shops file for updating.\n");
        return;
    }

    FILE *aux = fopen("files/temp_shops.txt", "w");
    if (!aux)
    {
        fclose(file);
        printf("\nERROR: Could not create temporary file for updating shops.\n");
        return;
    }

    Shop current_shop;
    while (fscanf(file, " %m[^;];%m[^;];%m[^;];%m[^;];%m[^;];%f;%f", &current_shop.code, &current_shop.name, &current_shop.address, &current_shop.phone, &current_shop.email, &current_shop.latitude, &current_shop.longitude) == 7)
    {
        if (strcmp(current_shop.code, shop.code) == 0)
        {
            fprintf(aux, "%s;%s;%s;%s;%s;%.2f;%.2f\n", shop.code, shop.name, shop.address, shop.phone, shop.email, shop.latitude, shop.longitude);
        }
        else
        {
            fprintf(aux, "%s;%s;%s;%s;%s;%.2f;%.2f\n", current_shop.code, current_shop.name, current_shop.address, current_shop.phone, current_shop.email, current_shop.latitude, current_shop.longitude);
        }
        SHOP_freeShop(&current_shop);
    }

    fclose(file);
    fclose(aux);

    remove("files/shops.txt");
    rename("files/temp_shops.txt", "files/shops.txt");

    printf("\nShop updated successfully!\n");
}

void STAFF_updateShop(Staff staff)
{
    Shop shop = SHOP_findShopByCode(staff.shop_code);
    if (shop.name == NULL)
    {
        printf("\nERROR: Shop not found.\n");
        return;
    }

    Shop aux;

    printf("\tCurrent shop name: %s\n", shop.name);
    printf("\tEnter new shop name: ");
    scanf("%ms", &aux.name);

    printf("\tCurrent shop address: %s\n", shop.address);
    printf("\tEnter new shop address: ");
    scanf(" %m[^\n]", &aux.address);

    printf("\tCurrent shop phone: %s\n", shop.phone);
    printf("\tEnter new shop phone: ");
    scanf("%ms", &aux.phone);

    printf("\tCurrent shop email: %s\n", shop.email);
    do
    {
        printf("\tEnter new shop email: ");
        scanf("%ms", &aux.email);
        if (!GLOBAL_validateEmail(aux.email))
        {
            printf("\nERROR: Invalid email. Please enter a valid email.\n");
            free(aux.email);
            aux.email = NULL;
        }
    } while (aux.email == NULL);

    printf("\tCurrent shop latitude: %.2f\n", shop.latitude);
    do
    {
        printf("\tEnter new shop latitude (-90.0 to 90.0): ");
        scanf("%f", &aux.latitude);
        if (aux.latitude < -90.0f || aux.latitude > 90.0f)
        {
            printf("\nERROR: Latitude must be between -90.0 and 90.0. Please enter a valid latitude.\n");
        }
    } while (aux.latitude < -90.0f || aux.latitude > 90.0f);

    printf("\tCurrent shop longitude: %.2f\n", shop.longitude);
    do
    {
        printf("\tEnter new shop longitude (-180.0 to 180.0): ");
        scanf("%f", &aux.longitude);
        if (aux.longitude < -180.0f || aux.longitude > 180.0f)
        {
            printf("\nERROR: Longitude must be between -180.0 and 180.0. Please enter a valid longitude.\n");
        }
    } while (aux.longitude < -180.0f || aux.longitude > 180.0f);

    aux.code = strdup(shop.code);

    STAFF_updateShopInFile(aux);

    SHOP_freeShop(&shop);
    SHOP_freeShop(&aux);
}

Discount DISCOUNT_findDiscountByCode(char *discount_code, char *shop_code)
{
    FILE *file = fopen("files/discounts.txt", "r");
    if (!file)
    {
        return (Discount){NULL, NULL, NULL, NULL, NULL};
    }

    Discount discount = {NULL, NULL, NULL, NULL, NULL};
    while (fscanf(file, " %m[^;];%m[^;];%m[^;];%m[^;];%ms", &discount.discount_code, &discount.product_code, &discount.shop_code, &discount.start_date, &discount.end_date) == 5)
    {
        if (strcmp(discount.discount_code, discount_code) == 0 && strcmp(discount.shop_code, shop_code) == 0)
        {
            fclose(file);
            return discount;
        }
        DISCOUNT_freeDiscount(&discount);
    }

    fclose(file);
    return (Discount){NULL, NULL, NULL, NULL, NULL};
}

void STAFF_addDiscount(Staff staff)
{
    Discount discount;
    char *input = NULL;
    do
    {
        printf("\tEnter discount code: ");
        scanf("%ms", &input);
        discount = DISCOUNT_findDiscountByCode(input, staff.shop_code);
        if (discount.discount_code != NULL)
        {
            printf("\nERROR: Discount code already exists. Please enter a different code.\n");
            free(discount.discount_code);
            discount.discount_code = NULL;
        }
        else
        {
            discount.discount_code = strdup(input);
            free(input);
            input = NULL;
            break;
        }
        free(input);
        input = NULL;
    } while (1);

    char **product_codes = NULL;
    int num_products = 0;
    int exit = 0;
    Product product = {NULL, NULL, NULL, 0.0f, 0, NULL, NULL};
    printf("\tEnter product codes for discount (type EXIT to finish):\n");
    while (1)
    {
        do
        {
            printf("\tEnter product code for discount: ");
            scanf("%ms", &input);
            if (strcmp(input, "EXIT") == 0)
            {
                free(input);
                input = NULL;
                exit = 1;
                break;
            }

            product = PRODUCT_findProductByCode(input, staff.shop_code);
            if (product.code == NULL)
            {
                printf("\nERROR: Product does not exists. Please enter a different code.\n");
            }
        } while (product.code == NULL);

        if (exit)
            break;

        PRODUCT_freeProduct(&product);

        product_codes = realloc(product_codes, sizeof(char *) * (num_products + 1));
        product_codes[num_products] = strdup(input);
        num_products++;
        free(input);
        input = NULL;
    }

    printf("\tEnter start date (YYYY-MM-DD): ");
    scanf("%ms", &discount.start_date);

    printf("\tEnter end date (YYYY-MM-DD): ");
    scanf("%ms", &discount.end_date);

    discount.shop_code = strdup(staff.shop_code);

    // Guardar información en el archivo
    for(int i = 0; i < num_products; i++)
    {
        char *buffer = NULL;
        asprintf(&buffer, "%s;%s;%s;%s;%s", discount.discount_code, product_codes[i], discount.shop_code, discount.start_date, discount.end_date);
        GLOBAL_printLineInFile("files/discounts.txt", buffer);
        free(buffer);
    }

    DISCOUNT_freeDiscount(&discount);
    for (int i = 0; i < num_products; i++)
    {
        free(product_codes[i]);
        product_codes[i] = NULL;
    }
    free(product_codes);
    product_codes = NULL;

    printf("\nDiscount added successfully!\n");
}

void DISCOUNT_updateDiscountInFile(Discount old, char *new_code)
{
    FILE *file = fopen("files/discounts.txt", "r");
    if (!file)
    {
        printf("\nERROR: Could not open discounts file for updating.\n");
        return;
    }

    FILE *aux = fopen("files/temp_discounts.txt", "w");
    if (!aux)
    {
        fclose(file);
        printf("\nERROR: Could not create temporary file for updating discounts.\n");
        return;
    }

    Discount discount;
    while (fscanf(file, " %m[^;];%m[^;];%m[^;];%m[^;];%ms", &discount.discount_code, &discount.product_code, &discount.shop_code, &discount.start_date, &discount.end_date) == 5)
    {
        if (strcmp(discount.discount_code, old.discount_code) == 0 && strcmp(discount.shop_code, old.shop_code) == 0)
        {
            fprintf(aux, "%s;%s;%s;%s;%s\n", new_code, discount.product_code, discount.shop_code, discount.start_date, discount.end_date);
        }
        else
        {
            fprintf(aux, "%s;%s;%s;%s;%s\n", discount.discount_code, discount.product_code, discount.shop_code, discount.start_date, discount.end_date);
        }
        DISCOUNT_freeDiscount(&discount);
    }

    fclose(file);
    fclose(aux);
    remove("files/discounts.txt");
    rename("files/temp_discounts.txt", "files/discounts.txt");

    printf("\nDiscount updated successfully!\n");
}

void STAFF_editDiscount(Staff staff)
{
    char *input = NULL;
    Discount discount;
    do
    {
        printf("\tEnter discount code to edit: ");
        scanf("%ms", &input);
        discount = DISCOUNT_findDiscountByCode(input, staff.shop_code);
        if (discount.discount_code == NULL)
        {
            printf("\nERROR: Discount not found.\n");
        }
        free(input);
        input = NULL;
    } while (discount.discount_code == NULL);

    Discount aux;
    do
    {
        printf("\tEnter new discount code: ");
        scanf("%ms", &input);
        aux = DISCOUNT_findDiscountByCode(input, staff.shop_code);
        if (aux.discount_code != NULL)
        {
            printf("\nERROR: Discount code already exists. Please enter a different code.\n");
            DISCOUNT_freeDiscount(&aux);
        }
        else
        {
            break;
        }
        free(input);
        input = NULL;
    } while (1);

    DISCOUNT_updateDiscountInFile(discount, input);
    free(input);
    input = NULL;
    DISCOUNT_freeDiscount(&discount);
}

void DISCOUNT_deleteDiscountInFile(Discount discount)
{
    FILE *file = fopen("files/discounts.txt", "r");
    if (!file)
    {
        printf("\nERROR: Could not open discounts file for deleting.\n");
        return;
    }

    FILE *aux = fopen("files/temp_discounts.txt", "w");
    if (!aux)
    {
        fclose(file);
        printf("\nERROR: Could not create temporary file for deleting discounts.\n");
        return;
    }

    Discount current_discount;
    while (fscanf(file, " %m[^;];%m[^;];%m[^;];%m[^;];%ms", &current_discount.discount_code, &current_discount.product_code, &current_discount.shop_code, &current_discount.start_date, &current_discount.end_date) == 5)
    {
        if (strcmp(current_discount.discount_code, discount.discount_code) != 0 || strcmp(current_discount.shop_code, discount.shop_code) != 0)
        {
            fprintf(aux, "%s;%s;%s;%s;%s\n", current_discount.discount_code, current_discount.product_code, current_discount.shop_code, current_discount.start_date, current_discount.end_date);
        }
        DISCOUNT_freeDiscount(&current_discount);
    }

    fclose(file);
    fclose(aux);
    remove("files/discounts.txt");
    rename("files/temp_discounts.txt", "files/discounts.txt");

    printf("\nDiscount deleted successfully!\n");
}

void STAFF_deleteDiscount(Staff staff)
{
    char *input = NULL;
    Discount discount;
    do
    {
        printf("\tEnter discount code to delete: ");
        scanf("%ms", &input);
        discount = DISCOUNT_findDiscountByCode(input, staff.shop_code);
        if (discount.discount_code == NULL)
        {
            printf("\nERROR: Discount not found.\n");
        }
        free(input);
        input = NULL;
    } while (discount.discount_code == NULL);

    DISCOUNT_deleteDiscountInFile(discount);
    DISCOUNT_freeDiscount(&discount);
}

void STAFF_menu(Staff staff)
{
    int option = 0;
    while (option != 7)
    {
        printf("\t1. Add Product\n");
        printf("\t2. Update Product\n");
        printf("\t3. Update Shop\n");
        printf("\t4. Add discount\n");
        printf("\t5. Edit discount\n");
        printf("\t6. Delete discount\n");
        printf("\t7. Logout\n");
        printf("Option: ");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            printf("\nADD PRODUCT\n");
            STAFF_addProduct(staff);
            break;
        case 2:
            printf("\nUPDATE PRODUCT\n");
            STAFF_updateProduct(staff);
            break;
        case 3:
            printf("\nUPDATE SHOP\n");
            STAFF_updateShop(staff);
            break;
        case 4:
            printf("\nADD DISCOUNT\n");
            STAFF_addDiscount(staff);
            break;
        case 5:
            printf("\nEDIT DISCOUNT\n");
            STAFF_editDiscount(staff);
            break;
        case 6:
            printf("\nDELETE DISCOUNT\n");
            STAFF_deleteDiscount(staff);
            break;
        case 7:
            printf("\nLogging out\n\n");
            break;
        default:
            printf("\nERROR: Invalid option.\n");
            break;
        }
    }
}

void STAFF_login()
{
    char *email = NULL;
    char *password = NULL;
    Staff staff;
    int found = 0;

    do
    {
        printf("\tEmail: ");
        scanf("%ms", &email);

        printf("\tPassword: ");
        scanf("%ms", &password);

        staff = STAFF_findStaffByEmail(email);

        if (staff.name == NULL)
        {
            printf("\nERROR: Invalid email or password. Please try again.\n");
            STAFF_freeStaff(&staff);
        }
        else if (strcmp(staff.password, password) != 0)
        {
            printf("\nERROR: Invalid email or password. Please try again.\n");
            STAFF_freeStaff(&staff);
        }
        else
        {
            found = 1;
            printf("\nLogin successful! Welcome, %s.\n", staff.name);
        }

        free(email);
        free(password);
        email = NULL;
        password = NULL;

    } while (!found);

    STAFF_menu(staff);

    STAFF_freeStaff(&staff);
}