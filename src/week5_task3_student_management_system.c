
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_STUDENTS 100
#define LINE_SIZE 256

typedef struct Student {
    char name[50];
    int id;
    float gpa;
} Student;

void read_line(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
}

int load_students(Student arr[], int max) {
    FILE *fp = fopen("students.txt", "r");
    char line[LINE_SIZE];
    int count = 0;

    if (fp == NULL) {
        return 0;
    }

    while (count < max && fgets(line, sizeof(line), fp) != NULL) {
        Student student;

        if (sscanf(line, "%49[^,],%d,%f",
                   student.name, &student.id, &student.gpa) == 3) {
            if (student.name[0] != '\0' &&
                student.gpa >= 0.0f && student.gpa <= 4.0f) {
                arr[count++] = student;
            }
        }
    }

    fclose(fp);
    return count;
}

int save_students(const Student arr[], int count) {
    FILE *fp = fopen("students.txt", "w");

    if (fp == NULL) {
        return -1;
    }

    for (int i = 0; i < count; i++) {
        if (fprintf(fp, "%s,%d,%.2f\n",
                    arr[i].name, arr[i].id, arr[i].gpa) < 0) {
            fclose(fp);
            return -1;
        }
    }

    if (fclose(fp) != 0) {
        return -1;
    }

    return 0;
}

void add_student(Student arr[], int *count) {
    char name[LINE_SIZE];
    char input[LINE_SIZE];
    char *end;
    long id;
    float gpa;

    if (*count >= MAX_STUDENTS) {
        printf("Cannot add more students (limit reached).\n");
        return;
    }

    printf("Enter name: ");
    read_line(name, sizeof(name));

    if (name[0] == '\0' || strchr(name, ',') != NULL ||
        strlen(name) >= sizeof(arr[*count].name)) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    printf("Enter ID: ");
    read_line(input, sizeof(input));

    errno = 0;
    id = strtol(input, &end, 10);

    if (input[0] == '\0' || *end != '\0' ||
        errno == ERANGE || id < 0 || id > 2147483647L) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    printf("Enter GPA: ");
    read_line(input, sizeof(input));

    errno = 0;
    gpa = strtof(input, &end);

    if (input[0] == '\0' || *end != '\0' ||
        errno == ERANGE || gpa < 0.0f || gpa > 4.0f) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    strcpy(arr[*count].name, name);
    arr[*count].id = (int)id;
    arr[*count].gpa = gpa;
    (*count)++;

    printf("Student added successfully!\n");
}

void list_students(const Student arr[], int count) {
    if (count == 0) {
        printf("No students found.\n");
        return;
    }

    printf("%-5s %-20s %s\n", "ID", "Name", "GPA");

    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %.2f\n",
               arr[i].id, arr[i].name, arr[i].gpa);
    }
}

int main(void) {
    Student students[MAX_STUDENTS];
    int count = load_students(students, MAX_STUDENTS);
    char input[LINE_SIZE];

    printf("Loaded %d student(s) from students.txt\n", count);

    for (;;) {
        printf("\n=== Student Management System ===\n");
        printf("1. List students\n");
        printf("2. Add student\n");
        printf("3. Save and Exit\n");
        printf("Select an option: ");

        read_line(input, sizeof(input));

        if (strcmp(input, "1") == 0) {
            list_students(students, count);
        } else if (strcmp(input, "2") == 0) {
            add_student(students, &count);
        } else if (strcmp(input, "3") == 0 || input[0] == '\0') {
            if (save_students(students, count) != 0) {
                printf("Error saving data.\n");
                return 1;
            }

            printf("Data saved. Goodbye!\n");
            return 0;
        } else {
            printf("Invalid option. Please select 1, 2, or 3.\n");
        }
    }
}
