/**
 * @file system.c
 * @brief Core system implementation for the ATM Management System
 * @author Khalid Hussein
 * @date 2025
 * 
 * This file contains the implementation of core system functions for the
 * ATM Management System, including account management, transactions,
 * and data validation.
 */

#include "header.h"
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <stdbool.h>

const char *RECORDS = "./data/records.txt";

#include <stdbool.h>
bool fileExists(const char *path);

/**
 * @brief Ensure a directory exists, create if it doesn't
 * 
 * @param path Path of the directory to check/create
 * @return true if directory exists or was created successfully
 */
void ensureDirectoryExists(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0700);
    }
}

/**
 * @brief Initialize the system
 * 
 * This function initializes the system by creating necessary directories
 * and files if they don't exist.
 * 
 * @return true if initialization was successful
 */
void initSystem() {
    ensureDirectoryExists("./data");
    
    // Create users file if it doesn't exist
    if (!fileExists("./data/users.txt")) {
        FILE *fp = fopen("./data/users.txt", "w");
        if (fp) {
            fclose(fp);
        }
    }
    
    // Create records file if it doesn't exist
    if (!fileExists("./data/records.txt")) {
        FILE *fp = fopen("./data/records.txt", "w");
        if (fp) {
            fclose(fp);
        }
    }
}

/**
 * @brief Check if a file exists
 * 
 * @param path Path to the file
 * @return true if the file exists, false otherwise
 */
bool fileExists(const char *path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}



/**
 * @brief Create a new account
 * 
 * @param u User information
 */
void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char initial[100];
    char userName[50];
    int count = 0;
    char c;
    FILE *pf;

    system("clear");
    if ((pf = fopen(RECORDS, "a+")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    printf("\t\t\t===== New record =====\n");

    while (getAccountFromFile(pf, &cr))
    {
        //loop till we reach the last value
    }
    count = cr.id;
    rewind(pf);
    r.id = count + 1;

validDate:
    printf("\nEnter today's date(mm/dd/yyyy):");
    fgets(initial,50,stdin);
    checkBuffer(initial);
    sscanf(initial,"%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);

    if (checkValidDate(&r.deposit) == 1)
    {
        printf("\n\n Please!! Enter a valid date\n\n");
        goto validDate;
    }


validAccount:
    printf("\nEnter the account number:");
    fgets(initial,50,stdin);
    checkBuffer(initial);
    if (checkValidType(initial, "int") != 0)
    {
        printf("\n\t\t✖ Please!! Enter a valid account number, numbers only or positive numbers \n\n");
        goto validAccount;
    }
    sscanf(initial,"%d", &r.accountNbr);
    if (r.accountNbr < 0) {
        printf("\n\t\tPlease enter a useable account number\n\n");
        goto validAccount;
    }

    while (getAccountFromFile(pf, &cr))
    {
        if (cr.accountNbr == r.accountNbr)
        {
            fclose(pf);
            stayOrReturn(0, "This Account number is already used", createNewAcc, u);
        }
    }

validCountry:
    printf("\nEnter the country:");
    fgets(initial,50,stdin);
    checkBuffer(initial);
    if (checkValidType(initial, "str") != 0)
    {
        printf("\n\t\t✖ Please!! Enter a valid country, don't include numbers or punctuation\n\n");
        goto validCountry;
    }
    strncpy(r.country, initial, sizeof(r.country) - 1);
    r.country[sizeof(r.country) - 1] = '\0';

validPhone:
    printf("\nEnter the phone number:");
    fgets(initial,50,stdin);
    checkBuffer(initial);
    if (checkValidType(initial, "int") != 0)
    {
        printf("\n\t\t✖ Please!! Enter a valid phone-number, don't include letters or punctuation\n\n");
        goto validPhone;
    }
    sscanf(initial,"%d", &r.phone);
    if (r.phone < 0) {
        printf("\n\t\tEnter a valid phone number\n\n");
        goto validPhone;
    }


validAmount:
    printf("\nEnter amount to deposit: $");
    fgets(initial,50,stdin);
    checkBuffer(initial);
    if (checkValidType(initial, "flt") != 0)
    {
        printf("\n\t\t✖ Please!! Enter a valid amount, don't use commas\n\n");
        goto validAmount;
    }
    sscanf(initial,"%lf", &r.amount);
    if (&r.amount < 0) {
        printf("\n\t\tEnter a valid amout\n\n");
        goto validAmount;
    }

validAccountType:
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    fgets(initial,50,stdin);
    checkBuffer(initial);
    if (checkValidAccount(initial) != 0)
    {
        printf("\n\t\t✖ Please!! Enter a valid option avaliable on the list\n\n");
        goto validAccountType;
    }
    strncpy(r.accountType, initial, sizeof(r.accountType) - 1);
    r.accountType[sizeof(r.accountType) - 1] = '\0';


    r.userId = u.id;
    strncpy(r.name, u.name, sizeof(r.name) - 1);
    r.name[sizeof(r.name) - 1] = '\0';
    toLowerCase(r.accountType);

    saveAccountToFile(pf, &r);

    fclose(pf);
    success(u);
}

/**
 * @brief Display the main menu for the user
 * 
 * @param u User information
 */
void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;
    FILE *pf;

    if ((pf = fopen(RECORDS, "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, &r))
    {
        if (strcmp(r.name, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

/**
 * @brief Update user account information
 * 
 * @param u User information
 */
void updateInfo(struct User u)
{
    struct Record cr;
    int phone = 0;
    int account;
    int checker = 0;
    char buffer[100];
    FILE *curr, *temp;


    system("clear");
    if ((curr = fopen(RECORDS, "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
invalid:
    printf("\t\t What is the account number you want to change ?\n");
    fgets(buffer,100,stdin);
    checkBuffer(buffer);

    if(checkValidType(buffer, "int")!= 0)
    {
        printf("\t\nPlease enter a valid option\n\n");
        goto invalid;
    }
        sscanf(buffer,"%d", &account);

    while (getAccountFromFile(curr, &cr))
    {
        if (strcmp(cr.name, u.name) == 0 && cr.accountNbr == account)
        {
            checker = 1;
            break;
        }
    }

    rewind(curr);
    if (checker == 0)
    {
    fclose(curr);
    stayOrReturn(0, "This account does not exist",updateInfo,u);

    }

    checker = 0;
validOption:
    printf("\tWhich information do you want?\n ");
    printf("\t 1-> phone number\n");
    printf("\t 2-> country\n");
    fgets(buffer,50,stdin);
    checkBuffer(buffer);

    if (checkValidType(buffer,"int") != 0)
    {
        printf("\n\tplease choose either 1 or 2 \n\n");
        goto validOption;
    }
    int number = 0;
    sscanf(buffer,"%d", &number);
    if (number < 0 || number > 2 )
    {
        printf("\t\nEnter between one and two\n\n");
        goto validOption;
    }
    switch (number)
    {
    case 1:
enterPhone:
        checker = 1;
        printf("Enter your new phone number: ");
        fgets(buffer,100,stdin);
        checkBuffer(buffer);

        if (checkValidType(buffer, "int") != 0)
        {
            printf("\n\tPlease enter a valide phone number\n\n");
            goto enterPhone;
        }
        sscanf(buffer,"%d", &phone);
        if (phone < 0) {
            printf("\t\nPlease enter a valid phone number\n\n");
            goto enterPhone;
        }
        break;
    case 2:
enterCountry:
        printf("Enter your new country: ");
        fgets(buffer,100,stdin);
        checkBuffer(buffer);

        if (checkValidType(buffer, "str") != 0)
        {
            printf("\n\tPlease enter a valid letters only\n\n");
            goto enterCountry;
        }
        break;
    default:
        break;
    }

    if ((temp = fopen("./data/temp.txt", "w")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (getAccountFromFile(curr, &cr))
    {
        if (strcmp(cr.name, u.name) == 0 && cr.accountNbr == account)
        {
            if(checker == 0) {
                strncpy(cr.country, buffer, sizeof(cr.country) - 1);
                cr.country[sizeof(cr.country) - 1] = '\0';
            } else {
                cr.phone = phone;
            }
        }
        saveAccountToFile(temp, &cr);
    }

    fclose(curr);
    fclose(temp);
    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);

    success(u);
}

/**
 * @brief Remove an account
 * 
 * @param u User information
 */
void removeAccount(struct User u)
{
    struct Record cr;
    FILE *curr, *temp;
    int checker = 0;
    char buffer[100];
    int account;

    system("clear");
    if ((curr = fopen(RECORDS, "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
enterAccount:
    printf("\t Enter the account you want to delete :");
    fgets(buffer,100,stdin);
    checkBuffer(buffer);

    if (checkValidType(buffer, "int") != 0)
    {
        printf("\t\nPlease input numericals only\n\n");
        goto enterAccount;
    }

    sscanf(buffer,"%d",&account);

    while (getAccountFromFile(curr, &cr))
    {
        if (strcmp(cr.name, u.name) == 0 && cr.accountNbr == account)
        {
            checker = 1;
            break;
        }
    }

    rewind(curr);
    if (checker == 0)
    {
        fclose(curr);
        stayOrReturn(0, "There is no account of this record", removeAccount, u);

    }
    system("clear");
    printf("\t\t====== Deleted account ======\n\n");
    printf("\tAccount number:%d\n", cr.accountNbr);
    printf("\tCountry:%s\n", cr.country);
    printf("\tPhone number:%d\n", cr.phone);
    printf("\tAmount deposited:%.2f\n", cr.amount);
    printf("\tType Of Account:%s\n\n", cr.accountType);

    if ((temp = fopen("./data/temp.txt", "w")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    checker = 0;
    while (getAccountFromFile(curr, &cr))
    {
        if (checker == 1)
        {
            cr.id = cr.id - 1;
        }
        if (strcmp(cr.name, u.name) == 0 && cr.accountNbr == account)
        {
            checker = 1;
            continue;
        }
        saveAccountToFile(temp, &cr);
    }

    fclose(curr);
    fclose(temp);
    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);
    success(u);
}

/**
 * @brief Check account details
 * 
 * @param u User information
 */
void checkDetails(struct User u)
{
    struct Record cr;
    FILE *fp;
    char buffer[100];
    int account;
    int checker = 0;

    system("clear");
    if ((fp = fopen(RECORDS, "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
validAccount:
    printf("\tEnter the account number: ");
    fgets(buffer,100,stdin);
    checkBuffer(buffer);

    if (checkValidType(buffer, "int") != 0)
    {
        printf("\t\n Please enter a valid account number\n\n");
        goto validAccount;
    }

    sscanf(buffer,"%d", &account);

    while(getAccountFromFile(fp, &cr))
    {
        if (strcmp(cr.name, u.name) == 0 && cr.accountNbr == account)
        {
            checker = 1;
            break;
        }
    }
    rewind(fp);
    if (checker == 0)
    {
        fclose(fp);
        stayOrReturn(0, "This account does not exist", checkDetails, u);
    }

    system("clear");
    printf("\n\tAccount number:%d\n", cr.accountNbr);
    printf("\tDeposit Date:%d/%d/%d\n", cr.deposit.day,cr.deposit.month,cr.deposit.year);
    printf("\tCountry:%s\n", cr.country);
    printf("\tPhone number:%d\n",cr.phone);
    printf("\tAmount deposited:%.2f\n", cr.amount);
    printf("\tType Of Account:%s\n\n", cr.accountType);

    float value;
    if (strcmp(cr.accountType, "saving") == 0)
    {
        value = (cr.amount * 0.07) / 12;
        printf("\tYou will get $%.2f as interest on day %d of every month", value, cr.deposit.day);
    } else if (strcmp(cr.accountType, "current") == 0)
    {
        printf("\tYou will not get interests because the account is of type current");
    } else if (strcmp(cr.accountType, "fixed01") == 0)
    {
        value = cr.amount * 0.04;
        printf("\tYou will get $%.2f as interest on  %d/%d/%d", value, cr.deposit.day,cr.deposit.month,cr.deposit.year + 1);
    } else if (strcmp(cr.accountType, "fixed02") == 0)
    {
        value = (cr.amount * 0.05) * 2;
        printf("\tYou will get $%.2f as interest on  %d/%d/%d", value, cr.deposit.day,cr.deposit.month,cr.deposit.year + 2);
    } else if (strcmp(cr.accountType, "fixed03") == 0)
    {
        value = (cr.amount * 0.08) * 3;
        printf("\tYou will get $%.2f as interest on  %d/%d/%d", value, cr.deposit.day,cr.deposit.month,cr.deposit.year + 3);
    } else
    {
        printf("\tYour account %s is not known and will be treated as current\n", cr.accountType);
    }
    success(u);
}

/**
 * @brief Make a transaction (deposit or withdraw) on an account
 * 
 * @param u User information
 */
void makeTransaction(struct User u)
{
    char buffer[100];
    struct Record cr;
    FILE *fp, *temp;
    int option;
    int account;
    double amount;
    int checker = 0;

    system("clear");
validac:
    printf("\tEnter your account number:");
    fgets(buffer,100,stdin);
    checkBuffer(buffer);

    if(checkValidType(buffer, "int")!= 0)
    {
        printf("\t\nPlease enter a valid option\n\n");
        goto validac;
    }
    sscanf(buffer,"%d", &account);


    if ((fp = fopen(RECORDS, "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    while(getAccountFromFile(fp, &cr))
    {
        if (strcmp(cr.name, u.name) == 0 && cr.accountNbr == account)
        {
            checker = 1;
            break;
        }
    }
    rewind(fp);
    if (checker == 0) {
        fclose(fp);
        stayOrReturn(0,"No account with that account number", makeTransaction, u);
    }

    if (strcmp(cr.accountType, "fixed01") == 0 || strcmp(cr.accountType, "fixed02") == 0 || strcmp(cr.accountType, "fixed03") == 0)
    {
        fclose(fp);
        stayOrReturn(0,"Cannot make transcations on fixed accounts", makeTransaction, u);
    }

option:
    printf("\tDo you want to\n\t\t1-> Deposit\n\t\t2-> Withdraw\n");
    fgets(buffer,100,stdin);
    checkBuffer(buffer);

    if(checkValidType(buffer, "int")!= 0)
    {
        printf("\t\nPlease enter a valid option\n\n");
        goto option;
    }
    sscanf(buffer,"%d", &option);

    if (option != 1 && option != 2)
    {
        printf("\tPlease pick a valid option\n");
        goto option;
        //redo the option
    }
Amount:
    printf("\tEnter the amount: $");
    fgets(buffer,100,stdin);
    checkBuffer(buffer);

    if(checkValidType(buffer, "flt")!= 0)
    {
        printf("\t\nPlease enter a valid option\n\n");
        goto Amount;
    }
    sscanf(buffer,"%lf", &amount);

    if (option == 2 && amount > cr.amount){
        fclose(fp);
        stayOrReturn(0,"Not enough money to make this transcation", makeTransaction, u);
    }
    if ((temp = fopen("./data/temp.txt", "w")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    while(getAccountFromFile(fp, &cr))
    {
        if (strcmp(cr.name, u.name) == 0 && cr.accountNbr == account) {
            if (option == 1) {
                cr.amount = cr.amount + amount;
            } else if (option == 2) {
                cr.amount = cr.amount - amount;
            }
        }
        saveAccountToFile(temp, &cr);
    }
    fclose(fp);
    fclose(temp);
    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);

    success(u);

}

/**
 * @brief Transfer ownership of an account
 * 
 * @param u User information
 */
void transferOwner(struct User u)
{
    const char *USERS = "./data/users.txt";
    struct Record r;
    struct User p;
    FILE *curr, *temp, *user;
    int checker = 0;
    int account;
    char buffer[100];
    char username[50];
    int userId = 0;

    system("clear");
validAcc:
    printf("\tEnter the account number you want to transfer ownership: ");
    fgets(buffer,100,stdin);
    checkBuffer(buffer);

    if(checkValidType(buffer, "int")!= 0)
    {
        printf("\t\nPlease enter a valid account number\n\n");
        goto validAcc;
    }
    sscanf(buffer,"%d", &account);

    if ((curr = fopen(RECORDS, "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    while (getAccountFromFile(curr, &r))
    {
        if (strcmp(u.name, r.name) == 0 && account == r.accountNbr) {
            checker = 1;
            break;
        }
    }
    rewind(curr);
    if (checker == 0)
    {
        fclose(curr);
        stayOrReturn(0, "This account does not exist", transferOwner, u);
    }

    printf("\n\t\t ==== Transfering account:\n\n");
    printf("\tAccount number: %d\n", r.accountNbr);
    printf("\tDeposit Date: %d/%d/%d \n", r.deposit.day, r.deposit.month, r.deposit.year);
    printf("\tCountry: %s\n", r.country);
    printf("\tPhone number: %d\n", r.phone);
    printf("\tAmount deposited: $%.2f\n", r.amount);
    printf("\tType Of Account: %s\n\n", r.accountType);

    printf("\tWhich user you want to transfer ownership to (user name): ");
    scanf("%99s", username);
    clearStdin();

    checker = 0;
    if ((user = fopen(USERS,"r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (getUser(user, &p))
    {
        if (strcmp(p.name, username) == 0)
        {
            checker = 1;
            userId = p.id;
            break;
        }
    }
    fclose(user);

    if (checker == 0)
    {
        fclose(curr);
        stayOrReturn(0, "The user provided does not exist", transferOwner, u);
    }

    if((temp = fopen("./data/temp.txt", "w")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (getAccountFromFile(curr, &r))
    {
        if (strcmp(u.name, r.name) == 0 && r.accountNbr == account)
        {
            strncpy(r.name, username, sizeof(r.name) - 1);
            r.name[sizeof(r.name) - 1] = '\0';
            r.userId = userId;
        }
        saveAccountToFile(temp, &r);
    }

    fclose(curr);
    fclose(temp);
    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);

    success(u);

}


/**
 * Converts a string to lowercase in-place.
 */
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

/**
 * Clears the standard input buffer.
 */
void clearStdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Returns 1 if the year is a leap year, 0 otherwise.
 */
int isLeapYear(int year) {
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

/**
 * Removes trailing newline from input and clears stdin if buffer overflowed.
 */
void checkBuffer(char *input) {
    int len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else {
        clearStdin();
    }
}

/**
 * Validates input string based on type: "str", "int", or "flt".
 * Returns 0 if valid, 1 if invalid.
 */
int checkValidType(const char *input, const char *type) {
    if (strlen(input) == 0) return 1;

    if (strcmp(type, "str") == 0) {
        for (int i = 0; input[i]; i++) {
            if (!isalpha((unsigned char)input[i])) return 1;
        }
        return 0;
    } else if (strcmp(type, "flt") == 0) {
        int dotCount = 0;
        for (int i = 0; input[i]; i++) {
            if (!isdigit((unsigned char)input[i])) {
                if (input[i] == '.') {
                    dotCount++;
                    if (dotCount > 1) return 1;
                } else {
                    return 1;
                }
            }
        }
        return 0;
    } else if (strcmp(type, "int") == 0) {
        for (int i = 0; input[i]; i++) {
            if (!isdigit((unsigned char)input[i])) return 1;
        }
        return 0;
    }
    return 1;
}

/**
 * Checks if the account type is valid.
 * Returns 0 if valid, 1 if invalid.
 */
int checkValidAccount(const char *accountType) {
    const char *accounts[] = {"saving", "current", "fixed01", "fixed02", "fixed03"};
    for (int i = 0; i < 5; i++) {
        if (strcmp(accountType, accounts[i]) == 0) return 0;
    }
    return 1;
}

/**
 * Validates a date.
 * Returns 0 if valid, 1 if invalid.
 */
int checkValidDate(const struct Date *date) {
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (date->month < 1 || date->month > 12) return 1;
    if (date->day < 1) return 1;

    if (date->month == 2 && isLeapYear(date->year)) {
        if (date->day > 29) return 1;
    } else {
        if (date->day > daysInMonth[date->month - 1]) return 1;
    }
    return 0;
}

/**
 * Reads an account record from file.
 * Returns 1 if successful, 0 if EOF or error.
 */
int getAccountFromFile(FILE *ptr, struct Record *r) {
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
                  &r->userId,
                  r->name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) == 11;
}

/**
 * Writes an account record to file.
 */
void saveAccountToFile(FILE *ptr, const struct Record *r) {
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r->id,
            r->userId,
            r->name,
            r->accountNbr,
            r->deposit.month,
            r->deposit.day,
            r->deposit.year,
            r->country,
            r->phone,
            r->amount,
            r->accountType);
}

/**
 * Handles user choice after an error or invalid operation.
 * If notGood == 0, offers retry, main menu, or exit.
 * If notGood != 0, offers main menu or exit.
 */
void stayOrReturn(int notGood, const char *message, void (*retryFunc)(struct User), struct User u) {
    char buffer[100];
    int option;

    if (notGood == 0) {
        system("clear");
        printf("\n✖ %s!!\n", message);
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit: ");
        fgets(buffer, sizeof(buffer), stdin);
        checkBuffer(buffer);

        if (checkValidType(buffer, "int") != 0) {
            printf("\nPlease enter a valid option\n\n");
            goto invalid;
        }
        sscanf(buffer, "%d", &option);

        if (option == 0)
            retryFunc(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    } else {
        printf("\nEnter 1 to go to the main menu and 0 to exit: ");
        fgets(buffer, sizeof(buffer), stdin);
        checkBuffer(buffer);

        if (checkValidType(buffer, "int") != 0) {
            printf("\nPlease enter a valid option\n\n");
            goto invalid;
        }
        sscanf(buffer, "%d", &option);

        if (option == 1) {
            system("clear");
            mainMenu(u);
        } else {
            system("clear");
            exit(1);
        }
    }
}

/**
 * Displays a success message and offers main menu or exit.
 */
void success(struct User u) {
    char buffer[100];
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    fgets(buffer, sizeof(buffer), stdin);
    checkBuffer(buffer);

    if (checkValidType(buffer, "int") != 0) {
        printf("\nPlease enter a valid option\n\n");
        goto invalid;
    }
    sscanf(buffer, "%d", &option);

    system("clear");
    if (option == 1) {
        mainMenu(u);
    } else if (option == 0) {
        exit(1);
    } else {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}
