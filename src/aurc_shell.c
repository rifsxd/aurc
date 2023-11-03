#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

// Function to get the current user's shell
char* getCurrentUserShell() {
    struct passwd *pw = getpwuid(getuid());
    if (pw == NULL) {
        perror("getpwuid");
        return NULL;
    }

    char* shell = strdup(pw->pw_shell);
    if (shell == NULL) {
        perror("strdup");
        return NULL;
    }

    // Remove path and keep only the shell name
    char* lastSlash = strrchr(shell, '/');
    if (lastSlash != NULL) {
        shell = lastSlash + 1;
    }

    return shell;
}

// Function to execute a command with the user's shell
void executeCommandWithUserShell(char* command) {
    char* userShell = getCurrentUserShell();
    if (userShell != NULL) {
        // Allocate enough space for the arguments, including the terminating NULL
        char* argv[] = {userShell, "-c", command, NULL};

        printf("Executing command with user's shell: %s\n", userShell);
        execvp(userShell, argv);
        
        // If execvp fails, print an error message and exit
        perror("execvp");
        free(userShell);
        exit(EXIT_FAILURE);
    } else {
        // Fallback to system() if getting user's shell fails
        printf("Unable to detect user's shell. Fallback to system() for command execution.\n");

        // Validate and sanitize the command before using system()
        if (command != NULL && strlen(command) > 0) {
            // Sanitize the command to prevent command injection
            if (system(command) == -1) {
                perror("system");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("Invalid command. Exiting.\n");
            exit(EXIT_FAILURE);
        }
    }
    free(userShell);
}
