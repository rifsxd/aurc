#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

// Cleanup function to free allocated resources
void cleanup(char* userShell) {
    free(userShell);
    // Add any other cleanup steps if needed
}

// Get the current user's shell
char* getCurrentUserShell() {
    struct passwd *pw = getpwuid(getuid());
    if (pw == NULL) {
        perror("getpwuid");
        return NULL;
    }

    char* shellName = strrchr(pw->pw_shell, '/');
    if (shellName != NULL) {
        shellName++;
    } else {
        shellName = pw->pw_shell;
    }

    char* shell = strdup(shellName);
    if (shell == NULL) {
        perror("strdup");
        return NULL;
    }

    return shell;
}

// Execute a command with the user's shell
void executeCommandWithUserShell(char* command) {
    // Get the user's shell
    char* userShell = getCurrentUserShell();
    if (userShell != NULL) {
        // Calculate the required size for argv
        size_t argvSize = strlen(userShell) + strlen("-c") + strlen(command) + 3;
        char** argv = (char**)malloc(argvSize * sizeof(char*));

        if (argv != NULL) {
            // Populate argv
            argv[0] = userShell;
            argv[1] = "-c";
            argv[2] = command;
            argv[3] = NULL;

            // Execute the command with the user's shell
            printf("Executing command with user's shell: %s\n", userShell);
            execvp(userShell, argv);

            // If execvp fails, print an error message
            perror("execvp");
            cleanup(userShell);
            free(argv);
            exit(EXIT_FAILURE);
        } else {
            perror("malloc");
            cleanup(userShell);
            exit(EXIT_FAILURE);
        }
    } else {
        // Fallback to execvp() if unable to detect user's shell
        printf("Unable to detect user's shell. Fallback to execvp() for command execution.\n");

        // Validate and sanitize the command before using execvp
        if (command != NULL && strlen(command) > 0) {
            // Execute the command with the default shell
            char* argv[] = {"sh", "-c", command, NULL};
            if (execvp("sh", argv) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("Invalid command. Exiting.\n");
            exit(EXIT_FAILURE);
        }
    }
}
