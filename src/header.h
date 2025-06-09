/**
 * @file header.h
 * @brief Main header file for the ATM Management System
 * @author Khalid Hussein
 * @date 2025
 * 
 * This header file contains all the necessary structures and function declarations
 * for the ATM Management System. It defines the core data structures and interfaces
 * used throughout the application.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERNAME_SIZE 50
#define MAX_PASSWORD_SIZE 50
#define MAX_COUNTRY_SIZE 100
#define MAX_TRANSACTION_TYPE_SIZE 10

/**
 * @brief Structure to store date information
 */
struct Date
{
    int month;  ///< Month (1-12)
    int day;    ///< Day (1-31)
    int year;   ///< Year (1900-2100)
};

/**
 * @brief Structure to store account record information
 */
struct Record
{
    int id;                         ///< Unique record identifier
    int userId;                     ///< ID of the user who owns this record
    char name[MAX_USERNAME_SIZE];   ///< Username of the account owner
    char country[MAX_COUNTRY_SIZE]; ///< Country of residence
    int phone;                      ///< Phone number
    char accountType[MAX_TRANSACTION_TYPE_SIZE]; ///< Type of account (savings/current/fixed)
    int accountNbr;                 ///< Account number
    double amount;                  ///< Current balance
    struct Date deposit;            ///< Date of account creation
    struct Date withdraw;           ///< Date of last withdrawal
};

/**
 * @brief Structure to store user information
 */
struct User
{
    int id;                         ///< Unique user identifier
    char name[MAX_USERNAME_SIZE];   ///< Username
    char password[MAX_PASSWORD_SIZE]; ///< User's password
};

// authentication functions
void loginMenu(char a[MAX_USERNAME_SIZE], char pass[MAX_PASSWORD_SIZE]);
// utility functions
const int setId();
const int getId(struct User u);
const char *getPassword(struct User u);
void registerUser(char a[MAX_USERNAME_SIZE],  char pass[MAX_PASSWORD_SIZE]);
const char *getUserName(struct User u);
void saveUser(struct User *u);
int getUser(FILE *ptr, struct User *u);

// system function
void createNewAcc(struct User u);
void mainMenu(struct User u);
void checkAllAccounts(struct User u);
void updateInfo(struct User u);
void removeAccount(struct User u);
void checkDetails(struct User u);
void makeTransaction(struct User u);
void transferOwner(struct User u);

//utility
void toLowerCase(char *str);
void clearStdin();
int checkValidDate(const struct Date *date);
int checkValidAccount(const char *accountType);
void checkBuffer(char initial[100]);
int checkValidType(const char *input, const char *type);
int isLeapYear(int year);
int getAccountFromFile(FILE *ptr, struct Record *r);
void saveAccountToFile(FILE *ptr, const struct Record *r);
void stayOrReturn(int notGood, const char *message, void (*retryFunc)(struct User), struct User u);
void success(struct User u);