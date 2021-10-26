// Copyright (c) 2021 David Maltby
// David Maltby
// 10/28/21
// CSE 374 - HW3
//
// Imitates the grep tool:
//  User passes in a string pattern and a list of filenames as arguments.
//  Parses through each file to search for an occurence of that pattern.
//  If the pattern is found, the entire line it was found in is
//  printed to stdout.
// Optional arguments:
//      -i : Ignores case when searching for the pattern
//      -n : Prints the line number along with the line
//      (both optional arguments can be used in the same call)
//
// Valid argument format:
//      ./myfgrep [options] <pattern> <file1> <file2>...

// Imports
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Constants
#define SIZE 502
#define SIZE_CMD 101

// Copies a string (up to max length)
// Optionally converts the copied string to lowercase
// Params:
//  - copy_to:    The string/buffer to copy to
//  - copy_from:  The null-terminated string to copy from
//  - max_length: The max characters to copy over (including null terminator)
//  - lowercase:  Whether to convert to lowercase after copying
void copy_string(char* copy_to, char* copy_from, int length,
                 bool lowercase) {
    if (lowercase) {
        int i;
        for (i = 0; i < length; i++) {
            copy_to[i] = tolower(copy_from[i]);
        }
    } else {
        strncpy(copy_to, copy_from, length);
    }
}

// Prints all lines in a file that match the pattern
// Params:
//  - file:              the file to check for results
//  - pattern:           the pattern to look for in each line
//  - ignore_case:       whether to ignore case when using the pattern
//  - print_line_number: whether to print the line number for each matching line
void print_matches_in_file(FILE* file, char* pattern,
                           bool ignore_case, bool print_line_number) {
    char copy1[SIZE];
    char copy2[SIZE];
    int lineNum = 0;

    // Copies the pattern, converting to lowercase if necessary
    char p[SIZE_CMD];
    copy_string(p, pattern, SIZE_CMD, ignore_case);

    // Searches each line in the file for the pattern
    while (fgets(copy1, sizeof(copy1), file) != NULL) {
        lineNum++;
        copy_string(copy2, copy1, sizeof(copy1), ignore_case);
        if (strstr(copy2, p) != NULL) {
            if (print_line_number) {
                printf("(%d) ", lineNum);
            }
            printf("%s", copy1);
        }
    }
}

// Traverses through a list of files and prints matches to the pattern for each
// Params:
//  - num_files:         number of files in the list
//  - filename_list:     list of filenames to check
//  - pattern:           the pattern to look for in each line
//  - ignore_case:       whether to ignore case when using the pattern
//  - print_line_number: whether to print the line number for each matching line
void print_all_matches(int num_files, char** filename_list, char* pattern,
                       bool ignore_case, bool print_line_number) {
    int i;
    FILE* file;
    char* filename;

    // Opens each file to search for the pattern
    // Prints error message to stdderr if the file cannot be opened
    for (i = 0; i < num_files; i++) {
        filename = filename_list[i];
        file = fopen(filename, "r");
        if (file == NULL) {
            fprintf(stderr, "Could not open file: %s\n", filename);
        } else {
            printf("%s:\n", filename);
            print_matches_in_file(file, pattern, ignore_case,
            print_line_number);
        }
    }
}

// Print the usage of the program
// Params:
//  - program_name: the name of the program being run (argv[0])
void usage(char* program_name) {
    printf("Usage: %s [OPTIONS] pattern file...\n", program_name);
}

// Main entry point of the function
// Parses the input into flags, pattern, and files
// Params:
//  - argc: The number of arguments provided
//  - argv: The list of arguments provided
// Returns the exit status
int main(int argc, char** argv) {
    bool ignore_case;
    bool print_line_number;
    char pattern[SIZE_CMD];

    // Checks for optional arguments
    int i;
    int count = 1;
    for (i = 1; i <= 2; i++) {
        if (argv[i] == NULL) {
            usage("myfgrep.c");
        return(EXIT_FAILURE);
        }
        if (strcmp(argv[i], "-i") == 0) {
            ignore_case = true;
            count++;
        } else if (strcmp(argv[i], "-n") == 0) {
            print_line_number = true;
            count++;
        }
    }

    if (argv[count] == NULL) {
        usage("myfgrep.c");
        return(EXIT_FAILURE);
    }

    if (strstr(argv[count], ".txt") != NULL) {
        strcat(pattern, " ");
    } else {
        strcat(pattern, argv[count]);
        count++;
        if (argv[count] == NULL) {
            usage("myfgrep.c");
            return(EXIT_FAILURE);
        }
        while (strstr(argv[count], ".txt") == NULL) {
            char temp[SIZE_CMD - sizeof(pattern)];
            strcat(temp, " ");
            strcat(temp, argv[count]);
            strcat(pattern, temp);
            *temp = '\0';
            count++;
            if (argv[count] == NULL) {
                usage("myfgrep.c");
                return(EXIT_FAILURE);
            }
        }
    }

    // Stores a list of all the filenames passed as arguments
    char* filename_list[SIZE];
    int index = 0;
    for (i = count; i <= argc; i++) {
        if (argv[i] != NULL) {
            filename_list[index] = argv[i];
            index++;
        }
    }

    // For each file, prints every line of text where the pattern is found
    print_all_matches(index, filename_list, pattern, ignore_case,
                      print_line_number);

    return(EXIT_SUCCESS);
}

