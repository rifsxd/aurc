#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <search.h>
#include "aurc_aur.c"
#include "aurc_shell.c"
#include "aurc_version.c"
#include "aurc_help.c"
#include "aurc_pac.c"
#include "aurc_alpm.c"
#include "aurc_valid.c"
#include "aurc_sanitize.c"
#include "aurc_git.c"

int main(int argc, char *argv[]) {
    // Check for version flag
    if (argc == 2 && (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0)) {
        displayVersion();
        return 0;
    }

    // Check for help flag
    if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        displayHelp(argv[0]);
        return 0;
    }

    if (argc < 2) {
        printf("Usage: %s -h (for detailed help)\n", argv[0]);
        return 1;
    }

    char action[MAX_PACKAGE_NAME_LENGTH];
    sanitizeInput(argv[1], action, sizeof(action));

    // Check if the action is valid
    if (!isValidAction(action)) {
        printf(RED "Invalid action: %s\n" RESET, action);
        return 1;
    }

    if (strcmp(action, "install") == 0) {
        if (argc >= 3) {
            installPackages(argc, argv);
        } else {
            printf(RED "Usage: %s install <package_name1> <package_name2> ...\n" RESET, argv[0]);
            return 1;
        }
    } else if (strcmp(action, "install-local") == 0) {
        if (argc >= 3) {
            installLocalPackages(argc, argv);
        } else {
            printf(RED "Usage: %s install-local <local_package_path>\n" RESET, argv[0]);
            return 1;
        }
    } else if (strcmp(action, "install-aur") == 0) {
        if (argc >= 3) {
            int numPackages = argc - 2;
            char **packageNames = argv + 2;
            installAurPackages(packageNames, numPackages);
        } else {
            printf(RED "Usage: %s install-aur <package_name1> <package_name2> ... <package_nameN>\n" RESET, argv[0]);
            return 1;
        }
    } else if (strcmp(action, "github") == 0) {
        openGitHub();
    } else if (strcmp(action, "search-aur") == 0) {
        if (argc == 3) {
            searchAurPackage(argv[2]);
        } else {
            printf(RED "Usage: %s search-aur <package_name>\n" RESET, argv[0]);
            return 1;
        }
    } else if (strcmp(action, "install-force") == 0) {
        if (argc >= 3) {
            installPackagesForce(argc, argv);
        } else {
            printf(RED "Usage: %s install-force <package_name1> <package_name2> ...\n" RESET, argv[0]);
            return 1;
        }
    } else if (strcmp(action, "remove") == 0) {
        if (argc >= 3) {
            removePackages(argc, argv);
        } else {
            printf(RED "Usage: %s remove <package_name1> <package_name2> ...\n" RESET, argv[0]);
            return 1;
        }
    } else if (strcmp(action, "query") == 0) {
        if (argc == 3) {
            queryPackage(argv[2]);
        } else {
            printf(RED "Usage: %s query <package_name>\n" RESET, argv[0]);
            return 1;
        }
    } else if (strcmp(action, "search") == 0) {
        if (argc == 3) {
            searchPackage(argv[2]);
        } else {
            printf(RED "Usage: %s search <package_name>\n" RESET, argv[0]);
            return 1;
        }
    } else if (strcmp(action, "remove-dep") == 0) {
        if (argc >= 3) {
            removePackagesWithDependencies(argc, argv);
        } else {
            printf(RED "Usage: %s remove-dep <package_name1> <package_name2> ...\n" RESET, argv[0]);
            return 1;
        }
    } else if (strcmp(action, "remove-force") == 0) {
        if (argc >= 3) {
            removePackagesForce(argc, argv);
        } else {
            printf(RED "Usage: %s remove-force <package_name1> <package_name2> ...\n" RESET, argv[0]);
            return 1;
        }
    } else if (strcmp(action, "remove-force-dep") == 0) {
        if (argc >= 3) {
            removePackagesForceWithDependencies(argc, argv);
        } else {
            printf(RED "Usage: %s remove-force-dep <package_name1> <package_name2> ...\n" RESET, argv[0]);
            return 1;
        }
    } else if (strcmp(action, "remove-orp") == 0) {
        removeOrphanPackages();
        return 0;
    } else if (strcmp(action, "update") == 0) {
        updateSystem();
        return 0;
    } else if (strcmp(action, "refresh") == 0) {
        refreshRepo();
        return 0;
    } else if (strcmp(action, "modify-repo") == 0) {
        modifyRepo();
        return 0;
    } else if (strcmp(action, "clear-aur-cache") == 0) {
        clearAurBuildCache();
        return 0;
    } else if (strcmp(action, "list-outdated") == 0) {
    listOutdatedPackages();
    return 0;
    }
    return 0;
}

