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

#define MAX_RECORDS 100
#define MAX_USERNAME_SIZE 50
#define MAX_PASSWORD_SIZE 50
#define MAX_ID_SIZE 5
#define MAX_COUNTRY_SIZE 100
#define MAX_TRANSACTION_TYPE_SIZE 10
#define MIN_ACCOUNT_BALANCE 0.0
#define MAX_ACCOUNT_BALANCE 999999999.99
#define MIN_PHONE_LENGTH 10
#define MAX_PHONE_LENGTH 15

const char *RECORDS = "./data/records.txt";

/**
 * @brief Safely read an integer from input
 * 
 * This function reads an integer from standard input and handles any
 * input errors or invalid characters.
 * 
 * @param value Pointer to store the read integer
 * @return true if successful, false if input is invalid
 */
bool safeReadInt(int *value) {
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return false;
    }
    char *endptr;
    *value = strtol(input, &endptr, 10);
    return (*endptr == '\n' || *endptr == '\0');
}

/**
 * @brief Safely read a double from input
 * 
 * This function reads a double from standard input and handles any
 * input errors or invalid characters.
 * 
 * @param value Pointer to store the read double
 * @return true if successful, false if input is invalid
 */
bool safeReadDouble(double *value) {
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return false;
    }
    char *endptr;
    *value = strtod(input, &endptr);
    return (*endptr == '\n' || *endptr == '\0');
}

/**
 * @brief Safely read a string from input
 * 
 * This function reads a string from standard input and ensures it
 * doesn't exceed the maximum length.
 * 
 * @param str Buffer to store the read string
 * @param maxLen Maximum length of the string
 * @return true if successful, false if input is invalid
 */
bool safeReadString(char *str, size_t maxLen) {
    if (fgets(str, maxLen, stdin) == NULL) {
        return false;
    }
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
    return true;
}

/**
 * @brief Check if a file exists
 * 
 * @param filename Name of the file to check
 * @return true if file exists, false otherwise
 */
bool fileExists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

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
 * @brief Validate a date structure
 * 
 * @param date Pointer to the date structure to validate
 * @return true if date is valid, false otherwise
 */
bool isValidDate(int month, int day, int year) {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    
    // Check for months with 30 days
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
    
    // Check for February
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            if (day > 29) return false;
        } else {
            if (day > 28) return false;
        }
    }
    
    return true;
}

/**
 * @brief Validate a phone number
 * 
 * @param phone Phone number to validate
 * @return true if phone number is valid, false otherwise
 */
bool isValidPhoneNumber(int phone) {
    char phoneStr[20];
    sprintf(phoneStr, "%d", phone);
    int len = strlen(phoneStr);
    return len >= MIN_PHONE_LENGTH && len <= MAX_PHONE_LENGTH;
}

/**
 * @brief Validate an amount
 * 
 * @param amount Amount to validate
 * @return true if amount is valid, false otherwise
 */
bool isValidAmount(double amount) {
    return amount >= MIN_ACCOUNT_BALANCE && amount <= MAX_ACCOUNT_BALANCE;
}

/**
 * @brief Validate an account type
 * 
 * @param type Account type to validate
 * @return true if account type is valid, false otherwise
 */
bool isValidAccountType(const char *type) {
    const char *validTypes[] = {"savings", "current", "fixed01", "fixed02", "fixed03"};
    for (int i = 0; i < 5; i++) {
        if (strcmp(type, validTypes[i]) == 0) return true;
    }
    return false;
}

/**
 * @brief Get current date
 * 
 * @param date Pointer to the date structure to fill
 */
void getCurrentDate(struct Date *date) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    date->year = tm.tm_year + 1900;
    date->month = tm.tm_mon + 1;
    date->day = tm.tm_mday;
}

/**
 * @brief Create a backup of the records file
 * 
 * @return true if backup was successful, false otherwise
 */
bool backupRecordsFile() {
    if (!fileExists(RECORDS)) {
        return false;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char backupName[256];
    strftime(backupName, sizeof(backupName), "./data/records_backup_%Y%m%d_%H%M%S.txt", t);

    FILE *source = fopen(RECORDS, "r");
    if (source == NULL) return false;

    FILE *backup = fopen(backupName, "w");
    if (backup == NULL) {
        fclose(source);
        return false;
    }

    char buffer[1024];
    size_t size;
    while ((size = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        fwrite(buffer, 1, size, backup);
    }

    fclose(source);
    fclose(backup);
    return true;
}

/**
 * @brief Get account information from file
 * 
 * This function reads account information from the records file and
 * validates the data before returning it.
 * 
 * @param u User structure containing user information
 * @param accountNbr Account number to search for
 * @param record Pointer to store the found record
 * @return true if account was found and is valid, false otherwise
 */
bool getAccountFromFile(struct User u, int accountNbr, struct Record *record)
{
    FILE *file = fopen(RECORDS, "r");
    if (file == NULL) {
        printf("\n\t\t✖ Error: Could not open records file\n");
        return false;
    }

    bool found = false;
    while (fread(record, sizeof(struct Record), 1, file)) {
        if (record->accountNbr == accountNbr && strcmp(record->name, u.name) == 0) {
            found = true;
            break;
        }
    }

    fclose(file);
    return found;
}

/**
 * @brief Save account information to file
 * 
 * This function saves account information to the records file after
 * validating the data.
 * 
 * @param record Pointer to the record to save
 * @return true if save was successful, false otherwise
 */
bool saveAccountToFile(struct Record *record)
{
    if (!isValidDate(record->deposit.month, record->deposit.day, record->deposit.year) ||
        !isValidPhoneNumber(record->phone) ||
        !isValidAmount(record->amount) ||
        !isValidAccountType(record->accountType)) {
        return false;
    }

    FILE *file = fopen(RECORDS, "a");
    if (file == NULL) {
        printf("\n\t\t✖ Error: Could not open records file\n");
        return false;
    }

    fwrite(record, sizeof(struct Record), 1, file);
    fclose(file);
    return true;
}

void updateUserAccountInFile(FILE *ptr, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n", r.id, r.userId,
            r.name, r.accountNbr, r.deposit.month, r.deposit.day, r.deposit.year,
            r.country, r.phone, r.amount, r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    do
    {
        if (notGood == 0)
        {
            printf("\n\t\tEnter 0 to try again, 1 to return to main menu and 2 to "
                   "exit! \n\n");
        }
        else
        {
            printf("\n\t\tEnter 1 to go to the main menu and 0 to exit! \n\n");
        }

        scanf("%d", &option);

        if (option == 0)
        {
            f(u);
            break;
        }
        else if (option == 1)
        {
            mainMenu(u);
            break;
        }
        else if (option == 2)
        {
            exit(0);
        }
        else
        {
            printf("Insert a valid operation!\n");
        }
    } while (option < 0 || option > 2);
}

void success(struct User u)
{
    int option;

    while (1)
    {
        printf("\n\t\tEnter 1 to go to the main menu and 0 to exit! \n\n");
        scanf("%d", &option);
        system("clear");

        if (option == 1)
        {
            mainMenu(u);
            break;
        }
        else if (option == 0)
        {
            exit(1);
        }
        else
        {
            printf("\n\t\tInsert a valid operation!\n");
        }
    }
}

int getUserId(const char *username)
{
    FILE *fp = fopen("./data/users.txt", "r");
    if (!fp)
    {
        perror("\n\t\tError opening file");
        exit(1);
    }

    char id[MAX_ID_SIZE], name[MAX_USERNAME_SIZE], pass[MAX_PASSWORD_SIZE];
    while (fscanf(fp, "%s %s %s", id, name, pass) != EOF)
    {
        if (strcmp(name, username) == 0)
        {
            fclose(fp);
            return atoi(id);
        }
    }

    fclose(fp);
    return -1;
}

int doesUserHaveAccounts(struct User u)
{
    struct Record r;
    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        perror("\n\t\tFailed to open file");
        return 0;
    }
    char userName[MAX_USERNAME_SIZE];
    while (getAccountFromFile(u, r.accountNbr, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            fclose(pf);
            return 1;
        }
    }
    fclose(pf);
    return 0;
}

/**
 * @brief Create a new account
 * 
 * This function handles the creation of a new account, including
 * input validation and file operations.
 * 
 * @param u User structure containing user information
 */
void createNewAccount(struct User u)
{
    struct Record record;
    record.id = 0;
    strncpy(record.name, u.name, sizeof(record.name) - 1);
    record.name[sizeof(record.name) - 1] = '\0';

    printf("\n\t\tEnter the account number: ");
    if (!safeReadInt(&record.accountNbr) || record.accountNbr <= 0) {
        printf("\n\t\t✖ Invalid account number\n");
        return;
    }

    printf("\n\t\tEnter the phone number: ");
    if (!safeReadString(record.phone, sizeof(record.phone)) || 
        !isValidPhoneNumber(record.phone)) {
        printf("\n\t\t✖ Invalid phone number\n");
        return;
    }

    printf("\n\t\tEnter amount to deposit: $");
    if (!safeReadDouble(&record.amount) || !isValidAmount(record.amount)) {
        printf("\n\t\t✖ Invalid amount\n");
        return;
    }

    printf("\n\t\tChoose the type of account:\n");
    printf("\t\t[1] savings\n");
    printf("\t\t[2] fixed01(for 1 year)\n");
    printf("\t\t[3] fixed02(for 2 years)\n");
    printf("\t\t[4] fixed03(for 3 years)\n");

    int choice;
    if (!safeReadInt(&choice) || choice < 1 || choice > 4) {
        printf("\n\t\t✖ Invalid choice\n");
        return;
    }

    switch (choice) {
        case 1:
            strncpy(record.accountType, "savings", sizeof(record.accountType) - 1);
            break;
        case 2:
            strncpy(record.accountType, "fixed01", sizeof(record.accountType) - 1);
            break;
        case 3:
            strncpy(record.accountType, "fixed02", sizeof(record.accountType) - 1);
            break;
        case 4:
            strncpy(record.accountType, "fixed03", sizeof(record.accountType) - 1);
            break;
    }
    record.accountType[sizeof(record.accountType) - 1] = '\0';

    getCurrentDate(&record.deposit);
    record.withdrawal = record.deposit;

    if (!backupRecordsFile()) {
        printf("\n\t\t✖ Error: Could not create backup before saving\n");
        return;
    }

    if (!saveAccountToFile(&record)) {
        printf("\n\t\t✖ Error: Could not save account information\n");
        return;
    }

    printf("\n\t\t✓ Account created successfully!\n");
}

/**
 * @brief Check all accounts for a user
 * 
 * This function displays all accounts associated with a user.
 * 
 * @param u User structure containing user information
 */
void checkAllAccounts(struct User u)
{
    FILE *file = fopen(RECORDS, "r");
    if (file == NULL) {
        printf("\n\t\t✖ Error: Could not open records file\n");
        return;
    }

    struct Record record;
    bool found = false;
    printf("\n\t\t====== All accounts from user, %s =======\n\n", u.name);
    
    while (fread(&record, sizeof(struct Record), 1, file)) {
        if (strcmp(record.name, u.name) == 0) {
            printf("\t\t_____________________\n");
            printf("\t\tAccount number:%d\n", record.accountNbr);
            printf("\t\tCountry:%s\n", record.country);
            printf("\t\tPhone number:%s\n", record.phone);
            printf("\t\tAmount deposited: $%.2f\n", record.amount);
            printf("\t\tType Of Account:%s\n", record.accountType);
            printf("\t\tCreated At:%d/%d/%d\n\n", 
                   record.deposit.month, record.deposit.day, record.deposit.year);
            found = true;
        }
    }

    if (!found) {
        printf("\n\t\t✖ No accounts found for user %s\n", u.name);
    }

    fclose(file);
}

/**
 * @brief Update account information
 * 
 * This function allows updating account information such as phone number
 * and country.
 * 
 * @param u User structure containing user information
 */
void updateAccountInformation(struct User u)
{
    if (!backupRecordsFile()) {
        printf("\n\t\t✖ Error: Could not create backup before updating\n");
        return;
    }

    int accountNbr;
    printf("\n\t\tEnter the account number: ");
    if (!safeReadInt(&accountNbr) || accountNbr <= 0) {
        printf("\n\t\t✖ Invalid account number\n");
        return;
    }

    FILE *file = fopen(RECORDS, "r+");
    if (file == NULL) {
        printf("\n\t\t✖ Error: Could not open records file\n");
        return;
    }

    struct Record record;
    bool found = false;
    long int position = 0;

    while (fread(&record, sizeof(struct Record), 1, file)) {
        if (record.accountNbr == accountNbr && strcmp(record.name, u.name) == 0) {
            found = true;
            break;
        }
        position = ftell(file);
    }

    if (!found) {
        printf("\n\t\t✖ Account not found\n");
        fclose(file);
        return;
    }

    if (strstr(record.accountType, "fixed") != NULL) {
        printf("\n\t\t✖ Cannot modify fixed-term accounts\n");
        fclose(file);
        return;
    }

    printf("\n\t\tEnter new phone number: ");
    if (!safeReadString(record.phone, sizeof(record.phone)) || 
        !isValidPhoneNumber(record.phone)) {
        printf("\n\t\t✖ Invalid phone number\n");
        fclose(file);
        return;
    }

    printf("\n\t\tEnter new country: ");
    if (!safeReadString(record.country, sizeof(record.country))) {
        printf("\n\t\t✖ Invalid country\n");
        fclose(file);
        return;
    }

    fseek(file, position, SEEK_SET);
    fwrite(&record, sizeof(struct Record), 1, file);
    fclose(file);

    printf("\n\t\t✓ Account information updated successfully!\n");
}

/**
 * @brief Check account details
 * 
 * This function displays detailed information about a specific account.
 * 
 * @param u User structure containing user information
 */
void checkAccountDetails(struct User u)
{
    int accountNbr;
    printf("\n\t\tEnter the account number: ");
    if (!safeReadInt(&accountNbr) || accountNbr <= 0) {
        printf("\n\t\t✖ Invalid account number\n");
        return;
    }

    struct Record record;
    if (!getAccountFromFile(u, accountNbr, &record)) {
        printf("\n\t\t✖ Account not found\n");
        return;
    }

    printf("\n\t\t====== Account Details ======\n");
    printf("\t\tAccount number: %d\n", record.accountNbr);
    printf("\t\tCountry: %s\n", record.country);
    printf("\t\tPhone number: %s\n", record.phone);
    printf("\t\tAmount deposited: $%.2f\n", record.amount);
    printf("\t\tType of account: %s\n", record.accountType);
    printf("\t\tCreated at: %d/%d/%d\n", 
           record.deposit.month, record.deposit.day, record.deposit.year);
}

/**
 * @brief Make a transaction
 * 
 * This function handles deposits and withdrawals for an account.
 * 
 * @param u User structure containing user information
 */
void makeTransaction(struct User u)
{
    if (!backupRecordsFile()) {
        printf("\n\t\t✖ Error: Could not create backup before transaction\n");
        return;
    }

    int accountNbr;
    printf("\n\t\tEnter the account number: ");
    if (!safeReadInt(&accountNbr) || accountNbr <= 0) {
        printf("\n\t\t✖ Invalid account number\n");
        return;
    }

    struct Record record;
    if (!getAccountFromFile(u, accountNbr, &record)) {
        printf("\n\t\t✖ Account not found\n");
        return;
    }

    if (strstr(record.accountType, "fixed") != NULL) {
        printf("\n\t\t✖ Cannot make transactions on fixed-term accounts\n");
        return;
    }

    printf("\n\t\tDo you want to\n");
    printf("\t\t[1] Deposit\n");
    printf("\t\t[2] Withdraw\n");

    int choice;
    if (!safeReadInt(&choice) || (choice != 1 && choice != 2)) {
        printf("\n\t\t✖ Invalid choice\n");
        return;
    }

    double amount;
    printf("\n\t\tEnter the amount: $");
    if (!safeReadDouble(&amount) || amount <= 0) {
        printf("\n\t\t✖ Invalid amount\n");
        return;
    }

    if (choice == 1) {
        record.amount += amount;
    } else {
        if (amount > record.amount) {
            printf("\n\t\t✖ Insufficient funds\n");
            return;
        }
        record.amount -= amount;
    }

    FILE *file = fopen(RECORDS, "r+");
    if (file == NULL) {
        printf("\n\t\t✖ Error: Could not open records file\n");
        return;
    }

    struct Record temp;
    while (fread(&temp, sizeof(struct Record), 1, file)) {
        if (temp.accountNbr == accountNbr && strcmp(temp.name, u.name) == 0) {
            fseek(file, -sizeof(struct Record), SEEK_CUR);
            fwrite(&record, sizeof(struct Record), 1, file);
            break;
        }
    }

    fclose(file);
    printf("\n\t\t✓ Transaction completed successfully!\n");
}

/**
 * @brief Remove an account
 * 
 * This function handles the removal of an account after validation
 * and confirmation.
 * 
 * @param u User structure containing user information
 */
void removeAccount(struct User u)
{
    if (!backupRecordsFile()) {
        printf("\n\t\t✖ Error: Could not create backup before removal\n");
        return;
    }

    int accountNbr;
    printf("\n\t\tEnter the account number: ");
    if (!safeReadInt(&accountNbr) || accountNbr <= 0) {
        printf("\n\t\t✖ Invalid account number\n");
        return;
    }

    struct Record record;
    if (!getAccountFromFile(u, accountNbr, &record)) {
        printf("\n\t\t✖ Account not found\n");
        return;
    }

    if (strstr(record.accountType, "fixed") != NULL) {
        printf("\n\t\t✖ Cannot remove fixed-term accounts\n");
        return;
    }

    printf("\n\t\tAre you sure you want to remove this account? (y/n): ");
    char confirm;
    if (!safeReadString(&confirm, 2) || (confirm != 'y' && confirm != 'Y')) {
        printf("\n\t\tAccount removal cancelled\n");
        return;
    }

    FILE *file = fopen(RECORDS, "r");
    if (file == NULL) {
        printf("\n\t\t✖ Error: Could not open records file\n");
        return;
    }

    FILE *temp = fopen("data/temp.txt", "w");
    if (temp == NULL) {
        printf("\n\t\t✖ Error: Could not create temporary file\n");
        fclose(file);
        return;
    }

    struct Record tempRecord;
    while (fread(&tempRecord, sizeof(struct Record), 1, file)) {
        if (!(tempRecord.accountNbr == accountNbr && strcmp(tempRecord.name, u.name) == 0)) {
            fwrite(&tempRecord, sizeof(struct Record), 1, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove(RECORDS);
    rename("data/temp.txt", RECORDS);

    printf("\n\t\t✓ Account removed successfully!\n");
}

/**
 * @brief Transfer account ownership
 * 
 * This function handles the transfer of account ownership to another user.
 * 
 * @param u User structure containing user information
 */
void transferOwnership(struct User u)
{
    if (!backupRecordsFile()) {
        printf("\n\t\t✖ Error: Could not create backup before transfer\n");
        return;
    }

    int accountNbr;
    printf("\n\t\tEnter the account number: ");
    if (!safeReadInt(&accountNbr) || accountNbr <= 0) {
        printf("\n\t\t✖ Invalid account number\n");
        return;
    }

    struct Record record;
    if (!getAccountFromFile(u, accountNbr, &record)) {
        printf("\n\t\t✖ Account not found\n");
        return;
    }

    if (strstr(record.accountType, "fixed") != NULL) {
        printf("\n\t\t✖ Cannot transfer fixed-term accounts\n");
        return;
    }

    char newOwner[50];
    printf("\n\t\tEnter the new owner's username: ");
    if (!safeReadString(newOwner, sizeof(newOwner))) {
        printf("\n\t\t✖ Invalid username\n");
        return;
    }

    if (strcmp(newOwner, u.name) == 0) {
        printf("\n\t\t✖ Cannot transfer to yourself\n");
        return;
    }

    FILE *users = fopen("data/users.txt", "r");
    if (users == NULL) {
        printf("\n\t\t✖ Error: Could not open users file\n");
        return;
    }

    struct User tempUser;
    bool userExists = false;
    while (fread(&tempUser, sizeof(struct User), 1, users)) {
        if (strcmp(tempUser.name, newOwner) == 0) {
            userExists = true;
            break;
        }
    }
    fclose(users);

    if (!userExists) {
        printf("\n\t\t✖ User does not exist\n");
        return;
    }

    FILE *file = fopen(RECORDS, "r+");
    if (file == NULL) {
        printf("\n\t\t✖ Error: Could not open records file\n");
        return;
    }

    struct Record temp;
    while (fread(&temp, sizeof(struct Record), 1, file)) {
        if (temp.accountNbr == accountNbr && strcmp(temp.name, u.name) == 0) {
            strncpy(temp.name, newOwner, sizeof(temp.name) - 1);
            temp.name[sizeof(temp.name) - 1] = '\0';
            fseek(file, -sizeof(struct Record), SEEK_CUR);
            fwrite(&temp, sizeof(struct Record), 1, file);
            break;
        }
    }

    fclose(file);
    printf("\n\t\t✓ Ownership transferred successfully!\n");
}

/**
 * @brief Handle failed login attempts
 * 
 * This function handles failed login attempts and implements
 * security measures.
 * 
 * @param u Pointer to the user structure
 */
void handleFailedLogin(struct User *u)
{
    static int attempts = 0;
    attempts++;

    if (attempts >= 3) {
        printf("\n\t\t✖ Too many failed attempts. Please try again later.\n");
        sleep(2);
        exit(1);
    }

    printf("\n\t\t✖ Failed attempt %d of 3\n", attempts);
    sleep(1);
}

/**
 * @brief Handle failed registration attempts
 * 
 * This function handles failed registration attempts and implements
 * security measures.
 * 
 * @param u Pointer to the user structure
 */
void handleFailedRegistration(struct User *u)
{
    static int attempts = 0;
    attempts++;

    if (attempts >= 3) {
        printf("\n\t\t✖ Too many failed registration attempts. Please try again later.\n");
        sleep(2);
        exit(1);
    }

    printf("\n\t\t✖ Failed registration attempt %d of 3\n", attempts);
    sleep(1);
}