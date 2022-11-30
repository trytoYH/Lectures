/* DO NOT MODIFY HEADER FILE*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#define True 1
#define False -1
#define MAX_LEN 100
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define WHITE "\x1b[0m"

void create_file(char*);
void read_file(char*, int);
void write_file(char* , char* );
void mixup_copy_file(char*source, char*dest, int n);
int file(char*command);
void ls();
void LOGO();
void PROMPT(int studentid);
void PRINT_COMMAND_RESULT(int flag, char* command);

