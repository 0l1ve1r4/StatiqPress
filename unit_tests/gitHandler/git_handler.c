#include "git_handler.h"

// gcc git_handler.c -o test_pull
// ./test_pull

#define _DEBUG

int main() {
    GitRepository repo;
    repo = newRepository("https://github.com/0l1ve1r4/blog_.git", "target/content/en/post");


    uint8_t result = pullToRepository(&repo);

    if (result == EXIT_SUCCESS) {
        printf("Pull to repository succeeded.\n");
    } else {
        printf("Pull to repository failed.\n");
    }

    return 0;
}
