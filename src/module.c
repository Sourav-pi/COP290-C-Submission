#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Include/module.h"

char *slice(const char *str, int start, int end)
{
    if (str == NULL) return NULL;
    int len = strlen(str);
    if (start < 0) start = len + start;
    if (end <= 0) end = len + end;
    if (start >= len || start >= end) return NULL;
    if (end > len) end = len;
    if (start < 0) start = 0;
    int slice_len = end - start;
    char *result = (char *)malloc(slice_len + 1);
    if (!result) return NULL;
    strncpy(result, str + start, slice_len);
    result[slice_len] = '\0';
    return result;
}

twostring split(const char *inp, char delim)
{
    twostring s;
    int i = 0;
    while (inp[i] != delim && inp[i] != '\0')
    {
        s.str1[i] = inp[i];
        i++;
    }
    s.str1[i] = '\0';
    if (inp[i] == delim) strcpy(s.str2, &inp[i + 1]);
    else s.str2[0] = '\0'; 
    return s;
}

int is_valid_cell(const char *str)
{
    int len = strlen(str);
    if (len < 2 || len > 6) return 0;
    int i = 0;
    while (i < len && str[i] >= 'A' && str[i] <= 'Z') i++;
    if (i < 1 || i > 3) return 0;
    while (i < len && str[i] >= '0' && str[i] <= '9') i++;
    if (i - (len - i) < 1 || i - (len - i) > 3) return 0;
    return i == len;
}

commandCall input(const char *inp)
{
    char cell[100], expr[100];
    char error[100] = "";
    const char *commands[] = {"exit", "w", "a", "s", "d", "disable_output", "enable_output"};
    const char *cmds[] = {"exit", "up", "left", "down", "right", "disable_output", "enable_output"};
    for (int i = 0; i < 7; i++) {
        if (strcmp(inp, commands[i]) == 0) {
            commandCall retVal;
            strcpy(retVal.cmd, cmds[i]);
            strcpy(retVal.param1, "");
            strcpy(retVal.param2, "");
            strcpy(retVal.error, "");
            return retVal;
        }
    }
    if (strncmp("scroll_to ", inp, strlen("scroll_to ")) == 0) {
        char *cell = slice(inp, strlen("scroll_to "), strlen(inp));
        commandCall retVal;
        strcpy(retVal.cmd, "scroll_to");
        strcpy(retVal.param1, cell);
        strcpy(retVal.param2, "");
        strcpy(retVal.error, "");
        return retVal;
    }

    twostring s = split(inp, '=');
    strcpy(cell, s.str1);
    strcpy(expr, s.str2);
    printf("Cell: %s\n", cell);
    printf("Expression: %s\n", expr);
    if (strcmp("", expr) == 0 || strcmp("", cell) == 0) strcpy(error, "Invalid expression");
    else
    {
        int ln = (strlen(expr) >= 7) ? 7 : strlen(expr);
        for (int i = 0; i < ln; i++)
        {
            if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/')
            {
                char cmd[100];
                switch (expr[i])
                {
                case '+':
                    strcpy(cmd, "add");
                    break;
                case '-':
                    strcpy(cmd, "sub");
                    break;
                case '*':
                    strcpy(cmd, "mul");
                    break;
                case '/':
                    strcpy(cmd, "div");
                    break;

                default:
                    break;
                }
                twostring splitted = split(expr, expr[i]);
                commandCall retVal;
                strcpy(retVal.cmd, cmd);
                strcpy(retVal.param1, splitted.str1);
                strcpy(retVal.param2, splitted.str2);
                return retVal;
            }
            else if (expr[i] == '(')
            {
                char *sliced1 = slice(expr, 0, i);
                char *sliced2 = slice(expr, i + 1, strlen(expr) - 1);
                twostring splitted = split(sliced2, ':');
                if (strcmp(sliced1, "SLEEP") == 0 && strcmp("", splitted.str2) != 0)
                {
                    strcpy(error, "Invalid expression");
                }
                else if (strcmp(sliced1, "MIN") == 0 || strcmp(sliced1, "MAX") == 0 || strcmp(sliced1, "AVG") == 0 || strcmp(sliced1, "SUM") == 0 || strcmp(sliced1, "STDEV") == 0)
                {
                    commandCall retVal;
                    strcpy(retVal.cmd, sliced1);
                    strcpy(retVal.param1, splitted.str1);
                    strcpy(retVal.param2, splitted.str2);
                    free(sliced1);
                    free(sliced2);
                    return retVal;
                }
                else strcpy(error, "Invalid expression");
            }
        }
        strcpy(error, "Invalid expression");
    }
    commandCall emptyRetVal = {"", "", "", ""};
    strcpy(emptyRetVal.error, error);
    return emptyRetVal;
}