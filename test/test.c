#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../src/count_binary.h"

#define STDOUT_FD_NR 1

// main result return code used by redefined assert_equals
static int rslt;

// Test suite main variables
static int done;
static int num_tests;
static int tests_passed;

// Define assert_equals to set a boolean flag
void assert_equals(int r, int val) {
    rslt = rslt && (r == val);
    if (r != val) {
      printf("SHOULD: %d, WAS: %d ... ", val, r);
    }
}

// Define assert_equals to set a boolean flag
void assert_equals_uint64_t(uint64_t r, uint64_t val) {
    rslt = rslt && (r == val);
    if (r != val) {
      printf("SHOULD: %ld, WAS: %ld ... ", val, r);
    }
}

// Used for "silencing" the tested functions (prevent them from printing to stdout)
int silentfd;
int savedstdoutfd;

//  Utility function
void test_start(char *name)
{
    num_tests++;
    rslt = 1;
    printf("-- Testing %s ... ", name);
}

//  Utility function
void test_end()
{
    if (rslt) tests_passed++;
    printf("%s\n", rslt ? "SUCCESS" : "FAIL");
}

void silence_stdout() {
    fflush(stdout); dup2(silentfd, STDOUT_FD_NR);
}

void desilence_stdout() {
    fflush(stdout); dup2(savedstdoutfd, STDOUT_FD_NR);
}

// Test case
void test_count_files_files0()
{
    test_start("files/files0");

    silence_stdout();
    int r = count_files("files/files0");
    desilence_stdout();

    assert_equals(r, 0);

    test_end();
}

// Test case
void test_count_files_files1()
{
    test_start("test_count_files_files1");

    silence_stdout();
    int r = count_files("files/files1");
    desilence_stdout();

    assert_equals(r, 1);

    test_end();
}

// Test case
void test_count_files_files2()
{
    test_start("test_count_files_files2");

    silence_stdout();
    int r = count_files("files/files2");
    desilence_stdout();

    assert_equals(r, 2);

    test_end();
}

// Test case
void test_count_files_files()
{
    test_start("test_count_files_files");

    silence_stdout();
    int r = count_files("files");
    desilence_stdout();

    assert_equals(r, 3);

    test_end();
}

// Test case
void test_count_bits_in_children_files()
{
    test_start("test_count_bits_in_children_files");

    int _ = 0;
    silence_stdout();
    bit_count *bit_c = count_bits_in_children("files", 3, &_);
    desilence_stdout();
    assert_equals(bit_c->total, 8);
    assert_equals(bit_c->zeros, 6);
    assert_equals(bit_c->ones, 2);
    free(bit_c);

    test_end();
}

// Test case
void test_count_bits_in_children_files2datatxt()
{
    test_start("test_count_bits_in_children_files2datatxt");

    int _ = 0;
    silence_stdout();
    bit_count *bit_c = count_bits_in_children("files/files2/data.txt", 2, &_);
    desilence_stdout();
    assert_equals(bit_c->total, 0);
    assert_equals(bit_c->zeros, 0);
    assert_equals(bit_c->ones, 0);
    free(bit_c);

    test_end();
}

int main()
{
    num_tests = 0;
    tests_passed = 0;
    done = 0;
    silentfd = open("/dev/null", O_WRONLY);
    savedstdoutfd = dup(STDOUT_FD_NR);

    test_count_files_files0();
    test_count_files_files1();
    test_count_files_files();

    test_count_bits_in_children_files();
    test_count_bits_in_children_files2datatxt();

    printf("\nTotal tests passed: %d / %d\n", tests_passed, num_tests);
    done = 1;

    return !(tests_passed == num_tests);
}
