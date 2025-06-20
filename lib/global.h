#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int GLOBAL_printLineInFile(char *filename, char *line);
int GLOBAL_validatePassword(char* password);
char GLOBAL_validateEmail(char *email);
char *GLOBAL_toLower(const char *str);
int GLOBAL_containsIgnoreCase(const char *text, const char *filter);

#endif