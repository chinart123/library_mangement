#include <stdio.h>
#include "book.h"
#include "user.h"
#include "transaction.h"

int main() {
    // Initialize books
    Book books[10];
    for (int i = 0; i < 10; i++) {
        char title[20];
        // sprintf(title, "Book %d", i+1); // lỗi tràn kí tự
        snprintf(title, sizeof(title), "Book %d", i+1); // Sử dụng snprintf an toàn
        book_init(&books[i], i+1, title, "Author");
    }

    // Initialize users
    User users[3];
    user_init(&users[0], 1, "User 1");  // Start with 0 books
    user_init(&users[1], 2, "User 2");  // Will borrow to limit
    user_init(&users[2], 3, "User 3");  // Will test edge cases

    // Initialize transaction system
    Transaction transactions[MAX_TRANSACTIONS];
    int transaction_count = 0;
    transaction_init(transactions, &transaction_count);

    printf("===== TESTING BORROWING LIMIT (MAX 3 BOOKS PER USER) =====\n");

    // Test 1: User borrows first book (success)
    printf("\n[TEST 1] User 1 borrows Book 1 (should succeed):\n");
    transaction_borrow(transactions, &transaction_count, &books[0], &users[0]);

    // Test 2: Same user borrows second book (success)
    printf("\n[TEST 2] User 1 borrows Book 2 (should succeed):\n");
    transaction_borrow(transactions, &transaction_count, &books[1], &users[0]);

    // Test 3: Same user borrows third book (success)
    printf("\n[TEST 3] User 1 borrows Book 3 (should succeed):\n");
    transaction_borrow(transactions, &transaction_count, &books[2], &users[0]);

    // Test 4: Try to borrow fourth book (should fail - limit reached)
    printf("\n[TEST 4] User 1 borrows Book 4 (should fail - limit reached):\n");
    transaction_borrow(transactions, &transaction_count, &books[3], &users[0]);

    // Test 5: Another user borrows same book (should fail - book already borrowed)
    printf("\n[TEST 5] User 2 borrows Book 1 (should fail - book taken):\n");
    transaction_borrow(transactions, &transaction_count, &books[0], &users[1]);

    printf("\n===== TESTING RETURN FUNCTIONALITY =====\n");

    // Test 6: Return one book (success)
    printf("\n[TEST 6] User 1 returns Book 1 (should succeed):\n");
    transaction_return(transactions, &transaction_count, &books[0], &users[0]);

    // Test 7: Try to borrow again after return (success - slot available)
    printf("\n[TEST 7] User 1 borrows Book 4 (should succeed - slot available):\n");
    transaction_borrow(transactions, &transaction_count, &books[3], &users[0]);

    // Test 8: Try to return same book twice (should fail)
    printf("\n[TEST 8] User 1 returns Book 1 again (should fail):\n");
    transaction_return(transactions, &transaction_count, &books[0], &users[0]);

    // Test 9: Return another book (success)
    printf("\n[TEST 9] User 1 returns Book 2 (should succeed):\n");
    transaction_return(transactions, &transaction_count, &books[1], &users[0]);

    printf("\n===== TESTING MULTIPLE USERS =====\n");

    // Test 10: Second user borrows to limit
    printf("\n[TEST 10] User 2 borrows 3 books:\n");
    transaction_borrow(transactions, &transaction_count, &books[4], &users[1]);
    transaction_borrow(transactions, &transaction_count, &books[5], &users[1]);
    transaction_borrow(transactions, &transaction_count, &books[6], &users[1]);

    // Test 11: Try to exceed limit (fail)
    printf("\n[TEST 11] User 2 borrows Book 8 (should fail - limit reached):\n");
    transaction_borrow(transactions, &transaction_count, &books[7], &users[1]);

    // Test 12: First user tries to borrow returned book (success)
    printf("\n[TEST 12] User 1 borrows Book 2 again (should succeed):\n");
    transaction_borrow(transactions, &transaction_count, &books[1], &users[0]);

    printf("\n===== TESTING EDGE CASES =====\n");

    // Test 13: Borrow with NULL book (fail)
    printf("\n[TEST 13] User 3 borrows NULL book (should fail):\n");
    transaction_borrow(transactions, &transaction_count, NULL, &users[2]);

    // Test 14: Borrow with NULL user (fail)
    printf("\n[TEST 14] NULL user borrows Book 8 (should fail):\n");
    transaction_borrow(transactions, &transaction_count, &books[7], NULL);

    // Test 15: Return with NULL book (fail)
    printf("\n[TEST 15] User 1 returns NULL book (should fail):\n");
    transaction_return(transactions, &transaction_count, NULL, &users[0]);

    // Test 16: Return with NULL user (fail)
    printf("\n[TEST 16] NULL user returns Book 1 (should fail):\n");
    transaction_return(transactions, &transaction_count, &books[0], NULL);

    // Test 17: Return book not borrowed by user (fail)
    printf("\n[TEST 17] User 3 returns Book 4 (should fail - not borrowed):\n");
    transaction_return(transactions, &transaction_count, &books[3], &users[2]);

    printf("\n===== TESTING TRANSACTION HISTORY =====\n");
    transaction_print_user_history(transactions, transaction_count, &users[0]);
    transaction_print_user_history(transactions, transaction_count, &users[1]);
    transaction_print_user_history(transactions, transaction_count, &users[2]);

    printf("\n===== FINAL ACTIVE TRANSACTIONS =====\n");
    transaction_print_active(transactions, transaction_count);

    // Test 18: Try to return all books
    printf("\n[TEST 18] Returning all borrowed books:\n");
    for (int i = 0; i < transaction_count; i++) {
        if (transactions[i].is_active) {
            transaction_return(transactions, &transaction_count, transactions[i].book, transactions[i].user);
        }
    }

    printf("\n===== STATE AFTER RETURNING ALL =====\n");
    transaction_print_active(transactions, transaction_count);

    return 0;
}