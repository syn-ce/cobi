#include "progress_bar.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void print_progress(double percentage) {
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

void clear_progress_bar() {
  printf("\r%*s\r", PBWIDTH + 7, ""); // Clear line (7 is just enough to do remove the progress bar entirely)
}

void print_before_progress_bar(double percentage, FILE *f, char *message, ...) {
  va_list args;
  va_start(args, message);

  clear_progress_bar();
  vprintf(message, args);
  va_end(args);

  print_progress(percentage);
}


