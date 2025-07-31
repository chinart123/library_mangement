#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdbool.h>
#include "book.h"
#include "user.h"

#define MAX_TRANSACTIONS 100
#define MAX_BOOKS_PER_USER 3
typedef struct {
    int id;                // Unique transaction identifier
    Book *book;            // Pointer to the borrowed book
    User *user;            // Pointer to the borrowing user
    bool is_active;        // True if book is currently borrowed
} Transaction;

// Initialize transaction system
void transaction_init(Transaction transactions[], int *count);

// Borrow a book
int transaction_borrow(Transaction transactions[], int *count, Book *book, User *user);

// Return a book
int transaction_return(Transaction transactions[], int *count, Book *book, User *user);

// Print active transactions
void transaction_print_active(const Transaction transactions[], int count);

// Print all transactions for a user
void transaction_print_user_history(const Transaction transactions[], int count, const User *user);

#endif