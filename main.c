#include <stdio.h>
#include <stdlib.h>
#include "src/count_binary.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Please specify the path of the directory.\n");
        exit(1);
    }
    count_binary(argv[1]);
}
