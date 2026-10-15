
#include <stdio.h>

int main(int argc, char *argv[]) {
    const char *filename = (argc > 1) ? argv[1] : "data.txt";
    char line[100];
    int lines_read = 0;

    FILE *fp;

    printf("Writing lines to %s...\n", filename);

    fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error opening file for writing");
        return 1;
    }

    fprintf(fp, "Hello, file I/O in C!\n");
    fprintf(fp, "This is another line.\n");

    if (fclose(fp) != 0) {
        perror("Error closing file");
        return 1;
    }

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file for reading");
        return 1;
    }

    printf("Reading contents:\n");

    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
        lines_read++;
    }

    if (ferror(fp)) {
        perror("Error reading file");
        fclose(fp);
        return 1;
    }

    if (fclose(fp) != 0) {
        perror("Error closing file");
        return 1;
    }

    printf("Lines read: %d\n", lines_read);

    return 0;
}
