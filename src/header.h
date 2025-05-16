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

/**
 * @brief Authentication Functions
 */

/**
 * @brief Initialize the main menu
 * @param u Pointer to the user structure
 */
void initMenu(struct User *u);

/**
 * @brief Display and handle login menu
 * @param a Array to store username
 * @param pass Array to store password
 */
void loginMenu(char a[MAX_USERNAME_SIZE], char pass[MAX_PASSWORD_SIZE]);

/**
 * @brief Display and handle registration menu
 * @param a Array to store username
 * @param pass Array to store password
 */
void registerMenu(char a[MAX_USERNAME_SIZE], char pass[MAX_PASSWORD_SIZE]);

/**
 * @brief Get password for a user
 * @param u User structure
 * @return Pointer to the password string
 */
const char *getPassword(struct User u);

/**
 * @brief System Functions
 */

/**
 * @brief Create a new account for a user
 * @param u User structure
 */
void createNewAccount(struct User u);

/**
 * @brief Display and handle main menu
 * @param u User structure
 */
void mainMenu(struct User u);

/**
 * @brief Check all accounts for a user
 * @param u User structure
 */
void checkAllAccounts(struct User u);

/**
 * @brief Update account information
 * @param u User structure
 */
void updateAccountInformation(struct User u);

/**
 * @brief Check details of a specific account
 * @param u User structure
 */
void checkAccountDetails(struct User u);

/**
 * @brief Make a transaction (deposit/withdraw)
 * @param u User structure
 */
void makeTransaction(struct User u);

/**
 * @brief Remove an existing account
 * @param u User structure
 */
void removeAccount(struct User u);

/**
 * @brief Transfer account ownership
 * @param u User structure
 */
void transferOwnership(struct User u);

/**
 * @brief Handle failed login attempts
 * @param u Pointer to the user structure
 */
void handleFailedLogin(struct User *u);

/**
 * @brief Handle failed registration attempts
 * @param a Array containing username
 * @param pass Array containing password
 */
void handleFailedRegistration(char a[MAX_USERNAME_SIZE], char pass[MAX_PASSWORD_SIZE]);