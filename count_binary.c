#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

int nr_digits(uint64_t n) {
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;
    if (n < 10000000000) return 10;
    if (n < 100000000000) return 11;
    if (n < 1000000000000) return 12;
    if (n < 100000000000000) return 14;
    if (n < 1000000000000000) return 15;
    if (n < 10000000000000000) return 16;
    if (n < 100000000000000000) return 17;
    if (n < 1000000000000000000) return 18;
    return -1;
}

void print_with_delim(uint64_t n, int leading_whitespace) {
    int nr_dig = nr_digits(n);
    int digits_until_next_whitespace = nr_dig % 3;
    if (digits_until_next_whitespace == 0) digits_until_next_whitespace = 3;
    char buffer[21];  // 20 digits for uint64_t + 1 for null terminator
    // Use sprintf to convert uint64_t to string
    sprintf(buffer, "%" PRIu64, n);
    printf("%*s", leading_whitespace, "");
    for (int i = 0; i < nr_dig; i++) {
        if (digits_until_next_whitespace-- == 0) digits_until_next_whitespace = 3, printf(" ");
        printf("%c", buffer[i]);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "File name needs to be specified as input parameter.\n");
        exit(1);
    }

    FILE *file;
    char *filename = argv[1];

    // Open file in binary mode
    file = fopen(filename, "rb");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    uint64_t count_tot = 0;
    uint64_t count_0 = 0;
    uint64_t count_1 = 0;

    // Read the file byte by byte
    uint8_t byte_masks[] = {1, 2, 4, 8, 16, 32, 64, 128};

    int byte;
    while ((byte = fgetc(file)) != EOF) {
        // Count ones in byte
        for (int i = 0; i < 8; i++) {
            if (byte & byte_masks[i]) count_1++;
            else count_0++;
        }
        count_tot += 8;
    }

    // Close file
    fclose(file);

    // Calc 0:1 ratio
    double ratio01;
    if (count_1 == 0) ratio01 = 0.0;
        else ratio01 = count_0 / (double) count_1;

    double share0, share1;
    if (count_tot == 0) share0 = 0, share1 = 0;
        else share0 = count_0 / (double) count_tot, share1 = count_1 / (double) count_tot;

    int max_nr_digits = nr_digits(count_tot);

    int digits_until_next_whitespace = max_nr_digits % 3;
    if (digits_until_next_whitespace == 0) digits_until_next_whitespace = 3;


    printf("B: ");
    print_with_delim(count_tot, 0);
    printf("\n");

    printf("0: ");
    print_with_delim(count_0, max_nr_digits - nr_digits(count_0));
    printf("  (%lf)\n", share0);

    printf("1: ");
    print_with_delim(count_1, max_nr_digits - nr_digits(count_1));
    printf("  (%lf)\n", share1);

    return 0;
}
