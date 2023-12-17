#ifndef COUNT_BINARY_H
#define COUNT_BINARY_H
#include <inttypes.h>

typedef struct bit_count {
    uint64_t total;
    uint64_t zeros;
    uint64_t ones;
} bit_count;

bit_count *count_bits(char *path, int nr_files, int nr_files_counted);

int count_files(char *path);

bit_count *count_bits_in_children(char *path, int nr_files, int *nr_current_file);

void count_binary(char * path);

#endif // COUNT_BINARY_H
