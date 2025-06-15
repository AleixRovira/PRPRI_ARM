#define _GNU_SOURCE

#include "../lib/staff.h"

void STAFF_freeStaff(Staff *staff)
{
    free(staff->name);
    free(staff->email);
    free(staff->password);
    free(staff->shop_code);

    // Optional: set pointers to NULL to avoid double free
    staff->name = NULL;
    staff->email = NULL;
    staff->password = NULL;
    staff->shop_code = NULL;
}

void PRODUCT_freeProduct(Product *product)
{
    free(product->name);
    free(product->category);
    free(product->description);
    free(product->shop_code);

    // Optional: set pointers to NULL to avoid double free
    product->name = NULL;
    product->category = NULL;
    product->description = NULL;
    product->shop_code = NULL;
    product->price = 0.0f;
    product->quantity = 0;
}

Staff STAFF_findStaffByEmail(char *email)
{
    FILE *file = fopen("files/staff.txt", "r");
    if (!file)
    {
        return (Staff){NULL, NULL, NULL, NULL};
    }

    Staff staff = {NULL, NULL, NULL, NULL};
    while (fscanf(file, "%m[^;];%m[^;];%m[^;];%ms", &staff.name, &staff.email, &staff.password, &staff.shop_code) == 4)
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

Product PRODUCT_findProductByName(char *name, char *shop_code)
{
    FILE *file = fopen("files/products.txt", "r");
    if (!file)
    {
        return (Product){NULL, NULL, 0.0f, 0, NULL, NULL};
    }

    Product product = {NULL, NULL, 0.0f, 0, NULL, NULL};
    while (fscanf(file, "%m[^;];%m[^;];%f;%d;%m[^;];%ms", &product.name, &product.category, &product.price, &product.quantity, &product.description, &product.shop_code) == 6)
    {
        if (strcmp(product.name, name) == 0 && strcmp(product.shop_code, shop_code) == 0)
        {
            fclose(file);
            return product;
        }
        PRODUCT_freeProduct(&product);
    }

    fclose(file);
    return (Product){NULL, NULL, 0.0f, 0, NULL, NULL};
}

void STAFF_addProduct(Staff staff)
{
    Product product;

    do
    {
        printf("\tEnter product name: ");
        scanf(" %ms", &product.name);
        product = PRODUCT_findProductByName(product.name, staff.shop_code);
        if (product.name != NULL)
        {
            printf("\nERROR: Product name already exists. Please enter a different name.\n");
            free(product.name);
            product.name = NULL;
        }
    } while (product.name == NULL);

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
    asprintf(&buffer, "%s;%s;%.2f;%d;%s;%s", product.name, product.category, product.price, product.quantity, product.description, product.shop_code);
    GLOBAL_printLineInFile("files/products.txt", buffer);
    free(buffer);

    PRODUCT_freeProduct(&product);

    printf("\nProduct added successfully!\n");
}

void PRODUCT_updateProductInFile(char *old_name, Product new_product)
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
    while (fscanf(file, "%m[^;];%m[^;];%f;%d;%m[^;];%ms", &product.name, &product.category, &product.price, &product.quantity, &product.description, &product.shop_code) == 6)
    {
        if (strcmp(product.name, old_name) == 0 && strcmp(product.shop_code, new_product.shop_code) == 0)
        {
            fprintf(aux, "%s;%s;%.2f;%d;%s;%s\n", new_product.name, new_product.category, new_product.price, new_product.quantity, new_product.description, new_product.shop_code);
        }
        else
        {
            fprintf(aux, "%s;%s;%.2f;%d;%s;%s\n", product.name, product.category, product.price, product.quantity, product.description, product.shop_code);
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

    printf("\tEnter product name to update: ");
    scanf("%ms", &product.name);
    product = PRODUCT_findProductByName(product.name, staff.shop_code);
    if (product.name == NULL)
    {
        printf("\nERROR: Product not found.\n");
        free(product.name);
        product.name = NULL;
        return;
    }

    Product aux;
    printf("\tEnter new product name: ");
    scanf("%ms", &aux.name);

    printf("\tEnter new product category: ");
    scanf("%ms", &aux.category);

    do
    {
        printf("\tEnter new product price (€): ");
        scanf("%f", &aux.price);
        if (aux.price < 0)
        {
            printf("\nERROR: Invalid price. Price must be 0 or positive.\n");
        }
    } while (aux.price < 0);

    do
    {
        printf("\tEnter new product quantity: ");
        scanf("%d", &aux.quantity);
        if (aux.quantity < 0)
        {
            printf("\nERROR: Invalid quantity. Quantity must be 0 or positive.\n");
        }
    } while (aux.quantity < 0);

    printf("\tEnter new product description: ");
    scanf(" %m[^\n]", &aux.description);

    aux.shop_code = strdup(product.shop_code);

    PRODUCT_updateProductInFile(product.name, aux);

    PRODUCT_freeProduct(&product);
    PRODUCT_freeProduct(&aux);
}

void STAFF_menu(Staff staff)
{
    int option = 0;
    while (option != 4)
    {
        printf("\t1. Add Product\n");
        printf("\t2. Update Product\n");
        printf("\t3. Update Shop\n");
        printf("\t4. Logout\n");
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
            // Placeholder for shop update functionality
            break;
        case 4:
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