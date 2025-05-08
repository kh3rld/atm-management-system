#include "header.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/atm_notifications"

const char *RECORDS = "./data/records.txt";

/// @brief Reads the account information from a file.
/// @param ptr 
/// @param name 
/// @param r 
/// @return 
int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
                  &r->userId,
                  name,
                  &r->accountNumber,
                  &r->depositDate.month,
                  &r->depositDate.day,
                  &r->depositDate.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

/// @brief Saves the account information to a file.
/// @param ptr 
/// @param u 
/// @param r
void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
            r.id,
            u.id,
            u.name,
            r.accountNumber,
            r.depositDate.month,
            r.depositDate.day,
            r.depositDate.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

/// @brief  Displays a message indicating that the record was not found and prompts the user to try again or return to the main menu.
/// @param notGood Indicates whether the record was found or not.
/// @param f The function to call if the user chooses to try again.
/// @param u The user currently logged in.
void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

/// @brief Displays a success message and prompts the user to return to the main menu or exit.
/// @param u The user currently logged in.
void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

/// @brief Creates a new account for the user.
/// @param u The user currently logged in.
void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");
    if (!pf)
    {
        perror("Error opening records file");
        return;
    }

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy): ");
    scanf("%d/%d/%d", &r.depositDate.month, &r.depositDate.day, &r.depositDate.year);
    printf("\nEnter the account number: ");
    scanf("%d", &r.accountNumber);

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNumber == r.accountNumber)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country: ");
    scanf("%s", r.country);
    printf("\nEnter the phone number: ");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice: ");
    scanf("%s", r.accountType);

    saveAccountToFile(pf, u, r);
    fclose(pf);
    success(u);
}

/// @brief Displays all accounts associated with a specific user.
/// @param u The user currently logged in.
void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;
    FILE *pf = fopen(RECORDS, "r");
    if (!pf)
    {
        perror("Error opening records file");
        return;
    }

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number: %d\nDeposit Date: %d/%d/%d\nCountry: %s\nPhone number: %d\nAmount deposited: $%.2f\nType Of Account: %s\n",
                   r.accountNumber,
                   r.depositDate.day,
                   r.depositDate.month,
                   r.depositDate.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

/// @brief Registers a new user.
/// @param name 
/// @param password
void registerMenu(char name[50], char password[50])
{
    struct User newUser;
    registerUser(&newUser);
}

/// @brief Registers a new user.
/// @param newUser The user to be registered.
/// @param name The name of the user.
/// @param password The password of the user.
void registerUser(struct User *newUser)
{
    FILE *pf = fopen("./data/users.txt", "r+");
    if (!pf)
    {
        perror("Unable to open users file");
        return;
    }

    struct User existingUser;
    char tempName[50];
    int maxId = -1;

    printf("Enter your name: ");
    scanf("%s", tempName);

    int userExists = 0;
    while (fscanf(pf, "%d %s %s", &existingUser.id, existingUser.name, existingUser.password) != EOF)
    {
        if (strcmp(existingUser.name, tempName) == 0)
        {
            printf("User with this name already exists. Please choose another name.\n");
            userExists = 1;
            break;
        }
    }

    if (userExists)
    {
        fclose(pf);
        return;
    }

    strcpy(newUser->name, tempName);
    printf("Enter your password: ");
    scanf("%s", newUser->password);

    newUser->id = 0;
    while (fscanf(pf, "%d %s %s", &existingUser.id, existingUser.name, existingUser.password) != EOF)
    {
        if (existingUser.id >= newUser->id)
        {
            newUser->id = existingUser.id + 1;
        }
    }

    fprintf(pf, "%d %s %s\n", newUser->id, newUser->name, newUser->password);
    fclose(pf);
    printf("Registration successful! You can now log in.\n");
}

/// @brief Updates the account information for a specific user.
/// @param u The user currently logged in.
/// @param r The record of the account to be updated.
/// @param userName The name of the user associated with the account.
/// @param notGood Indicates whether the account was found or not.
/// @param pf The file pointer to the records file.
/// @param tempFile The file pointer to the temporary file.
/// @param cr The record of the account to be updated.
void updateAccountInfo(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "r+");
    if (!pf)
    {
        perror("Error opening records file");
        return;
    }
    system("clear");
    printf("\t\t\t===== Update account info =====\n");
    printf("\nEnter the account number: ");
    scanf("%d", &r.accountNumber);
    int notGood = 0;
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNumber == r.accountNumber)
        {
            notGood = 1;
            break;
        }
    }
    if (notGood == 0)
    {
        stayOrReturn(notGood, updateAccountInfo, u);
        fclose(pf);
        return;
    }
    printf("\nEnter the new country: ");
    scanf("%s", r.country);
    printf("\nEnter the new phone number: ");
    scanf("%d", &r.phone);
    printf("\nEnter the new amount to deposit: $");
    scanf("%lf", &r.amount); 
    printf("\nChoose the new type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice: ");
    scanf("%s", r.accountType);
    fseek(pf, 0, SEEK_SET);
    FILE *tempFile = fopen("./data/temp.txt", "w");
    if (!tempFile)
    {
        perror("Error opening temporary file");
        fclose(pf);
        return;
    }
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNumber == r.accountNumber)
        {
            saveAccountToFile(tempFile, u, r);
        }
        else
        {
            saveAccountToFile(tempFile, u, cr);
        }
    }
    fclose(pf);
    fclose(tempFile);
    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);
    success(u);
    fclose(pf);
    fclose(tempFile);
}
/// @brief Displays the details of a specific account.
/// @param u The user currently logged in.
/// @param r The record of the account to be displayed.
/// @param userName The name of the user associated with the account.
void checkAccountDetails(struct User u)
{
    struct Record r;
    char userName[50];
    FILE *pf = fopen(RECORDS, "r");
    if (!pf)
    {
        perror("Error opening records file");
        return;
    }
    system("clear");
    printf("\t\t\t===== Check account details =====\n");
    printf("\nEnter the account number: ");
    scanf("%d", &r.accountNumber);
    int notGood = 0;
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNumber == r.accountNumber)
        {
            notGood = 1;
            break;
        }
    }
    if (notGood == 0)
    {
        stayOrReturn(notGood, checkAccountDetails, u);
        fclose(pf);
        return;
    }
    printf("\nAccount number: %d\nDeposit Date: %d/%d/%d\nCountry: %s\nPhone number: %d\nAmount deposited: $%.2f\nType Of Account: %s\n",
           r.accountNumber,
           r.depositDate.day,
           r.depositDate.month,
           r.depositDate.year,
           r.country,
           r.phone,
           r.amount,
           r.accountType);

    // Calculate and display interest for savings and fixed accounts
    if (strcmp(r.accountType, "saving") == 0)
    {
        double interest = r.amount * 0.07;
        printf("Interest (7%% annual): $%.2f\n", interest);
    }
    else if (strcmp(r.accountType, "fixed01") == 0)
    {
        double interest = r.amount * 0.04;
        printf("Interest (4%% annual): $%.2f\n", interest);
    }
    else if (strcmp(r.accountType, "fixed02") == 0)
    {
        double interest = r.amount * 0.05;
        printf("Interest (5%% annual): $%.2f\n", interest);
    }
    else if (strcmp(r.accountType, "fixed03") == 0)
    {
        double interest = r.amount * 0.08;
        printf("Interest (8%% annual): $%.2f\n", interest);
    }
    else if (strcmp(r.accountType, "current") == 0)
    {
        printf("No interest is earned on current accounts.\n");
    }
    else
    {
        printf("Unknown account type.\n");
    }
    
    // Calculate monthly interest based on account type
    double monthlyInterest = 0.0;
    if (strcmp(r.accountType, "saving") == 0) {
        monthlyInterest = (r.amount * 0.07) / 12;
        printf("\nYou will get $%.2f as interest on day %d of every month\n", 
               monthlyInterest, r.depositDate.day);
    }
    else if (strcmp(r.accountType, "fixed01") == 0) {
        monthlyInterest = (r.amount * 0.04) / 12;
        printf("\nYou will get $%.2f as interest after 1 year on %d/%d/%d\n",
               monthlyInterest * 12, r.depositDate.day, r.depositDate.month, r.depositDate.year + 1);
    }
    else if (strcmp(r.accountType, "fixed02") == 0) {
        monthlyInterest = (r.amount * 0.05) / 12;
        printf("\nYou will get $%.2f as interest after 2 years on %d/%d/%d\n",
               monthlyInterest * 24, r.depositDate.day, r.depositDate.month, r.depositDate.year + 2);
    }
    else if (strcmp(r.accountType, "fixed03") == 0) {
        monthlyInterest = (r.amount * 0.08) / 12;
        printf("\nYou will get $%.2f as interest after 3 years on %d/%d/%d\n",
               monthlyInterest * 36, r.depositDate.day, r.depositDate.month, r.depositDate.year + 3);
    }
    else if (strcmp(r.accountType, "current") == 0) {
        printf("\nYou will not get interests because the account is of type current\n");
    }

    fclose(pf);
}

// /// @brief Displays the main menu for the ATM management system.
// /// @param u The user currently logged in.
// void mainMenu(struct User u)
// {
//     int choice;
//     system("clear");
//     printf("\t\t\t===== ATM Management System =====\n");
//     printf("\nWelcome, %s!\n", u.name);
//     printf("\n1. Create New Account");
//     printf("\n2. Check All Accounts");
//     printf("\n3. Check Account Details");
//     printf("\n4. Update Account Info");
//     printf("\n5. Make a Transaction");
//     printf("\n6. Remove Account");
//     printf("\n7. Transfer Ownership");
//     printf("\n8. Exit");
//     printf("\n\nEnter your choice: ");
//     scanf("%d", &choice);

//     switch (choice)
//     {
//     case 1:
//         createNewAcc(u);
//         break;
//     case 2:
//         checkAllAccounts(u);
//         break;
//     case 3:
//         checkAccountDetails(u);
//         break;
//     case 4:
//         updateAccountInfo(u);
//         break;
//     case 5:
//         makeTransaction(u);
//         break;
//     case 6:
//         removeAccount(u);
//         break;
//     case 7:
//         transferOwnership(u);
//         break;
//     case 8:
//         system("clear");
//         printf("Thank you for using the ATM Management System. Goodbye!\n");
//         exit(0);
//     default:
//         printf("Invalid choice! Please try again.\n");
//         mainMenu(u);
//     }
// }

/// @brief Displays the login menu and prompts the user for their credentials.
/// @param u 
void makeTransaction(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "r+");
    if (!pf)
    {
        perror("Error opening records file");
        return;
    }
    system("clear");
    printf("\t\t\t===== Make a transaction =====\n");
    printf("\nEnter the account number: ");
    scanf("%d", &r.accountNumber);
    int notGood = 0;
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNumber == r.accountNumber)
        {
            notGood = 1;
            break;
        }
    }
    if (notGood == 0)
    {
        stayOrReturn(notGood, makeTransaction, u);
        fclose(pf);
        return;
    }

    // Add restriction for fixed accounts
    if (strncmp(cr.accountType, "fixed", 5) == 0)
    {
        printf("\n✖ Transactions are not allowed for fixed deposit accounts!\n");
        fclose(pf);
        stayOrReturn(0, makeTransaction, u);
        return;
    }

    printf("\nEnter the amount to withdraw: $");
    scanf("%lf", &r.amount);
    printf("\nEnter today's date(mm/dd/yyyy): ");
    scanf("%d/%d/%d", &r.withdrawDate.month, &r.withdrawDate.day, &r.withdrawDate.year);
    fseek(pf, 0, SEEK_SET);
    FILE *tempFile = fopen("./data/temp.txt", "w");
    if (!tempFile)
    {
        perror("Error opening temporary file");
        fclose(pf);
        return;
    }
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNumber == r.accountNumber)
        {
            cr.amount -= r.amount;
            saveAccountToFile(tempFile, u, cr);
        }
        else
        {
            saveAccountToFile(tempFile, u, cr);
        }
    }
    fclose(pf);
    fclose(tempFile);
    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);
    success(u);
    fclose(pf);
    fclose(tempFile);
}

/// @brief Displays the login menu and prompts the user for their credentials.
/// @param u 
void removeAccount(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "r+");
    if (!pf)
    {
        perror("Error opening records file");
        return;
    }
    system("clear");
    printf("\t\t\t===== Remove account =====\n");
    printf("\nEnter the account number: ");
    scanf("%d", &r.accountNumber);
    int notGood = 0;
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNumber == r.accountNumber)
        {
            notGood = 1;
            break;
        }
    }
    if (notGood == 0)
    {
        stayOrReturn(notGood, removeAccount, u);
        fclose(pf);
        return;
    }
    fseek(pf, 0, SEEK_SET);
    FILE *tempFile = fopen("./data/temp.txt", "w");
    if (!tempFile)
    {
        perror("Error opening temporary file");
        fclose(pf);
        return;
    }
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) != 0 || cr.accountNumber != r.accountNumber)
        {
            saveAccountToFile(tempFile, u, cr);
        }
    }
    fclose(pf);
    fclose(tempFile);
    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);
    success(u);
    fclose(pf);
    fclose(tempFile);
}

/// @brief Transfers ownership of an account to a new user.
/// @param u The user currently logged in.
/// @param r The record of the account to be transferred.
/// @param userName The name of the user associated with the account.
/// @param notGood Indicates whether the account was found or not.
/// @param pf The file pointer to the records file.
/// @param tempFile The file pointer to the temporary file.
void transferOwnership(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "r+");
    if (!pf)
    {
        perror("Error opening records file");
        return;
    }
    system("clear");
    printf("\t\t\t===== Transfer ownership =====\n");
    printf("\nEnter the account number: ");
    scanf("%d", &r.accountNumber);
    int notGood = 0;
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNumber == r.accountNumber)
        {
            notGood = 1;
            break;
        }
    }
    if (notGood == 0)
    {
        stayOrReturn(notGood, transferOwnership, u);
        fclose(pf);
        return;
    }
    
    // Verify new owner exists
    FILE *userFile = fopen("./data/users.txt", "r");
    if (!userFile) {
        perror("Error opening users file");
        fclose(pf);
        return;
    }

    struct User newOwner;
    int userFound = 0;
    char newOwnerName[50];
    
    printf("\nEnter the new owner name: ");
    scanf("%s", newOwnerName);
    
    while (fscanf(userFile, "%d %s %s", &newOwner.id, newOwner.name, newOwner.password) != EOF) {
        if (strcmp(newOwner.name, newOwnerName) == 0) {
            userFound = 1;
            break;
        }
    }
    fclose(userFile);

    if (!userFound) {
        printf("\n✖ New owner not found!\n");
        stayOrReturn(0, transferOwnership, u);
        fclose(pf);
        return;
    }

    // Send notification through pipe
    int notification_fd = open(FIFO_PATH, O_WRONLY | O_NONBLOCK);
    if (notification_fd >= 0) {
        char notification[256];
        snprintf(notification, sizeof(notification), 
                "User %s transferred account %d to you\n", 
                u.name, r.accountNumber);
        write(notification_fd, notification, strlen(notification));
        close(notification_fd);
    }

    // Update ownership in records
    fseek(pf, 0, SEEK_SET);
    FILE *tempFile = fopen("./data/temp.txt", "w");
    if (!tempFile)
    {
        perror("Error opening temporary file");
        fclose(pf);
        return;
    }

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNumber == r.accountNumber)
        {
            cr.userId = newOwner.id;
            saveAccountToFile(tempFile, newOwner, cr);
        }
        else
        {
            saveAccountToFile(tempFile, u, cr);
        }
    }

    fclose(pf);
    fclose(tempFile);
    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);
    success(u);
}

// Add this function to handle notifications
void setupNotificationSystem() {
    mkfifo(FIFO_PATH, 0666);
}

void checkNotifications(struct User u) {
    int notification_fd = open(FIFO_PATH, O_RDONLY | O_NONBLOCK);
    if (notification_fd >= 0) {
        char buffer[256];
        ssize_t bytes_read = read(notification_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("\n🔔 New Notification:\n%s\n", buffer);
        }
        close(notification_fd);
    }
}