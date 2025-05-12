#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User
{
    int id;
    char name[50];
    char password[50];
};

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r);
void saveAccountToFile(FILE *ptr, struct User u, struct Record r);

// User management
void registerUser(struct User *newUser);
int getNextUserId();
int getNextRecordId();
int accountNumberExists(int userId, int accountNbr);

// Account operations
void createNewAcc(struct User u);
void checkAllAccounts(struct User u);
void updateAccountInfo(struct User u);
void checkAccountDetails(struct User u);
void makeTransaction(struct User u);
void removeAccount(struct User u);
void transferOwnership(struct User u);

// Helper functions
void stayOrReturn(int notGood, void f(struct User u), struct User u);
void success(struct User u);