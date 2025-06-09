#include <termios.h>
#include "header.h"
#include <string.h>

char *USERS = "./data/users.txt";

/**
 * @brief Login menu for user authentication
 * 
 * This function prompts the user for their username and password, disabling echo for password input.
 * It reads the input and stores it in the provided character arrays.
 * 
 * @param a Character array to store the username
 * @param pass Character array to store the password
 */
void loginMenu(char a[MAX_USERNAME_SIZE], char pass[MAX_PASSWORD_SIZE])
{
    struct termios oflags, nflags;
    char buffer[100];

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    fgets(buffer,100,stdin);
    checkBuffer(buffer);
    sscanf(buffer,"%s",a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    fgets(buffer,100,stdin);
    checkBuffer(buffer);
    sscanf(buffer,"%s",pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

/**
 * @brief Register a new user
 * 
 * This function prompts the user for a username and password, ensuring that the inputs are not empty.
 * It reads the input and stores it in the provided character arrays.
 * 
 * @param a Character array to store the username
 * @param pass Character array to store the password
 */
void registerUser(char a[MAX_USERNAME_SIZE], char pass[MAX_PASSWORD_SIZE]) 
{
    char buffer[100];
    system("clear");
name:
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t UserName:");
    fgets(buffer,100,stdin);
    checkBuffer(buffer);
    int len = strlen(buffer);
    if (len == 0) {
        printf("\n\t Please don't enter empty values\n\n");
        goto name;
    }
    sscanf(buffer,"%s",a);

pass:
    printf("\n\n\n\n\n\t\t\t\tEnter your password:");
    fgets(buffer,100,stdin);
    checkBuffer(buffer);
    len = strlen(buffer);
    if (len == 0) {
        printf("\n\t Please don't enter empty values\n\n");
        goto pass;
    }
    sscanf(buffer,"%s",pass);


}

/**
 * @brief Get the username of a user
 * 
 * This function searches the USERS file for a user with the specified username and returns it.
 * 
 * @param u User structure containing the username
 * @return The username if found, otherwise "no user found"
 */
const char *getUserName(struct User u)
{
    FILE *fp;
    char data[256];

    if ((fp = fopen(USERS, "r")) == NULL)
    {
        printf("Error! opening the file");
        exit(1);
    }

    while (fgets(data, sizeof(data), fp) != NULL)
    {
        char* piece = strtok(data, " ");
        piece = strtok(NULL, " ");
        if (strcmp(piece, u.name)==0) {
            fclose(fp);
            return piece;
        }
    }
    fclose(fp);
    return "no user found";
}

/**
 * @brief Get the password of a user
 * @param u User structure containing the username
 * @return The password of the user if found, otherwise "no user found"
 */
const char *getPassword(struct User u)
{
    FILE *fp;
    char data[256];

    if ((fp = fopen(USERS, "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fgets(data,sizeof(data), fp ) != NULL)
    {
        char* piece = strtok(data, " ");
        piece = strtok(NULL, " ");
        if (strcmp(piece, u.name) == 0) 
        {
            fclose(fp);
            piece = strtok(NULL, " ");
            piece[strcspn(piece, "\n")] = 0;
            return piece;
        } 
    }

    fclose(fp);
    return "no user found";
}

/**
 * @brief Set the ID for a new user
 * 
 * This function reads the USERS file and counts the number of lines to determine the next available ID.
 * 
 * @return The next available user ID
 */
const int setId()
{
    FILE *fp;
    char data[256];
    int count = 0;

    if ((fp = fopen(USERS, "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fgets(data,sizeof(data), fp ) != NULL)
    {
        count++; 
    }
    fclose(fp);
    return  count;   
}

/**
 * @brief Get the ID of a user based on their username
 * 
 * This function searches the USERS file for a user with the specified username and returns their ID.
 * 
 * @param u User structure containing the username
 * @return The ID of the user if found, otherwise -1
 */
const int getId(struct User u)
{
    FILE *fp;
    char data[256];
    char *initial;
    int id;

    if ((fp = fopen(USERS, "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fgets(data,sizeof(data), fp ) != NULL)
    {
        char *piece = strtok(data, " ");
        initial = piece;
        piece = strtok(NULL, " ");
        if (strcmp(u.name, piece) == 0)
        {
            id = atoi(initial);
        }


    }
    fclose(fp);
    return id;
}

/**
 * @brief Save a new user to the USERS file
 * 
 * This function appends a new user's information to the USERS file.
 * 
 * @param u Pointer to a User structure containing user information
 */
void saveUser(struct User *u)
{
    FILE *fp;
    if ((fp = fopen(USERS, "a")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }
    fprintf(fp, "%d %s %s\n", 
    u->id,
    u->name,
    u->password
    );

    fclose(fp);
}

/**
 * @brief Read a user from the USERS file
 * 
 * This function reads a user's information from the USERS file and stores it in the provided User structure.
 * 
 * @param ptr Pointer to the file stream
 * @param u Pointer to a User structure to store user information
 * @return 1 if successful, 0 if end of file is reached
 */
int getUser(FILE *ptr, struct User *u)
{
    return fscanf(ptr, "%d %s %s ",
            &u->id,
		    u->name,
		    u->password) != EOF;
}