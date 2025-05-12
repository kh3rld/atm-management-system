#include "header.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

const char *RECORDS = "./data/records.txt";

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

/// @brief Registers a new user.
/// @param name
/// @param password
void registerMenu(char a[50], char pass[50])
{
    FILE *fp;
    struct User userChecker;
    struct User newUser;
    int lastId = -1;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Registration:");

    // Check if username exists
    if ((fp = fopen("./data/users.txt", "r+")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    // Find last ID and check if username exists
    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        lastId = userChecker.id;
        if (strcmp(userChecker.name, a) == 0)
        {
            fclose(fp);
            printf("\n\nUsername already exists!\n");
            exit(1);
        }
    }

    // Get new user details
    printf("\nEnter username: ");
    scanf("%s", newUser.name);
    printf("\nEnter password: ");
    scanf("%s", newUser.password);

    // Write new user to file
    newUser.id = lastId + 1;
    fprintf(fp, "\n%d %s %s", newUser.id, newUser.name, newUser.password);

    fclose(fp);

    // Copy to output parameters
    strcpy(a, newUser.name);
    strcpy(pass, newUser.password);

    printf("\nRegistration successful!\n");
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
    FILE *fp, *temp;
    struct Record r;
    char userName[50];
    int accountNbr, choice;
    int found = 0;

    system("clear");
    printf("\n\t\t\t===== Update Account Information =====\n");
    printf("\nEnter account number to update: ");
    scanf("%d", &accountNbr);

    fp = fopen(RECORDS, "r");
    temp = fopen("./data/temp.txt", "w");

    while (getAccountFromFile(fp, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr)
        {
            found = 1;
            printf("\nAccount found! What would you like to update?");
            printf("\n1. Country");
            printf("\n2. Phone number");
            printf("\nEnter your choice: ");
            scanf("%d", &choice);

            if (choice == 1)
            {
                printf("\nEnter new country: ");
                scanf("%s", r.country);
            }
            else if (choice == 2)
            {
                printf("\nEnter new phone number: ");
                scanf("%d", &r.phone);
            }
            else
            {
                printf("\nInvalid choice!");
            }
        }
        saveAccountToFile(temp, u, r);
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove(RECORDS);
        rename("./data/temp.txt", RECORDS);
        printf("\nAccount updated successfully!");
    }
    else
    {
        remove("./data/temp.txt");
        printf("\nAccount not found!");
    }

    stayOrReturn(found, updateAccountInfo, u);
}

/// @brief Displays the details of a specific account.
/// @param u The user currently logged in.
/// @param r The record of the account to be displayed.
/// @param userName The name of the user associated with the account.
void checkAccountDetails(struct User u)
{
    FILE *fp;
    struct Record r;
    char userName[50];
    int accountNbr, found = 0;
    double interest;

    system("clear");
    printf("\n\t\t\t===== Check Account Details =====\n");
    printf("\nEnter account number to check: ");
    scanf("%d", &accountNbr);

    fp = fopen(RECORDS, "r");

    while (getAccountFromFile(fp, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr)
        {
            found = 1;
            printf("\n_____________________\n");
            printf("\nAccount number: %d", r.accountNbr);
            printf("\nDeposit Date: %d/%d/%d", r.deposit.month, r.deposit.day, r.deposit.year);
            printf("\nCountry: %s", r.country);
            printf("\nPhone number: %d", r.phone);
            printf("\nAmount deposited: $%.2f", r.amount);
            printf("\nType of Account: %s", r.accountType);

            // Calculate interest
            if (strcmp(r.accountType, "savings") == 0)
            {
                interest = r.amount * 0.07;
                printf("\n\nYou will get $%.2f as interest on day %d of every month", interest, r.deposit.day);
            }
            else if (strcmp(r.accountType, "fixed01") == 0)
            {
                interest = r.amount * 0.04;
                printf("\n\nYou will get $%.2f as interest after 1 year", interest);
            }
            else if (strcmp(r.accountType, "fixed02") == 0)
            {
                interest = r.amount * 0.05;
                printf("\n\nYou will get $%.2f as interest after 2 years", interest);
            }
            else if (strcmp(r.accountType, "fixed03") == 0)
            {
                interest = r.amount * 0.08;
                printf("\n\nYou will get $%.2f as interest after 3 years", interest);
            }
            else if (strcmp(r.accountType, "current") == 0)
            {
                printf("\n\nYou will not get interests because the account is of type current");
            }
            break;
        }
    }

    fclose(fp);
    stayOrReturn(!found, checkAccountDetails, u);
}

/// @brief Displays the login menu and prompts the user for their credentials.
/// @param u
void makeTransaction(struct User u)
{
    FILE *fp, *temp;
    struct Record r;
    char userName[50];
    int accountNbr, choice, found = 0;
    double amount;

    system("clear");
    printf("\n\t\t\t===== Make Transaction =====\n");
    printf("\nEnter account number: ");
    scanf("%d", &accountNbr);

    fp = fopen(RECORDS, "r");
    temp = fopen("./data/temp.txt", "w");

    while (getAccountFromFile(fp, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr)
        {
            found = 1;

            // Check if account is fixed
            if (strstr(r.accountType, "fixed") != NULL)
            {
                printf("\nCannot make transactions on fixed accounts!");
                fclose(fp);
                fclose(temp);
                remove("./data/temp.txt");
                stayOrReturn(0, makeTransaction, u);
                return;
            }

            printf("\n1. Deposit");
            printf("\n2. Withdraw");
            printf("\nEnter your choice: ");
            scanf("%d", &choice);

            printf("\nEnter amount: ");
            scanf("%lf", &amount);

            if (choice == 1)
            {
                r.amount += amount;
                printf("\nDeposit successful!");
            }
            else if (choice == 2)
            {
                if (r.amount >= amount)
                {
                    r.amount -= amount;
                    printf("\nWithdrawal successful!");
                }
                else
                {
                    printf("\nInsufficient funds!");
                }
            }
            else
            {
                printf("\nInvalid choice!");
            }
        }
        saveAccountToFile(temp, u, r);
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove(RECORDS);
        rename("./data/temp.txt", RECORDS);
    }
    else
    {
        remove("./data/temp.txt");
        printf("\nAccount not found!");
    }

    stayOrReturn(!found, makeTransaction, u);
}

/// @brief Displays the login menu and prompts the user for their credentials.
/// @param u
void removeAccount(struct User u)
{
    FILE *fp, *temp;
    struct Record r;
    char userName[50];
    int accountNbr, found = 0;

    system("clear");
    printf("\n\t\t\t===== Remove Account =====\n");
    printf("\nEnter account number to remove: ");
    scanf("%d", &accountNbr);

    fp = fopen(RECORDS, "r");
    temp = fopen("./data/temp.txt", "w");

    while (getAccountFromFile(fp, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr)
        {
            found = 1;
            printf("\nAccount found and removed!");
            continue; // Skip writing this record to temp file
        }
        saveAccountToFile(temp, u, r);
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove(RECORDS);
        rename("./data/temp.txt", RECORDS);
    }
    else
    {
        remove("./data/temp.txt");
        printf("\nAccount not found!");
    }

    stayOrReturn(!found, removeAccount, u);
}

// Helper function to get the next available user ID
int getNextUserId() {
    FILE *fp = fopen("./data/users.txt", "r");
    if (!fp) return 0; // Start from 0 if file doesn't exist
    
    int maxId = -1;
    struct User temp;
    while (fscanf(fp, "%d %s %s", &temp.id, temp.name, temp.password) != EOF) {
        if (temp.id > maxId) maxId = temp.id;
    }
    fclose(fp);
    return maxId + 1;
}

// Helper function to get the next available record ID
int getNextRecordId() {
    FILE *fp = fopen(RECORDS, "r");
    if (!fp) return 0;
    
    int maxId = -1;
    struct Record r;
    char userName[50];
    
    while (getAccountFromFile(fp, userName, &r)) {
        if (r.id > maxId) maxId = r.id;
    }
    fclose(fp);
    return maxId + 1;
}

// Helper function to check if account number exists for user
int accountNumberExists(int userId, int accountNbr) {
    FILE *fp = fopen(RECORDS, "r");
    if (!fp) return 0;
    
    struct Record r;
    char name[50];
    while (getAccountFromFile(fp, name, &r)) {
        if (r.userId == userId && r.accountNbr == accountNbr) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r) {
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                &r->id,
                &r->userId,
                name,
                &r->accountNbr,
                &r->deposit.month,
                &r->deposit.day,
                &r->deposit.year,
                r->country,
                &r->phone,
                &r->amount,
                r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r) {
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
            r.id,
            u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void createNewAcc(struct User u) {
    struct Record r;
    struct Record cr;
    char userName[50];
    
    // Get next available record ID
    r.id = getNextRecordId();
    
    FILE *pf = fopen(RECORDS, "a+");
    if (!pf) {
        perror("Error opening records file");
        return;
    }

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy): ");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    
    printf("\nEnter the account number: ");
    scanf("%d", &r.accountNbr);
    
    // Check if account number already exists for this user
    if (accountNumberExists(u.id, r.accountNbr)) {
        printf("✖ This Account already exists for this user\n\n");
        goto noAccount;
    }

    printf("\nEnter the country: ");
    scanf("%99s", r.country);  // Limit input to prevent buffer overflow
    
    while (1) {
        printf("\nEnter phone number (digits only): ");
        if (scanf("%d", &r.phone) == 1 && r.phone > 0) {
            break;
        }
        printf("Invalid input! Please enter numbers only.\n");
        while (getchar() != '\n'); // Clear input buffer
    }
    
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice: ");
    scanf("%9s", r.accountType);  // Limit input to prevent buffer overflow

    saveAccountToFile(pf, u, r);
    fclose(pf);
    success(u);
}

void registerUser(struct User *newUser) {
    FILE *fp = fopen("./data/users.txt", "a+");
    if (!fp) {
        perror("Unable to open users file");
        return;
    }

    // Get next available user ID
    newUser->id = getNextUserId();
    
    printf("Enter your name: ");
    scanf("%49s", newUser->name);  // Limit input to prevent buffer overflow
    
    // Check if username exists
    struct User existingUser;
    rewind(fp);  // Go back to start of file
    while (fscanf(fp, "%d %s %s", &existingUser.id, existingUser.name, existingUser.password) != EOF) {
        if (strcmp(existingUser.name, newUser->name) == 0) {
            printf("User with this name already exists. Please choose another name.\n");
            fclose(fp);
            return;
        }
    }

    printf("Enter your password: ");
    scanf("%49s", newUser->password);  // Limit input to prevent buffer overflow
    
    // Write new user to file
    fprintf(fp, "%d %s %s\n", newUser->id, newUser->name, newUser->password);
    fclose(fp);
    printf("Registration successful! You can now log in.\n");
}

void transferOwnership(struct User u) {
    FILE *fp, *temp, *userFile;
    struct Record r;
    struct User newOwner;
    char userName[50], newOwnerName[50];
    int accountNbr, found = 0, userFound = 0;

    system("clear");
    printf("\n\t\t\t===== Transfer Ownership =====\n");
    printf("\nEnter account number to transfer: ");
    scanf("%d", &accountNbr);

    // First verify new owner exists
    printf("\nEnter new owner's username: ");
    scanf("%49s", newOwnerName);

    userFile = fopen("../data/users.txt", "r");
    while (userFile && fscanf(userFile, "%d %s %s", &newOwner.id, newOwner.name, newOwner.password) != EOF) {
        if (strcmp(newOwner.name, newOwnerName) == 0) {
            userFound = 1;
            break;
        }
    }
    if (userFile) fclose(userFile);

    if (!userFound) {
        printf("\nNew owner not found!");
        stayOrReturn(0, transferOwnership, u);
        return;
    }

    fp = fopen(RECORDS, "r");
    temp = fopen("./data/temp.txt", "w");

    while (fp && temp && getAccountFromFile(fp, userName, &r)) {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr) {
            found = 1;
            // Change the user info to new owner
            strcpy(userName, newOwner.name);
            r.userId = newOwner.id;
        }
        saveAccountToFile(temp, (found && strcmp(userName, newOwner.name) == 0) ? newOwner : u, r);
    }

    if (fp) fclose(fp);
    if (temp) fclose(temp);

    if (found) {
        remove(RECORDS);
        rename("./data/temp.txt", RECORDS);
        printf("\nOwnership transferred successfully!");
        
        #define FIFO_PATH "/tmp/atm_notifications"

        // Bonus: Notify new owner via FIFO
        int fd = open(FIFO_PATH, O_WRONLY | O_NONBLOCK);
        if (fd != -1) {
            char msg[256];
            snprintf(msg, sizeof(msg), "Account %d transferred to you by %s", 
                    accountNbr, u.name);
            write(fd, msg, strlen(msg)+1);
            close(fd);
        }
    } else {
        remove("./data/temp.txt");
        printf("\nAccount not found!");
    }

    stayOrReturn(!found, transferOwnership, u);
}
