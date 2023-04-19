## A simple C program to check signatures against a `.c` file and a `.h` file

A C program who take two arguments, -c which is the `.c` file to open and -h which the `.h` file to open.

- raise an error if the `.c` file is not found or if the `.h` file is not found.
- raise an error if the given arguments are not correct.

Parse each files, so the `.c` file and the `.h` file and store the result of each in a separate structs:

- raise an error if the parsing fails for any reason.

Then performs the following tests for each generated struct arrays:

- if the number of lines for each array is different, raise a warning.

- if signature exists in the `.c` file but not in the `.h` file, raise a warning.

- if signature exists in the `.h` file but not in the `.c` file, raise a warning.

- if all signatures in the `.c` file are present in the `.h` file AND all the signatures in the h file are present in the c file are present, print a success message.

