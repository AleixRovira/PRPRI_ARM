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
            free(input);
            input = NULL;
        }
    } while (product.code != NULL);
    product.code = strdup(input);
    free(input);
    input = NULL;

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

void STAFF_updateProductInFile(char *old_code, Product new_product)
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
        if (strcmp(product.code, old_code) == 0 && strcmp(product.shop_code, new_product.shop_code) == 0)
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
    char *input = NULL;

    printf("\tEnter product code to update: ");
    scanf("%ms", &input);
    product = PRODUCT_findProductByCode(input, staff.shop_code);
    if (product.code == NULL)
    {
        printf("\nERROR: Product not found.\n");
        free(product.code);
        product.code = NULL;
        return;
    }
    free(input);
    input = NULL;

    Product aux = {NULL, NULL, NULL, 0.0f, 0, NULL, NULL};
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
    aux.code = strdup(product.code);

    STAFF_updateProductInFile(product.code, aux);

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
    for (int i = 0; i < num_products; i++)
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

Product *STAFF_getProductsByShop(char *shop_code, int *count)
{
    FILE *file = fopen("files/products.txt", "r");
    if (!file)
    {
        return NULL;
    }

    Product *products = NULL;

    Product product;
    while (fscanf(file, " %m[^;];%m[^;];%m[^;];%f;%d;%m[^;];%ms", &product.code, &product.name, &product.category, &product.price, &product.quantity, &product.description, &product.shop_code) == 7)
    {
        if (strcmp(product.shop_code, shop_code) == 0)
        {
            products = realloc(products, sizeof(Product) * (*count + 1));
            products[*count] = product;
            (*count)++;
        }
        else
        {
            PRODUCT_freeProduct(&product);
        }
    }

    fclose(file);
    return products;
}

void STAFF_viewStock(Staff staff)
{
    int count = 0;
    Product *products = STAFF_getProductsByShop(staff.shop_code, &count);
    if (products == NULL)
    {
        printf("\nNo products found for this shop\n");
        return;
    }

    printf("\nSTOCK:\n");
    for (int i = 0; i < count; i++)
    {
        if (products[i].quantity > 0)
        {
            printf("\tCode: %s\n", products[i].code);
            printf("\tName: %s\n", products[i].name);
            printf("\tQuantity: %d\n", products[i].quantity);
            printf("\tCategory: %s\n", products[i].category);
            printf("\tPrice: %.2f €\n", products[i].price);
            printf("\tDescription: %s\n\n", products[i].description);
        }
    }

    printf("OUT OF STOCK:\n");
    for (int i = 0; i < count; i++)
    {
        if (products[i].quantity <= 0)
        {
            printf("\tCode: %s\n", products[i].code);
            printf("\tName: %s\n", products[i].name);
            printf("\tQuantity: %d\n", products[i].quantity);
            printf("\tCategory: %s\n", products[i].category);
            printf("\tPrice: %.2f €\n", products[i].price);
            printf("\tDescription: %s\n\n", products[i].description);
        }
        PRODUCT_freeProduct(&products[i]);
    }
    free(products);
}

char STAFF_checkIfFileExists(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void STAFF_createOrder(char *code, char *shop_code, char **product_codes, int *quantities, int count)
{
    char *buffer = NULL;
    asprintf(&buffer, "files/orders/%s%s.txt", code, shop_code);
    FILE *file = fopen(buffer, "a");
    if (!file)
    {
        printf("\nERROR: Could not open orders file for writing.\n");
        return;
    }
    free(buffer);

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s;%d\n", product_codes[i], quantities[i]);
        free(product_codes[i]);
    }
    free(product_codes);
    free(quantities);

    fclose(file);
    printf("\nOrder placed successfully!\n");
}

void STAFF_placeOrder(Staff staff)
{
    int count = 0;
    char **product_codes = NULL;
    int *quantities = NULL;
    char *input = NULL;
    char *code = NULL;
    int exist = 0;
    char *buffer = NULL;

    do
    {
        printf("\n\tEnter order code: ");
        scanf("%ms", &code);
        buffer = NULL;
        asprintf(&buffer, "files/orders/%s%s.txt", code, staff.shop_code);
        exist = STAFF_checkIfFileExists(buffer);
        free(buffer);
        if (exist)
        {
            printf("\nERROR: Order code already exists. Please enter a different code.\n");
            free(code);
            code = NULL;
        }
    } while (exist);

    printf("\nWhat products would you like to order?\n");
    while (1)
    {
        printf("\tEnter product code (or type EXIT to finish): ");
        scanf("%ms", &input);
        if (strcmp(input, "EXIT") == 0)
        {
            free(input);
            input = NULL;
            break;
        }

        Product product = PRODUCT_findProductByCode(input, staff.shop_code);
        if (product.code == NULL)
        {
            printf("\nERROR: Product not found. Please enter a valid product code.\n");
        }
        else
        {
            product_codes = realloc(product_codes, sizeof(char *) * (count + 1));
            product_codes[count] = strdup(input);
            quantities = realloc(quantities, sizeof(int) * (count + 1));
            free(input);
            input = NULL;
            do
            {
                printf("\tEnter quantity for product %s: ", product_codes[count]);
                scanf("%d", &quantities[count]);
                if (quantities[count] <= 0)
                {
                    printf("\nERROR: Invalid quantity. Quantity must be greater than 0.\n");
                }
            } while (quantities[count] <= 0);
            count++;
            PRODUCT_freeProduct(&product);
        }
    }

    if (count == 0)
    {
        printf("\nERROR: No products selected for the order.\n");
        free(product_codes);
        free(quantities);
        free(code);
        return;
    }

    STAFF_createOrder(code, staff.shop_code, product_codes, quantities, count);
    free(code);
}

void STAFF_updateProductQuantityInFile(char *product_code, int quantity, char *shop_code)
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
        if (strcmp(product.code, product_code) == 0 && strcmp(product.shop_code, shop_code) == 0)
        {
            product.quantity += quantity;
            fprintf(aux, "%s;%s;%s;%.2f;%d;%s;%s\n", product.code, product.name, product.category, product.price, product.quantity, product.description, product.shop_code);
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

    printf("\nProduct quantity updated successfully!\n");
}

void STAFF_receiveProducts(char *filename, Staff staff)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("\nERROR: Could not open order file for reading.\n");
        return;
    }

    char *product_code = NULL;
    int quantity = 0;
    while (fscanf(file, " %m[^;];%d", &product_code, &quantity) == 2)
    {
        STAFF_updateProductQuantityInFile(product_code, quantity, staff.shop_code);
        printf("Received %d of product %s\n", quantity, product_code);
        free(product_code);
        product_code = NULL;
    }

    fclose(file);
}

void STAFF_receiveOrder(Staff staff)
{
    char *order_code = NULL;
    char *buffer = NULL;
    int exist = 0;

    do
    {
        printf("\n\tEnter order code to receive: ");
        scanf("%ms", &order_code);
        buffer = NULL;
        asprintf(&buffer, "files/orders/%s%s.txt", order_code, staff.shop_code);
        exist = STAFF_checkIfFileExists(buffer);

        if (!exist)
        {
            printf("\nERROR: Order code does not exist. Please enter a valid order code.\n");
            free(order_code);
            order_code = NULL;
            free(buffer);
        }
    } while (!exist);

    STAFF_receiveProducts(buffer, staff);
    if (remove(buffer) != 0)
    {
        printf("\nERROR: Could not delete order file after processing.\n");
    }
    free(buffer);
    buffer = NULL;

    printf("\nOrder %s received successfully!\n", order_code);

    free(order_code);
    order_code = NULL;
}

Product *STAFF_getAllProducts(int *count)
{
    FILE *file = fopen("files/products.txt", "r");
    if (!file)
    {
        return NULL;
    }

    Product *products = NULL;
    Product product;

    while (fscanf(file, " %m[^;];%m[^;];%m[^;];%f;%d;%m[^;];%ms", &product.code, &product.name, &product.category, &product.price, &product.quantity, &product.description, &product.shop_code) == 7)
    {
        products = realloc(products, sizeof(Product) * (*count + 1));
        products[*count] = product;
        (*count)++;
    }

    fclose(file);
    return products;
}

void STAFF_showFilteredStock(int option, char *filter, float min_price, float max_price, int min_quantity, int max_quantity)
{
    int count = 0;
    Product *products = STAFF_getAllProducts(&count);
    if (products == NULL)
    {
        printf("\nNo products found for this shop\n");
        return;
    }

    int found = 0;
    printf("\nFILTERED STOCK:\n");
    for (int i = 0; i < count; i++)
    {
        int show_product = 1;

        if (option == 1 && filter != NULL && !GLOBAL_containsIgnoreCase(products[i].name, filter))
            show_product = 0;
        else if (option == 2 && filter != NULL && !GLOBAL_containsIgnoreCase(products[i].category, filter))
            show_product = 0;
        else if (option == 3 && !OP_isInRange(products[i].price, min_price, max_price, 1))
            show_product = 0;
        else if (option == 4 && !OP_isInRange(products[i].quantity, min_quantity, max_quantity, 1))
            show_product = 0;
        else if (option == 5 && filter != NULL && strcasecmp(products[i].shop_code, filter) != 0)
            show_product = 0;

        if (show_product)
        {
            found = 1;
            printf("\tCode: %s\n", products[i].code);
            printf("\tName: %s\n", products[i].name);
            printf("\tQuantity: %d\n", products[i].quantity);
            printf("\tCategory: %s\n", products[i].category);
            printf("\tPrice: %.2f €\n", products[i].price);
            printf("\tDescription: %s\n\n", products[i].description);
        }
        PRODUCT_freeProduct(&products[i]);
    }
    if (!found)
    {
        printf("\nNo products found matching the filter criteria.\n");
    }
    free(products);
    products = NULL;
}

void STAFF_filterStock()
{
    int option = 0;

    printf("\nFILTER STOCK\n");
    while (option < 1 || option > 6)
    {
        printf("\t1. Filter by Name\n");
        printf("\t2. Filter by Category\n");
        printf("\t3. Filter by Price Range\n");
        printf("\t4. Filter by Quantity Range\n");
        printf("\t5. Filter by Shop\n");
        printf("\t6. Exit\n");
        printf("Option: ");
        scanf("%d", &option);

        if (option < 1 || option > 6)
        {
            printf("\nERROR: Invalid option. Please try again.\n");
        }
        else
        {
            break;
        }
    }

    if (option == 6)
    {
        printf("\nExiting filter stock menu.\n");
        return;
    }
    else
    {
        char *filter = NULL;
        float min_price = 0.0f, max_price = 0.0f;
        int min_quantity = 0, max_quantity = 0;
        switch (option)
        {
        case 1:
            printf("\tEnter product name to filter: ");
            scanf(" %m[^\n]", &filter);
            break;
        case 2:
            printf("\tEnter product category to filter: ");
            scanf(" %m[^\n]", &filter);
            break;
        case 3:
            printf("\tEnter minimum price: ");
            scanf("%f", &min_price);
            printf("\tEnter maximum price: ");
            scanf("%f", &max_price);
            break;
        case 4:
            printf("\tEnter minimum quantity: ");
            scanf("%d", &min_quantity);
            printf("\tEnter maximum quantity: ");
            scanf("%d", &max_quantity);
            break;
        case 5:
            printf("\tEnter shop code to filter: ");
            scanf(" %ms", &filter);
            break;
        }
        STAFF_showFilteredStock(option, filter, min_price, max_price, min_quantity, max_quantity);
        if (filter != NULL)
        {
            free(filter);
            filter = NULL;
        }
    }

    printf("\n");
}

void STAFF_calculateStockValue()
{
    int option = 0;
    int count = 0;
    Product *products = NULL;

    while (option < 1 || option > 3)
    {
        printf("\t1. Calculate total stock value for all shops\n");
        printf("\t2. Calculate total stock value for a specific shop\n");
        printf("\t3. Exit\n");
        printf("Option: ");
        scanf("%d", &option);

        if (option < 1 || option > 3)
        {
            printf("\nERROR: Invalid option. Please try again.\n");
        }
    }

    switch (option)
    {
    case 1:
        products = STAFF_getAllProducts(&count);
        if (products == NULL || count == 0)
        {
            printf("\nNo products found.\n");
            return;
        }
        break;

    case 2:
        char *shop_code = NULL;
        printf("\tEnter shop code: ");
        scanf(" %ms", &shop_code);
        products = STAFF_getProductsByShop(shop_code, &count);
        if (products == NULL || count == 0)
        {
            printf("\nNo products found for this shop.\n");
            free(shop_code);
            return;
        }
        break;
    }

    float total_value = 0.0f;
    for (int i = 0; i < count; i++)
    {
        total_value += products[i].price * products[i].quantity;
        PRODUCT_freeProduct(&products[i]);
    }
    free(products);
    products = NULL;

    printf("\nTotal stock value: %.2f €\n", total_value);
}

void STAFF_menu(Staff staff)
{
    int option = 0;
    while (option != 12)
    {
        printf("\t1. Add Product\n");
        printf("\t2. Update Product\n");
        printf("\t3. Update Shop\n");
        printf("\t4. Add discount\n");
        printf("\t5. Edit discount\n");
        printf("\t6. Delete discount\n");
        printf("\t7. View stock\n");
        printf("\t8. Place an order\n");
        printf("\t9. Receive order\n");
        printf("\t10. Filter stock\n");
        printf("\t11. Calculate total stock value\n");
        printf("\t12. Logout\n");
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
            printf("\nVIEW STOCK\n");
            STAFF_viewStock(staff);
            break;
        case 8:
            printf("\nPLACE AN ORDER\n");
            STAFF_placeOrder(staff);
            break;
        case 9:
            printf("\nRECEIVE ORDER\n");
            STAFF_receiveOrder(staff);
            break;
        case 10:
            printf("\nFILTER STOCK\n");
            STAFF_filterStock();
            break;
        case 11:
            printf("\nCALCULATE TOTAL STOCK VALUE\n");
            STAFF_calculateStockValue();
            break;
        case 12:
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