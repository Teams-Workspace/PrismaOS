#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/*
 * This program reads the current version from 'current_version.txt',
 * increments the patch version (V3), updates the text file, and
 * regenerates the 'include/version.h' header file with macros V1, V2, V3.
 */

int main(int argc, char *argv[])
{
    FILE *fp;               // File pointer for reading/writing version files
    char *line = NULL;      // Buffer to store a line from the version file
    size_t len = 0;         // Length of the buffer
    ssize_t read;           // Number of characters read by getline
    const char s[2] = "."; // Delimiter for splitting version (major.minor.patch)
    char *token;            // Temporary token for parsing
    unsigned int version[3];// Array to store version numbers [V1,V2,V3]
    unsigned short int i = 0;  // Index for array
    int count = 0;          // Unused (can be removed)

    // Open version file for reading
    fp = fopen("current_version.txt", "r");
    if (fp == NULL)         // Exit if file cannot be opened
        exit(EXIT_FAILURE);

    // Read each line from the file (should be just one line)
    while ((read = getline(&line, &len, fp)) != -1)
    {
        // Split the line using '.' as delimiter
        token = strtok(line, s);

        // Walk through each token and convert to integer
        while (token != NULL)
        {
            version[i] = atoi(token); // Convert string to int
            i++;
            token = strtok(NULL, s);  // Move to next token
        }
    }
    fclose(fp);               // Close file
    if (line)
        free(line);           // Free dynamically allocated buffer
    line = NULL;
    len = 0;

    // Open version file for writing (overwrite)
    fp = fopen("current_version.txt", "w");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    printf("%d\n", version[2]);  // Print old patch version
    version[2] = version[2] + 1; // Increment patch version
    printf("%d\n", version[2]);  // Print new patch version

    // Write updated version back to file
    fprintf(fp, "%u.%u.%u", version[0], version[1], version[2]);
    fclose(fp);

    i = 0;
    // Open header file for writing version macros
    fp = fopen("include/version.h", "w");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    // Write header guard
    fprintf(fp, "#ifndef _VERSION_H\n");
    fprintf(fp, "#define _VERSION_H 1\n\n");

    // Write version macros V1, V2, V3
    for (i = 0; i < 3; i++)
    {
        fprintf(fp, "#define V%d %u\n", i + 1, version[i]);
    }

    fprintf(fp, "\n#endif"); // Close header guard

    return 0; // Exit program successfully
}
