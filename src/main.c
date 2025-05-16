/**
 * @file main.c
 * @brief Main implementation file for the ATM Management System
 * @author Khalid Hussein
 * @date 2025
 * 
 * This file contains the main entry point and core menu functions for the
 * ATM Management System. It handles user interaction and navigation through
 * the system's various features.
 */

#include "header.h"

/**
 * @brief Display and handle the main menu options
 * 
 * This function displays the main menu of the ATM system and handles user
 * input for various operations like creating accounts, making transactions,
 * and managing account information.
 * 
 * @param u User structure containing the current user's information
 */
void mainMenu(struct User u)
{
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to choose one of the options below, %s <<--\n",
           u.name);
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n\n\n");
    
    if (!safeReadInt(&option)) {
        printf("\n\t\t✖ Invalid input. Please enter a number.\n");
        mainMenu(u);
        return;
    }

    switch (option)
    {
    case 1:
        createNewAccount(u);
        break;
    case 2:
        updateAccountInformation(u);
        break;
    case 3:
        checkAccountDetails(u);
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        makeTransaction(u);
        break;
    case 6:
        removeAccount(u);
        break;
    case 7:
        transferOwnership(u);
        break;
    case 8:
        printf("\n\t\tThank you for using our ATM system!\n");
        exit(0);
        break;
    default:
        printf("\n\t\t✖ Invalid operation! Please try again.\n");
        sleep(1);
        mainMenu(u);
        break;
    }
}

/**
 * @brief Display and handle the initialization menu
 * 
 * This function displays the initial menu where users can choose to login,
 * register, or exit the system. It handles user authentication and
 * registration processes.
 * 
 * @param u Pointer to the user structure to store user information
 */
void initMenu(struct User *u)
{
    int r = 0;
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n\n\n");
    
    while (!r)
    {
        if (!safeReadInt(&option)) {
            printf("\n\t\t✖ Invalid input. Please enter a number.\n");
            continue;
        }
        
        switch (option)
        {
        case 1:
            loginMenu(u->name, u->password);
            if (strcmp(u->password, getPassword(*u)) == 0)
            {
                r = 1;
            }
            else
            {
                printf("\n\t\t✖ Wrong password or username\n");
                handleFailedLogin(u);
            }
            break;
        case 2:
            registerMenu(u->name, u->password);
            r = 1;
            break;
        case 3:
            printf("\n\t\tThank you for using our ATM system!\n");
            exit(0);
            break;
        default:
            printf("\n\t\t✖ Invalid operation! Please try again.\n");
            break;
        }
    }
}

/**
 * @brief Main entry point of the application
 * 
 * This function initializes the system, creates necessary directories and files,
 * and starts the user interaction process.
 * 
 * @return 0 on successful execution
 */
int main()
{
    // Initialize the system
    initSystem();
    
    struct User u;
    initMenu(&u);
    mainMenu(u);
    return 0;
}