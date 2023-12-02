#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <dirent.h>
#include <string.h>

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
        if (digits_until_next_whitespace-- == 0) digits_until_next_whitespace = 2, printf(" ");
        printf("%c", buffer[i]);
    }
}

struct bit_count {
    uint64_t total;
    uint64_t zeros;
    uint64_t ones;
};

typedef struct bit_count bit_count;

bit_count *count_bits(char *path) {
    FILE *file;

    // Open file in binary mode
    file = fopen(path, "rb");

    bit_count *bit_c = malloc(sizeof(bit_count));
    if (bit_c == NULL) perror("Error during malloc\n"), exit(1);

    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", path);
        return bit_c;
    }

    bit_c->total = 0;
    bit_c->zeros = 0;
    bit_c->ones = 0;

    // Read the file byte by byte
    uint8_t byte_masks[] = {1, 2, 4, 8, 16, 32, 64, 128};

    int byte;
    while ((byte = fgetc(file)) != EOF) {
        // Count ones in byte
        for (int i = 0; i < 8; i++) {
            if (byte & byte_masks[i]) bit_c->ones++;
            else bit_c->zeros++;
        }
        bit_c->total += 8;
    }

    // Close file
    fclose(file);

    return bit_c;
}

int count_files(char *path) {
    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;
    
    dirp = opendir(path); /* There should be error handling after this */
    if (dirp == NULL) {
        fprintf(stderr, "Error when trying to open path %s\n", path); return file_count;
    }
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
             file_count++;
        } else if (entry->d_type == DT_DIR && strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,"..") != 0) {
            char *fullpath = malloc(strlen(entry->d_name) + strlen(path) + 2);
            if (fullpath == NULL) fprintf(stderr, "Error while allocating"), exit(1);
            sprintf(fullpath, "%s/%s", path, entry->d_name);
            file_count += count_files(fullpath);
            free(fullpath);
        };
    }
    closedir(dirp);
    return file_count;
}


bit_count *count_bits_in_subdirectories(char *path) {
    DIR * dirp;
    struct dirent * entry;

    bit_count *bit_c = malloc(sizeof(bit_count));
    if (bit_c == NULL) perror("Error during malloc\n"), exit(1);
    bit_c->total = 0;
    bit_c->zeros = 0;
    bit_c->ones = 0;
    
    dirp = opendir(path); /* There should be error handling after this */
    if (dirp == NULL) {
        fprintf(stderr, "Error when trying to open path %s\n", path); return bit_c;
    }
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
            char *fullpath = malloc(strlen(entry->d_name) + strlen(path) + 2);
            if (fullpath == NULL) fprintf(stderr, "Error while allocating"), exit(1);
            sprintf(fullpath, "%s/%s", path, entry->d_name);
            bit_count *bit_c_file = count_bits(fullpath);
            free(fullpath);
            bit_c->total += bit_c_file->total;
            bit_c->zeros += bit_c_file->zeros;
            bit_c->ones += bit_c_file->ones;
            free(bit_c_file);
        } else if (entry->d_type == DT_DIR && strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,"..") != 0) {
            char *fullpath = malloc(strlen(entry->d_name) + strlen(path) + 2);
            if (fullpath == NULL) fprintf(stderr, "Error while allocating"), exit(1);
            sprintf(fullpath, "%s/%s", path, entry->d_name);
            bit_count *bit_c_rec = count_bits_in_subdirectories(fullpath);
            bit_c->total += bit_c_rec->total;
            bit_c->zeros += bit_c_rec->zeros;
            bit_c->ones += bit_c_rec->ones;
            free(fullpath);
            free(bit_c_rec);
        };
    }
    closedir(dirp);
    return bit_c;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Please specify the path of the directory.\n");
        exit(1);
    }

    printf("This will look through %d files. Proceed? [y/n]", count_files(argv[1]));
    char answer = 'b';
    scanf(" %c", &answer);
    if (answer != 'y') printf("%c", answer), exit(0);

    bit_count *bit_c = count_bits_in_subdirectories(argv[1]);

    // Calc 0:1 ratio
    double ratio01;
    if (bit_c->ones == 0) ratio01 = 0.0;
        else ratio01 = bit_c->zeros / (double) bit_c->ones;

    double share0, share1; 
    if (bit_c->total == 0) share0 = 0, share1 = 0;
        else share0 = bit_c->zeros / (double) bit_c->total, share1 = bit_c->ones / (double) bit_c->total;

    int max_nr_digits = nr_digits(bit_c->total);

    printf("B: ");
    print_with_delim(bit_c->total, 0);
    printf("\n");

    printf("0: ");
    print_with_delim(bit_c->zeros, max_nr_digits - nr_digits(bit_c->zeros));
    printf("  (%lf)\n", share0);

    printf("1: ");
    print_with_delim(bit_c->ones, max_nr_digits - nr_digits(bit_c->ones));
    printf("  (%lf)\n", share1);

    return 0;
}
