#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Include/module.h"


int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    char inp[100];
    printf("n: %d\n", n);
    printf("m: %d\n", m);
    printf("Enter a string: ");
    scanf("%s", inp);
    commandCall opt =  input(inp);
    printf("Command: %s\n", opt.cmd);
    printf("Parameter 1: %s\n", opt.param1);
    printf("Parameter 2: %s\n", opt.param2);
    printf("Error: %s\n", opt.error);
    return 0;
}