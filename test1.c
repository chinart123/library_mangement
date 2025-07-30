#include <stdio.h>
#include "book.h"
#include "user.h"
#include "transaction.h"

int main() {
    // Initialize books (10 books as requested)
    Book books[10];
    for (int i = 0; i < 10; i++) {
        char title[20];
        sprintf(title, "Book %d", i+1);
        book_init(&books[i], i+1, title, "Author");
    }

    // Initialize users (5 users with different borrowing status)
    User users[5];
    user_init(&users[0], 1, "User 1");  // Can borrow (0 books)
    user_init(&users[1], 2, "User 2");  // Can borrow (1 book)
    user_init(&users[2], 3, "User 3");  // At limit (2 books)
    user_init(&users[3], 4, "User 4");  // Can't borrow (3 books)
    user_init(&users[4], 5, "User 5");  // Can borrow (0 books)

    // Initialize transaction system
    Transaction transactions[MAX_TRANSACTIONS];
    int transaction_count = 0;
    transaction_init(transactions, &transaction_count);

    // Pre-set some borrowed books for testing
    // User 2 has 1 book (Book 2)
    transaction_borrow(transactions, &transaction_count, &books[1], &users[1]);
    // User 3 has 2 books (Book 3, Book 4)
    transaction_borrow(transactions, &transaction_count, &books[2], &users[2]);
    transaction_borrow(transactions, &transaction_count, &books[3], &users[2]);
    // User 4 has 3 books (Book 5, Book 6, Book 7)
    transaction_borrow(transactions, &transaction_count, &books[4], &users[3]);
    transaction_borrow(transactions, &transaction_count, &books[5], &users[3]);
    transaction_borrow(transactions, &transaction_count, &books[6], &users[3]);

    printf("===== INITIAL STATE =====\n");
    transaction_print_active(transactions, transaction_count);

    // ===== TEST BORROWING =====
    printf("\n===== TEST BORROWING =====\n");

    // Test 1: User 1 borrows Book 1 (Success)
    printf("\n[TEST 1] User 1 borrows Book 1 (Should succeed):\n");
    transaction_borrow(transactions, &transaction_count, &books[0], &users[0]);

    // Test 2: User 1 borrows Book 1 again (Fail - already borrowed)
    printf("\n[TEST 2] User 1 borrows Book 1 again (Should fail):\n");
    transaction_borrow(transactions, &transaction_count, &books[0], &users[0]);

    // Test 3: User 2 borrows Book 2 (Success)
    printf("\n[TEST 3] User 2 borrows Book 2 (Should succeed):\n");
    transaction_borrow(transactions, &transaction_count, &books[1], &users[1]);

    // Test 4: User 3 borrows Book 5 (Fail - at limit)
    printf("\n[TEST 4] User 3 borrows Book 5 (Should fail - limit reached):\n");
    transaction_borrow(transactions, &transaction_count, &books[4], &users[2]);

    // Test 5: User 4 borrows Book 8 (Fail - can't borrow more)
    printf("\n[TEST 5] User 4 borrows Book 8 (Should fail - over limit):\n");
    transaction_borrow(transactions, &transaction_count, &books[7], &users[3]);

    // Test 6: User 5 borrows Book 100 (Fail - book doesn't exist)
    printf("\n[TEST 6] User 5 borrows Book 100 (Should fail - invalid book):\n");
    // Note: We'll simulate this by passing NULL as book
    transaction_borrow(transactions, &transaction_count, NULL, &users[4]);

    // Test 7: User 100 borrows Book 5 (Fail - user doesn't exist)
    printf("\n[TEST 7] User 100 borrows Book 5 (Should fail - invalid user):\n");
    // Note: We'll simulate this by passing NULL as user
    transaction_borrow(transactions, &transaction_count, &books[4], NULL);

    // Test 8: User 3 borrows Book 1 (Fail - already borrowed by User 1)
    printf("\n[TEST 8] User 3 borrows Book 1 (Should fail - book taken):\n");
    transaction_borrow(transactions, &transaction_count, &books[0], &users[2]);

    // ===== TEST RETURNING =====
    printf("\n===== TEST RETURNING =====\n");

    // Test 9: User 1 returns Book 1 (Success)
    printf("\n[TEST 9] User 1 returns Book 1 (Should succeed):\n");
    transaction_return(transactions, &transaction_count, &books[0], &users[0]);

    // Test 10: User 1 returns Book 1 again (Fail - already returned)
    printf("\n[TEST 10] User 1 returns Book 1 again (Should fail):\n");
    transaction_return(transactions, &transaction_count, &books[0], &users[0]);

    // Test 11: User 2 returns Book 3 (Fail - didn't borrow this)
    printf("\n[TEST 11] User 2 returns Book 3 (Should fail - wrong book):\n");
    transaction_return(transactions, &transaction_count, &books[2], &users[1]);

    // Test 12: User 5 returns Book 2 (Fail - didn't borrow any)
    printf("\n[TEST 12] User 5 returns Book 2 (Should fail - no books):\n");
    transaction_return(transactions, &transaction_count, &books[1], &users[4]);

    // Test 13: User 4 returns Book 100 (Fail - book doesn't exist)
    printf("\n[TEST 13] User 4 returns Book 100 (Should fail - invalid book):\n");
    transaction_return(transactions, &transaction_count, NULL, &users[3]);

    // Test 14: User 100 returns Book 5 (Fail - user doesn't exist)
    printf("\n[TEST 14] User 100 returns Book 5 (Should fail - invalid user):\n");
    transaction_return(transactions, &transaction_count, &books[4], NULL);

    // Test 15: User 2 returns Book 2 (Success)
    printf("\n[TEST 15] User 2 returns Book 2 (Should succeed):\n");
    transaction_return(transactions, &transaction_count, &books[1], &users[1]);

    // ===== SPECIAL CASES =====
    printf("\n===== SPECIAL TESTS =====\n");

    // Test 16: User 3 borrows Book 1 after return (Success)
    printf("\n[TEST 16] User 3 borrows Book 1 after return (Should succeed):\n");
    transaction_borrow(transactions, &transaction_count, &books[0], &users[2]);

    // Test 17: User 3 borrows Book 5 (Fail - at limit)
    printf("\n[TEST 17] User 3 borrows Book 5 (Should fail - limit reached):\n");
    transaction_borrow(transactions, &transaction_count, &books[4], &users[2]);

    // Test 18: User 3 returns Book 1 (Success)
    printf("\n[TEST 18] User 3 returns Book 1 (Should succeed):\n");
    transaction_return(transactions, &transaction_count, &books[0], &users[2]);

    // Test 19: User 3 borrows Book 5 after return (Success)
    printf("\n[TEST 19] User 3 borrows Book 5 after return (Should succeed):\n");
    transaction_borrow(transactions, &transaction_count, &books[4], &users[2]);

    // Test 20: Auto-update status after return
    printf("\n[TEST 20] Auto-update status after return:\n");
    printf("Before return:\n");
    transaction_print_user_history(transactions, transaction_count, &users[2]);
    
    printf("Returning Book 5:\n");
    transaction_return(transactions, &transaction_count, &books[4], &users[2]);
    
    printf("After return:\n");
    transaction_print_user_history(transactions, transaction_count, &users[2]);

    // Final state
    printf("\n===== FINAL STATE =====\n");
    transaction_print_active(transactions, transaction_count);

    return 0;
}