#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void print_progress(double);

void clear_progress_bar();

void print_before_progress_bar(double, FILE *, char *, ...);


#endif // PROGRESS_BAR_H 
