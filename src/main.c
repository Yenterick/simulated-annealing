#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    printf("\n");

    /* Checks if the parameters were correctly given */
    if (argc == 1) {
        printf("Not enough arguments were given...\n");
        printf("Example: annealing ./input.txt ./output.txt \n\n");
        return 1;
    } 

    /* Tries to open the file and checks if it was opened successfully */
    FILE* input = fopen(argv[1], "r");

    if (input == NULL) {
        printf("The file couldn't be opened...\n\n");
        return 1;
    }

    /* Creates the output file */
    FILE* output;

    if (argv[2]) {
        output = fopen(argv[2], "w");
    } else {
        output = fopen("./output.txt", "w");
    }

    /* Closes the file connections */
    fclose(input);
    fclose(output);

    return 0;
}