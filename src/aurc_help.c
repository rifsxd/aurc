#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "aurc_colors.c"

void displayHelp(const char *programName) {
    printf("\n");
    printf("Aurc Package Manager - Help\n\n");
    printf("Usage: %s <action> [package_name]\n", programName);
    printf("\nActions:\n");
    printf("  " GREEN "update" GREEN RESET "           Update outdated system/user packages\n");
    printf("  " GREEN "refresh" GREEN RESET "          Refresh repository database\n");
    printf("  " GREEN "install" RESET "          Install packages\n");
    printf("  " GREEN "install-local" RESET "    Install a local package\n");
    printf("  " GREEN "install-aur" RESET "      Install aur package\n");
    printf("  " GREEN "github" RESET "           Visit the gitHub repo!\n");
    printf("  " YELLOW "install-force" RESET "    Forcefully install packages\n");
    printf("  " YELLOW "modify-repo" RESET "      Modify arch repositories\n");
    printf("  " YELLOW "query" RESET "            Query if a package is installed\n");
    printf("  " YELLOW "list-outdated" RESET "    List outdated base repo packages\n");
    printf("  " YELLOW "search" RESET "           Search for a package in the base repository\n");
    printf("  " YELLOW "search-aur" RESET "       Search for a package in the aur repository\n");
    printf("  " RED "remove" RESET "           Remove packages\n");
    printf("  " RED "remove-dep" RESET "       Remove packages along with its dependencies\n");
    printf("  " RED "remove-force" RESET "     Forcefully remove packages even if other packages depend on it\n");
    printf("  " RED "remove-force-dep" RESET " Forcefully remove packages even if other packages depend on it along with its dependencies\n");
    printf("  " RED "remove-orp" RESET "       Remove orphan packages\n");
    printf("\nOptions:\n");
    printf("  --version, -v    Display the version of the package manager\n");
    printf("  --help, -h       Display this help guide\n");
    printf("\n");
}