#define ATM_MANAGEMENT_SYSTEM_HEADER_H
#define ATM_MANAGEMENT_SYSTEM_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Date structure
struct Date {
    int day;
    int month;
    int year;
};

// File paths
#define RECORDS_FILE_PATH "./data/records.txt"
#define USERS_FILE_PATH "./data/users.txt"

// Limits
#define MAX_RECORDS 1000
#define MAX_USERS 1000
#define MAX_NAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_COUNTRY_LENGTH 100
#define MAX_ACCOUNT_TYPE_LENGTH 10
#define MAX_PHONE_LENGTH 15
#define MAX_ACCOUNT_NUMBER_LENGTH 20

// Record structure
struct Record {
    int id;
    int userId;
    char name[MAX_NAME_LENGTH];
    char country[MAX_COUNTRY_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char accountType[MAX_ACCOUNT_TYPE_LENGTH];
    char accountNumber[MAX_ACCOUNT_NUMBER_LENGTH];
    double amount;
    struct Date depositDate;
    struct Date withdrawDate;
};

// User structure
struct User {
    int id;
    char name[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

// Authentication functions
void loginMenu(char a[50], char pass[50]);
void registerMenu(char a[50], char pass[50]);
const char *getPassword(struct User u);

// System functions
void createNewAcc(struct User u);
void mainMenu(struct User u);
void checkAllAccounts(struct User u);
void registerUser(struct User *u);
void updateAccountInfo(struct User u);
void checkAccountDetails(struct User u);
void makeTransaction(struct User u);
void removeAccount(struct User u);
void transferOwnership(struct User u);
