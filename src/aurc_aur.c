#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

void queryAurRepo(char *packageName, char *message) {
    size_t packageNameLen = strlen(packageName);

    // Check the length of packageName and handle it if it's too long
    if (packageNameLen > 400) {
        printf("Package name is too long.\n");
        return;
    }

    // Sanitize packageName by replacing non-alphanumeric characters with '_'
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

    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error opening pipe");
        return;
    }

    char buffer[512];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer) - 1, fp);
    if (bytesRead == 0 && ferror(fp)) {
        perror("fread");
        pclose(fp);
        return;
    }

    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        printf("%s '%s':\n%s\n", message, packageName, buffer);
    } else {
        printf("No matches found for '%s'.\n", packageName);
    }

    pclose(fp);
}

void searchAurPackage(char *packageName) {
    queryAurRepo(packageName, "Search results for");
}

void existingAurPackage(char *packageName) {
    queryAurRepo(packageName, "Available & similar packages for");
}

void displayPKGBUILD(char *packageName, char *downloadDir) {
    char displayCommand[300];
    snprintf(displayCommand, sizeof(displayCommand), "less %s/%s/PKGBUILD", downloadDir, packageName);
    system(displayCommand);
}

void installAurPackages(char **packageNames, int numPackages) {
    char downloadDir[256];
    // Use snprintf to avoid buffer overflow
    snprintf(downloadDir, sizeof(downloadDir), "%s", "~/.cache/aurc/");

    char mkdirCommand[200];
    // Use snprintf to avoid buffer overflow
    snprintf(mkdirCommand, sizeof(mkdirCommand), "mkdir -p %s", downloadDir);
    system(mkdirCommand);

    for (int i = 0; i < numPackages; ++i) {
        char *packageName = packageNames[i];

        char url[256];
        // Use snprintf to avoid buffer overflow
        snprintf(url, sizeof(url), "https://aur.archlinux.org/cgit/aur.git/snapshot/%s.tar.gz", packageName);

        char checkUrlCommand[300];
        // Use snprintf to avoid buffer overflow
        snprintf(checkUrlCommand, sizeof(checkUrlCommand), "curl -o /dev/null --silent --head --fail %s", url);
        int urlStatus = system(checkUrlCommand);

        if (urlStatus != 0) {
            printf("Error: Package '%s' not found.\n", packageName);
            existingAurPackage(packageName);
            continue;
        }

        char downloadCommand[300];
        // Use snprintf to avoid buffer overflow
        snprintf(downloadCommand, sizeof(downloadCommand), "curl -L -o %s/%s.tar.gz %s", downloadDir, packageName, url);
        system(downloadCommand);

        char extractCommand[300];
        // Use snprintf to avoid buffer overflow
        snprintf(extractCommand, sizeof(extractCommand), "tar -xzf %s/%s.tar.gz -C %s", downloadDir, packageName, downloadDir);
        system(extractCommand);

        char userInput[10];
        printf("Do you want to view the PKGBUILD for '%s' before installation? (Recommended) (y/n): ", packageName);
        fgets(userInput, sizeof(userInput), stdin);

        if (tolower(userInput[0]) == 'y') {
            displayPKGBUILD(packageName, downloadDir);

            printf("Do you want to continue with the installation of '%s'? (y/n): ", packageName);
            fgets(userInput, sizeof(userInput), stdin);

            if (tolower(userInput[0]) != 'y') {
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
        system(buildCommand);

        printf("Do you want to clean up the build cache for '%s'? (Recommended) (y/n): ", packageName);
        fgets(userInput, sizeof(userInput), stdin);

        if (tolower(userInput[0]) == 'y') {
            char cleanupCommand[300];
            // Use snprintf to avoid buffer overflow
            snprintf(cleanupCommand, sizeof(cleanupCommand), "rm -rf %s/%s %s/%s.tar.gz", downloadDir, packageName, downloadDir, packageName);
            system(cleanupCommand);
        }
    }
}

void clearAurBuildCache() {
    char downloadDir[256];
    // Use snprintf to avoid buffer overflow
    snprintf(downloadDir, sizeof(downloadDir), "~/.cache/aurc/");

    char cleanupCommand[300];
    // Use snprintf to avoid buffer overflow
    snprintf(cleanupCommand, sizeof(cleanupCommand), "rm -rf %s/*", downloadDir);
    system(cleanupCommand);
}