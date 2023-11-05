#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <search.h>

#define MAX_PACKAGE_NAME_LENGTH 256
#define MAX_INPUT_SIZE 512

void sanitizeInput(const char *input, char *output, size_t size) {
    // Ensure input is not too long to prevent buffer overflow
    if (strlen(input) >= size) {
        fprintf(stderr, "Too many args.\n");
        exit(1);
    }
    strncpy(output, input, size - 1);
    output[size - 1] = '\0'; // Null-terminate the string
}