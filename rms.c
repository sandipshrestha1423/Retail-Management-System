#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product
{
    int id;
    char product_name[50];
    int quantity;
    float price;
} prod;

struct Transaction
{
    char product_name[50];
    int quantity_sold;
    float total_earned;
} trans;

void gap()
{
    printf("\n\t\t\t\t\t");
}

void yes_no();
void add_product();
void show_product();
void search_product();
void delete_product();
void update_stock(int sold_quantity, char product_name[50]);
int menu();
void authenticate();

void yes_no()
{
    char choice;
    printf("Do you want to continue? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y')
    {
        menu();
    }
    else
    {
        printf("Thank you for using the program.\n");
    }
}

void add_product()
{
    FILE *product;
    product = fopen("products.txt", "a+");

    int max_id = 0;
    while (fscanf(product, "%d %s %d %f", &prod.id, prod.product_name, &prod.quantity, &prod.price) == 4)
    {
        if (max_id < prod.id)
        {
            max_id = prod.id;
        }
    }
    prod.id = max_id + 1;
    printf("--------------------------------\n");
    printf("Enter the product name: ");
    scanf("%s", prod.product_name);
    printf("Enter the quantity: ");
    scanf("%d", &prod.quantity);
    printf("Enter the price: ");
    scanf("%f", &prod.price);
    fprintf(product, "%d %s %d %.2f\n", prod.id, prod.product_name, prod.quantity, prod.price);
    fclose(product);
    printf("Product added successfully.\n\n");
    yes_no();
}

void show_product()
{
    FILE *product;
    product = fopen("products.txt", "r");
    printf("--------------------------------\n");
    printf("Product List:\n");
    while (fscanf(product, "%d %s %d %f", &prod.id, prod.product_name, &prod.quantity, &prod.price) == 4)
    {
        printf("ID: %d\n", prod.id);
        printf("Product Name: %s\n", prod.product_name);
        printf("Quantity: %d\n", prod.quantity);
        printf("Price: %.2f\n", prod.price);
        printf("--------------------------\n\n");
    }

    fclose(product);
    printf("End of product list.\n\n");
    yes_no();
}

void search_product()
{
    FILE *product;
    char search[50];
    int found = 0;
    product = fopen("products.txt", "r");
    printf("\nEnter Product Name: ");
    scanf("%s", search);

    while (fscanf(product, "%d %s %d %f", &prod.id, prod.product_name, &prod.quantity, &prod.price) == 4)
    {
        if (strcasecmp(prod.product_name, search) == 0)
        {
            printf("--------------------------\n");
            printf("Product Found:\n");
            printf("ID: %d\n", prod.id);
            printf("Product Name: %s\n", prod.product_name);
            printf("Quantity: %d\n", prod.quantity);
            printf("Price: %.2f\n", prod.price);
            printf("--------------------------\n\n");
            found = 1;
            break;
        }
    }
    fclose(product);
    if (found == 0)
    {
        printf("Product not found.\n\n");
    }
    yes_no();
}

void delete_product()
{
    FILE *product = fopen("products.txt", "r");
    FILE *temp = fopen("temp.txt", "a");
    char delete_name[50];
    int found = 0;
    printf("Enter the product you want to delete: ");
    scanf("%s", delete_name);
    int current_id = 1;
    while (fscanf(product, "%d %s %d %f", &prod.id, prod.product_name, &prod.quantity, &prod.price) == 4)
    {
        if (strcasecmp(delete_name, prod.product_name) == 0)
        {
            printf("DATA FOUND. DELETING...\n\n");
            found = 1;
        }
        else
        {
            fprintf(temp, "%d %s %d %.2f\n", current_id, prod.product_name, prod.quantity, prod.price);
            current_id++;
        }
    }
    fclose(product);
    fclose(temp);
    remove("products.txt");
    rename("temp.txt", "products.txt");

    yes_no();
}
void update_stock(int sold_quantity, char product_name[50])
{
    FILE *product = fopen("products.txt", "r");
    FILE *temporary = fopen("temporary.txt", "w");

    if (product == NULL || temporary == NULL)
    {
        printf("Error: Unable to open the file.\n");
        return;
    }

    while (fscanf(product, "%d %s %d %f", &prod.id, prod.product_name, &prod.quantity, &prod.price) == 4)
    {
        if (strcasecmp(prod.product_name, product_name) == 0)
        {
            prod.quantity = prod.quantity - sold_quantity;
        }
        fprintf(temporary, "%d %s %d %.2f\n", prod.id, prod.product_name, prod.quantity, prod.price);
    }

    fclose(product);
    fclose(temporary);

    product = fopen("products.txt", "w");
    temporary = fopen("temporary.txt", "r");

    if (product == NULL || temporary == NULL)
    {
        printf("Error: Unable to overwrite the file.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), temporary))
    {
        fputs(line, product);
    }

    fclose(product);
    fclose(temporary);
    remove("temporary.txt");
}

void save_transaction(char product_name[50], int quantity_sold, float total_earned)
{
    strcpy(trans.product_name, product_name);
    trans.quantity_sold = quantity_sold;
    trans.total_earned = total_earned;

    FILE *transaction_file = fopen("transactions.txt", "a");
    if (transaction_file == NULL)
    {
        printf("Error: Unable to open the transaction file.\n");
        return;
    }

    fprintf(transaction_file, "%s %d %.2f\n", trans.product_name, trans.quantity_sold, trans.total_earned);
    fclose(transaction_file);
}

void show_transaction()
{
    FILE *transaction_file = fopen("transactions.txt", "r");
    while (fscanf(transaction_file, "%s %d %f", trans.product_name, &trans.quantity_sold, &trans.total_earned) == 3)
    {
        printf("\n--------------------------\n");
        printf("Transaction Details:\n");
        printf("Product Name: %s\n", trans.product_name);
        printf("Quantity Sold: %d\n", trans.quantity_sold);
        printf("Total Earned: %.2f\n", trans.total_earned);
        printf("--------------------------\n");
    }
    yes_no();
}

int menu()
{
    gap();
    int choice;
    printf("Welcome to Retail Management System");
    gap();
    printf("-----------------------------------\n");
    printf("\n\n");
    printf("1. Add Product\n");
    printf("2. Show Product\n");
    printf("3. Search Product\n");
    printf("4. Delete Product\n");
    printf("5. Show Transaction\n");
    printf("6. Login\n\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        add_product();
    }
    if (choice == 2)
    {
        show_product();
    }
    if (choice == 3)
    {
        search_product();
    }
    if (choice == 4)
    {
        delete_product();
    }
    if (choice == 5)
    {
        show_transaction();
    }
    if (choice == 6)
    {
        authenticate();
    }
    return 0;
}

int cashier()
{
    FILE *product;

    char product_name[50], choice;
    int order;
    float bill = 0.0;

    gap();
    printf("  Whaiyat Ullu Swat Store");
    gap();
    printf("Welcome to the Cashier Menu!\n");
    printf("----------------------------\n");

    do
    {
        int found = 0;
        printf("Enter the product: ");
        scanf("%s", product_name);

        product = fopen("products.txt", "r");
        if (product == NULL)
        {
            printf("Error: Unable to open the file.\n");
            return 1;
        }

        while (fscanf(product, "%d %s %d %f", &prod.id, prod.product_name, &prod.quantity, &prod.price) == 4)
        {
            if (strcasecmp(product_name, prod.product_name) == 0)
            {
                found = 1;
                printf("In Stock: %d\n", prod.quantity);
                printf("Price: %.2f\n", prod.price);

                printf("Enter the quantity: ");
                scanf("%d", &order);

                if (order <= prod.quantity)
                {
                    bill += prod.price * order;
                    printf("Product added to order. Current total bill: %.2f\n", bill);

                    update_stock(order, product_name);
                    save_transaction(product_name, order, prod.price * order);
                }
                else
                {
                    printf("Not enough stock.\n");
                }
                break;
            }
        }

        fclose(product);

        if (!found)
        {
            printf("Product not found or out of stock.\n");
        }

        printf("Do you want to add another product? (y/n): ");
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');

    printf("Your Total amount is %.2f\n", bill);
    printf("Thank you for shopping with us!\n");

    return 0;
}

void authenticate()
{
    printf("Enter username: ");
    char username[20];
    scanf("%s", username);
    printf("Enter password: ");
    char password[20];
    scanf("%s", password);
    if (strcmp(username, "admin") == 0 && strcmp(password, "*****") == 0)
    {
        printf("Authentication successful.\n\n");
        menu();
    }
    else if (strcmp(username, "cashier") == 0 && strcmp(password, "*****") == 0)
    {
        printf("Authentication Successful.\n");
        cashier();
    }
    else
    {
        printf("Authentication failed.\n");
    }
}

int main()
{
    authenticate();
    return 0;
}
