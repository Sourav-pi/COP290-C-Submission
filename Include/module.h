#ifndef MODULE_H
#define MODULE_H

typedef struct
{
    char str1[100];
    char str2[100];
} twostring;

typedef struct
{
    char cmd[100];
    char param1[100];
    char param2[100];
    char error[100];
} commandCall;

typedef struct
{
    int val;
    char cmd[100];
    char param1[100];
    char param2[100];
    // set

} cell;

char *slice(const char *str, int start, int end);
twostring split(const char *inp, char delim);
commandCall input(const char *inp);
int is_valid_cell(const char *str);

#endif 