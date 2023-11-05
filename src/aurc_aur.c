#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

// Function to query the AUR repository for package information
void queryAurRepo(char *packageName, char *message) {
    size_t packageNameLen = strlen(packageName);

    // Check if the package name is too long
    if (packageNameLen > 400) {
        printf("Package name is too long.\n");
        return;
    }

    // Sanitize package name by replacing non-alphanumeric characters with '_'
    for (char *p = packageName; *p != '\0'; p++) {
        if (!isalnum((unsigned char)*p)) {
            *p = '_';
        }
    }

    char command[500];
    // Use snprintf to avoid buffer overflow
    int ret = snprintf(command, sizeof(command), "curl -s 'https://aur.archlinux.org/rpc/?v=5&type=search&arg=%s' | jq -r '.results[] | .Name'", packageName);
    if (ret < 0 || ret >= sizeof(command)) {
        printf("Command is too long or snprintf failed.\n");
        return;
    }

    // Open a pipe to execute the command and read the output
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error opening pipe");
        return;
    }

    char buffer[512];
    // Read the output into a buffer
    size_t bytesRead = fread(buffer, 1, sizeof(buffer) - 1, fp);
    if (bytesRead == 0 && ferror(fp)) {
        perror("fread");
        pclose(fp);
        return;
    }

    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        // Print the results
        printf("%s '%s':\n%s\n", message, packageName, buffer);
    } else {
        printf("No matches found for '%s'.\n", packageName);
    }

    // Close the pipe
    pclose(fp);
}

// Function to search for AUR packages
void searchAurPackage(char *packageName) {
    queryAurRepo(packageName, "Search results for");
}

// Function to display available and similar AUR packages
void existingAurPackage(char *packageName) {
    queryAurRepo(packageName, "Available & similar packages for");
}

// Function to display the contents of the PKGBUILD file
void displayPKGBUILD(char *packageName, char *downloadDir) {
    char displayCommand[300];
    // Use snprintf to avoid buffer overflow
    snprintf(displayCommand, sizeof(displayCommand), "less %s/%s/PKGBUILD", downloadDir, packageName);
    // Execute the command
    system(displayCommand);
}

// Function to install AUR packages
void installAurPackages(char **packageNames, int numPackages) {
    char downloadDir[256];
    // Use snprintf to avoid buffer overflow
    snprintf(downloadDir, sizeof(downloadDir), "%s", "~/.cache/aurc/");

    char mkdirCommand[200];
    // Use snprintf to avoid buffer overflow
    snprintf(mkdirCommand, sizeof(mkdirCommand), "mkdir -p %s", downloadDir);
    // Create the download directory
    system(mkdirCommand);

    for (int i = 0; i < numPackages; ++i) {
        char *packageName = packageNames[i];

        char url[256];
        // Use snprintf to avoid buffer overflow
        snprintf(url, sizeof(url), "https://aur.archlinux.org/cgit/aur.git/snapshot/%s.tar.gz", packageName);

        char checkUrlCommand[300];
        // Use snprintf to avoid buffer overflow
        snprintf(checkUrlCommand, sizeof(checkUrlCommand), "curl -o /dev/null --silent --head --fail %s", url);
        // Check if the package URL exists
        int urlStatus = system(checkUrlCommand);

        if (urlStatus != 0) {
            // Print an error message and display existing packages
            printf("Error: Package '%s' not found.\n", packageName);
            existingAurPackage(packageName);
            continue;
        }

        char downloadCommand[300];
        // Use snprintf to avoid buffer overflow
        snprintf(downloadCommand, sizeof(downloadCommand), "curl -L -o %s/%s.tar.gz %s", downloadDir, packageName, url);
        // Download the package
        system(downloadCommand);

        char extractCommand[300];
        // Use snprintf to avoid buffer overflow
        snprintf(extractCommand, sizeof(extractCommand), "tar -xzf %s/%s.tar.gz -C %s", downloadDir, packageName, downloadDir);
        // Extract the package
        system(extractCommand);

        char userInput[10];
        // Ask the user if they want to view the PKGBUILD before installation
        printf("Do you want to view the PKGBUILD for '%s' before installation? (Recommended) (y/n): ", packageName);
        fgets(userInput, sizeof(userInput), stdin);

        // Check if the user pressed Enter without typing anything
        if (userInput[0] == '\n') {
            userInput[0] = 'y';  // Set the default answer to "yes"
        } else {
            // Handle other cases if needed
        }

        if (tolower(userInput[0]) == 'y') {
            // Display the PKGBUILD
            displayPKGBUILD(packageName, downloadDir);

            // Ask the user if they want to continue with the installation
            printf("Do you want to continue with the installation of '%s'? (y/n): ", packageName);
            fgets(userInput, sizeof(userInput), stdin);

            // Check if the user pressed Enter without typing anything
            if (userInput[0] == '\n') {
                userInput[0] = 'y';  // Set the default answer to "yes"
            } else {
                // Handle other cases if needed
            }

            if (tolower(userInput[0]) != 'y') {
                // Print a message and clean up in case of abort
                printf("Installation of '%s' aborted.\n", packageName);
                char cleanupCommand[300];
                // Use snprintf to avoid buffer overflow
                snprintf(cleanupCommand, sizeof(cleanupCommand), "rm -rf %s/%s %s/%s.tar.gz", downloadDir, packageName, downloadDir, packageName);
                system(cleanupCommand);
                continue;
            }
        }

        char buildCommand[300];
        // Use snprintf to avoid buffer overflow
        snprintf(buildCommand, sizeof(buildCommand), "cd %s/%s && makepkg -si", downloadDir, packageName);

        // Fork a child process to execute the build command
        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            execlp("sh", "sh", "-c", buildCommand, (char *)NULL);
            _exit(EXIT_FAILURE);
        } else {
            // Parent process
            int status;
            // Wait for the child process to finish
            waitpid(pid, &status, 0);
            // Check if the child process exited successfully
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                // Print an error message and clean up in case of failure
                printf("Installation of '%s' failed.\n", packageName);
                char cleanupCommand[300];
                // Use snprintf to avoid buffer overflow
                snprintf(cleanupCommand, sizeof(cleanupCommand), "rm -rf %s/%s %s/%s.tar.gz", downloadDir, packageName, downloadDir, packageName);
                system(cleanupCommand);
                continue;
            }
        }

        // Ask the user if they want to clean up the build cache
        printf("Do you want to clean up the build cache for '%s'? (Recommended) (y/n): ", packageName);
        fgets(userInput, sizeof(userInput), stdin);

        // Check if the user pressed Enter without typing anything
        if (userInput[0] == '\n') {
            userInput[0] = 'y';  // Set the default answer to "yes"
        } else {
            // Handle other cases if needed
        }

        if (tolower(userInput[0]) == 'y') {
            // Clean up the build cache
            char cleanupCommand[300];
            // Use snprintf to avoid buffer overflow
            snprintf(cleanupCommand, sizeof(cleanupCommand), "rm -rf %s/%s %s/%s.tar.gz", downloadDir, packageName, downloadDir, packageName);
            system(cleanupCommand);
        }
    }
}

// Function to clear the AUR build cache
void clearAurBuildCache() {
    char downloadDir[256];
    // Use snprintf to avoid buffer overflow
    snprintf(downloadDir, sizeof(downloadDir), "~/.cache/aurc/");

    char cleanupCommand[300];
    // Use snprintf to avoid buffer overflow
    snprintf(cleanupCommand, sizeof(cleanupCommand), "rm -rf %s/*", downloadDir);
    // Clean up the build cache
    system(cleanupCommand);
}
