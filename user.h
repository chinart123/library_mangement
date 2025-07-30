#ifndef USER_H
#define USER_H

#define MAX_NAME_LENGTH 100

typedef struct {
    int id;                // Unique user identifier
    char name[MAX_NAME_LENGTH]; // User name
} User;

// Function to initialize a user
void user_init(User *user, int id, const char *name);

// Function to print user information
void user_print_info(const User *user);

#endif