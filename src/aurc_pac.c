#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bsd/string.h>
#include "aurc_colors.c"

#define MAX_COMMAND_LENGTH 500

// Function to execute a pacman command
void executePacmanCommand(int argc, char *argv[], const char *commandPrefix, const char *usageMessage) {
    // Check if enough arguments are provided
    if (argc >= 3) {
        size_t commandLength = strlen(commandPrefix);

        // Calculate the total length of the command
        for (int i = 2; i < argc; ++i) {
            commandLength += strlen(argv[i]) + 1;
        }

        // Check if the command length exceeds the maximum limit
        if (commandLength > MAX_COMMAND_LENGTH) {
            fprintf(stderr, "Command too long\n");
            return;
        }

        // Build the command string
        char command[MAX_COMMAND_LENGTH];
        strncpy(command, commandPrefix, sizeof(command));

        // Concatenate arguments to form the complete command
        for (int i = 2; i < argc; ++i) {
            size_t remainingSpace = MAX_COMMAND_LENGTH - strlen(command);
            snprintf(command + strlen(command), remainingSpace, " %s", argv[i]);
        }

        // Execute the command
        executeCommandWithUserShell(command);
    } else {
        // Print usage message in case of insufficient arguments
        fprintf(stderr, RED "Usage: %s %s\n" RESET, argv[0], usageMessage);
    }
}

// Function to install packages forcefully
void installPackagesForce(int argc, char *argv[]) {
    // Delegate to the generic pacman command execution with force flag
    executePacmanCommand(argc, argv, "sudo pacman -Sdd ", "remove <package_name1> <package_name2> ...");
}

// Function to install local packages
void installLocalPackages(int argc, char *argv[]) {
    // Check if enough arguments are provided
    if (argc >= 3) {
        char command[MAX_COMMAND_LENGTH];

        // Build the command string for installing local packages
        snprintf(command, sizeof(command), "sudo pacman -U %s", argv[2]);

        // Concatenate additional package paths to the command
        for (int i = 3; i < argc; ++i) {
            strncat(command, " ", MAX_COMMAND_LENGTH - strlen(command));
            strncat(command, argv[i], MAX_COMMAND_LENGTH - strlen(command));
        }

        // Execute the command
        executeCommandWithUserShell(command);
    } else {
        // Print usage message in case of insufficient arguments
        fprintf(stderr, RED "Usage: %s install-local <package_path1> <package_path2> ...\n" RESET, argv[0]);
    }
}

// Function to remove packages forcefully
void removePackagesForce(int argc, char *argv[]) {
    // Delegate to the generic pacman command execution with force flag
    executePacmanCommand(argc, argv, "sudo pacman -Rdd ", "remove <package_name1> <package_name2> ...");
}

// Function to remove packages forcefully with dependencies
void removePackagesForceWithDependencies(int argc, char *argv[]) {
    // Delegate to the generic pacman command execution with force and dependency flags
    executePacmanCommand(argc, argv, "sudo pacman -Rdds ", "remove <package_name1> <package_name2> ...");
}

// Function to install packages
void installPackages(int argc, char *argv[]) {
    // Check if enough arguments are provided
    if (argc >= 3) {
        char command[MAX_COMMAND_LENGTH];

        // Build the command string for installing packages
        snprintf(command, sizeof(command), "sudo pacman -S %s", argv[2]);

        // Concatenate additional package names to the command
        for (int i = 3; i < argc; ++i) {
            strlcat(command, " ", sizeof(command));
            strlcat(command, argv[i], sizeof(command));
        }

        // Execute the command
        executeCommandWithUserShell(command);
    } else {
        // Print usage message in case of insufficient arguments
        printf(RED "Usage: %s install <package_name1> <package_name2> ...\n" RESET, argv[0]);
    }
}

// Function to remove packages
void removePackages(int argc, char *argv[]) {
    // Check if enough arguments are provided
    if (argc >= 3) {
        char command[MAX_COMMAND_LENGTH];

        // Build the command string for removing packages
        snprintf(command, sizeof(command), "sudo pacman -R %s", argv[2]);

        // Concatenate additional package names to the command
        for (int i = 3; i < argc; ++i) {
            strlcat(command, " ", sizeof(command));
            strlcat(command, argv[i], sizeof(command));
        }

        // Execute the command
        executeCommandWithUserShell(command);
    } else {
        // Print usage message in case of insufficient arguments
        printf(RED "Usage: %s remove <package_name1> <package_name2> ...\n" RESET, argv[0]);
    }
}

// Function to query if a package is installed
void queryPackage(char *packageName) {
    char command[MAX_COMMAND_LENGTH];

    // Build the command string for querying the package
    snprintf(command, sizeof(command), "pacman -Q %s", packageName);

    // Execute the command and check the result
    int result = system(command);
    if (result != 0) {
        printf(RED "%s is not installed.\n" RESET, packageName);
    } else {
        printf(GREEN "%s is installed.\n" RESET, packageName);
    }
}

// Function to search for a package in the repository
void searchPackage(char *packageName) {
    char command[MAX_COMMAND_LENGTH];

    // Build the command string for searching the package
    snprintf(command, sizeof(command), "pacman -Ss %s", packageName);

    // Execute the command to search for the package
    executeCommandWithUserShell(command);
}

// Function to remove packages with dependencies
void removePackagesWithDependencies(int argc, char *argv[]) {
    // Check if enough arguments are provided
    if (argc >= 3) {
        char command[MAX_COMMAND_LENGTH];

        // Build the command string for removing packages with dependencies
        snprintf(command, sizeof(command), "sudo pacman -Rs %s", argv[2]);

        // Concatenate additional package names to the command
        for (int i = 3; i < argc; ++i) {
            strlcat(command, " ", sizeof(command));
            strlcat(command, argv[i], sizeof(command));
        }

        // Execute the command
        executeCommandWithUserShell(command);
    } else {
        // Print usage message in case of insufficient arguments
        printf(RED "Usage: %s remove-dep <package_name1> <package_name2> ...\n" RESET, argv[0]);
    }
}

// Function to remove orphan packages
void removeOrphanPackages() {
    // Execute the command to remove orphan packages
    executeCommandWithUserShell("sudo pacman -Rns $(pacman -Qdtq)");
}

// Function to update the system
void updateSystem() {
    // Execute the command to update the system
    executeCommandWithUserShell("sudo pacman -Syyu");
}

// Function to refresh the repository
void refreshRepo() {
    // Execute the command to refresh the repository
    executeCommandWithUserShell("sudo pacman -Syy");
}

// Function to modify the repository configuration
void modifyRepo() {
    // Execute the command to open the repository configuration file in Vim
    executeCommandWithUserShell("sudo vim /etc/pacman.d/mirrorlist");
}

// Function to list outdated packages
void listOutdatedPackages() {
    // Execute the command to list outdated packages
    char command[] = "pacman -Qu";
    FILE *fp = popen(command, "r");

    if (fp == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    char buffer[128];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    if (bytesRead == 0) {
        printf("\n");
        printf("No packages are outdated. Run 'aurc refresh' to update the database.\n");
        printf("\n");
    } else {
        // Execute the command with the user's shell
        executeCommandWithUserShell("pacman -Qu");
    }
}
