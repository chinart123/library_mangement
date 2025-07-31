#include "user.h"
#include <stdio.h>
#include <string.h>

void user_init(User *user, int id, const char *name) {
    user->id = id;
    strncpy(user->name, name, MAX_NAME_LENGTH - 1);
    user->name[MAX_NAME_LENGTH - 1] = '\0'; // Đảm bảo kết thúc chuỗi
}

void user_print_info(const User *user) {
    printf("User ID: %d\n", user->id);
    printf("Name: %s\n", user->name);
}