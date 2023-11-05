#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "aurc_colors.c"

void openGitHub() {
    const char *url = "https://github.com/rifsxd/aurc";
    printf("Opening GitHub...\n");
    char sanitizedCommand[MAX_INPUT_SIZE];
    snprintf(sanitizedCommand, sizeof(sanitizedCommand), "xdg-open %s >/dev/null 2>&1", url);
    int result = system(sanitizedCommand);
    if (result == 0) {
        printf(GREEN "GitHub opened successfully!\n" RESET);
    } else {
        printf(RED "Error: Failed to open GitHub!\n" RESET);
        exit(1);
    }
}