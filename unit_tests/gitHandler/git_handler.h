/*******************************************************************************************
*
*   gitApi
*
*   MODULE USAGE:
*       #define GIT_hHANDLER_IMPLEMENTATION
*       #include "git_handler.h"
*
*
*
*        Git Handler - a simple C header-only library to interact with the GitHub API
*        Copyright (C) 2024-2031 - Guilherme Oliveira (0l1ve1r4)

        This program is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
**********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define NEW_POST_PATH "./posts/new"
#define CLONED_PROJECT_NAME "target"

typedef struct {
    char url[__UINT8_MAX__];        // URL of git repository
    char postsPath[__UINT8_MAX__];  // Path of the posts folder in the target repository
} GitRepository;

GitRepository newRepository(const char *url, const char *postsPath) {
    GitRepository repo;
    strncpy(repo.url, url, sizeof(repo.url));
    strncpy(repo.postsPath, postsPath, sizeof(repo.postsPath));
    return repo;
}

uint8_t cloneRepository(GitRepository *repo) { 
    char command[__UINT8_MAX__ + 50];  
    snprintf(command, sizeof(command), "git clone %s %s", repo->url, CLONED_PROJECT_NAME);
    
    #ifndef _DEBUG
        printf("Command: %s\n", command);
    #endif

    int result = system(command);

    #if defined(_DEBUG)
    if (result == 0) {
        printf("Cloning git repo success\n");
    } else {
        printf("Cloning git repo failed\n");
        return EXIT_FAILURE;
    }
    #endif

    return result == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

// Remove the cloned project to avoid conflicts
uint8_t cleanupAfterPull(void){
    char command[__UINT8_MAX__ + 50];
    snprintf(command, sizeof(command), "rm -rf %s", CLONED_PROJECT_NAME);
    if (system(command) != 0) {
        fprintf(stderr, "Error: Failed to remove cloned project\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

uint8_t pullToRepository(GitRepository *repo) {
    uint8_t result = cloneRepository(repo);
    if (result != EXIT_SUCCESS) {
        return result;
    }

    char command[__UINT8_MAX__ + 50];
    snprintf(command, sizeof(command), "cp %s %s", NEW_POST_PATH, repo->postsPath);
    if (system(command) != 0) {
        fprintf(stderr, "Error: Failed to move new post to repository\n");
        return EXIT_FAILURE;
    }

    system("cd target && git checkout -b StatiqPress");
    system("cd target && git add content/en/post");
    system("cd target && git commit -m 'StatiqPress Automatized Pull'");
    system("cd target && git push origin StatiqPress");

    cleanupAfterPull();

    return EXIT_SUCCESS;
}