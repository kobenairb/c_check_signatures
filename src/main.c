#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BINVERSION __BUILD_VERSION
#define BINDATE __BUILD_DATE

#define MAX_SIGNATURE_LENGTH 256
#define MAX_SIGNATURE_LINES 10240

typedef struct
{
    char **array;
    int length;
} Signatures;

void free_signatures(Signatures s)
{
    for (int i = 0; i < s.length; i++)
    {
        free(s.array[i]);
    }
    free(s.array);
}

void print_options()
{
    fprintf(stderr, "[ERROR]: %s\n", "Invalid arguments. Usage: program -c <c_file> -h <h_file>");
}

void print_version()
{
    char year[5];
    sprintf(year, "%.*s", 4, BINDATE);

    printf("816-tcc-opt v%s built date: %s\n", BINVERSION, BINDATE);
    printf("Copyright (c) %s coco l'asticot\n", year);
}

Signatures get_signatures(const char *filename)
{
    FILE *file = fopen(filename, "r");
    char line[MAX_SIGNATURE_LENGTH + 1];
    int count = 0;

    char **signatures = malloc(MAX_SIGNATURE_LINES * sizeof(char *));

    while (fgets(line, sizeof(line), file) && count < MAX_SIGNATURE_LINES)
    {
        char *pos = line;
        while (*pos != '\0' && !isspace(*pos))
        {
            pos++;
        }
        *pos = '\0';

        if (strcmp(line, "float") == 0 || strcmp(line, "double") == 0 || strcmp(line, "long") == 0 || strcmp(line, "short") == 0 || strcmp(line, "struct") == 0 || strcmp(line, "enum") == 0 || strcmp(line, "char") == 0 || strcmp(line, "void") == 0 || strcmp(line, "int") == 0 || strcmp(line, "unsigned") == 0 || strcmp(line, "extern") == 0 || strcmp(line, "static") == 0)
        {
            char *start = pos + 1;
            pos         = start;
            while (*pos != '\0' && *pos != '(')
            {
                pos++;
            }
            *pos              = '\0';
            signatures[count] = malloc(strlen(start) + 1);
            strcpy(signatures[count], start);
            count++;
        }
    }

    fclose(file);

    Signatures result = { signatures, count };
    return result;
}

int main(int argc, char *argv[])
{
    char *c_filename = NULL;
    char *h_filename = NULL;

    // Parse command line arguments to print version
    for (int i = 1; i < argc; i += 2)
    {
        if (strcmp(argv[i], "-v") == 0)
        {
            print_version();
            return 0;
        }
    }

    // Check that we have the correct number of arguments
    if (argc != 5)
    {
        print_options();
        return 1;
    }

    // Parse command line arguments
    for (int i = 1; i < argc; i += 2)
    {
        if (strcmp(argv[i], "-c") == 0)
        {
            c_filename = argv[i + 1];
        }
        else if (strcmp(argv[i], "-h") == 0)
        {
            h_filename = argv[i + 1];
        }
        else
        {
            print_options();
            return 1;
        }
    }

    // Check that both filenames were provided
    if (!c_filename || !h_filename)
    {
        fprintf(stderr, "[ERROR]: %s\n", "Invalid arguments. Usage: program -c <c_file> -h <h_file>");
        return 1;
    }

    Signatures c_file_signatures = get_signatures(c_filename);
    Signatures h_file_signatures = get_signatures(h_filename);

    int all_match = 1;

    if (c_file_signatures.length != h_file_signatures.length)
    {
        fprintf(stderr, "[WARNING]: Tne number of signatures in %s and %s differ\n", c_filename, h_filename);
        return 1;
    }

    for (int i = 0; i < h_file_signatures.length; i++)
    {
        int found = 0;
        for (int j = 0; j < c_file_signatures.length; j++)
        {
            if (strcmp(h_file_signatures.array[i], c_file_signatures.array[j]) == 0)
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            fprintf(stderr, "[WARNING]: Signature %s exists in the c file but not in the h file.\n", c_file_signatures.array[i]);
            all_match = 0;
        }
    }

    for (int i = 0; i < c_file_signatures.length; i++)
    {
        int found = 0;
        for (int j = 0; j < h_file_signatures.length; j++)
        {
            if (strcmp(c_file_signatures.array[i], h_file_signatures.array[j]) == 0)
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            fprintf(stderr, "[WARNING]: Signature %s exists in the h file but not in the c file.\n", h_file_signatures.array[i]);
            all_match = 0;
        }
    }

    free_signatures(c_file_signatures);
    free_signatures(h_file_signatures);

    if (all_match)
    {
        printf("[SUCCESS]: All signatures in the c file and header file match!\n");
    }
    else
        return 1;

    return 0;
}
