#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../Include/module.h"

//checks if the cell is valid 
int is_valid_cell(const char *str) {
    int len = strlen(str);
    if (len < 2 || len > 6)
        return 0;
    int i = 0;
    while (i < len && str[i] >= 'A' && str[i] <= 'Z')
        i++;
    if (i < 1 || i > 3)
        return 0;
    while (i < len && str[i] >= '0' && str[i] <= '9')
        i++;
    if (i - (len - i) < 1 || i - (len - i) > 3)
        return 0;
    return i == len;
}

//checks if the value is valid (number in string form)
int is_valid_val(char *str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

// parses the input and returns the commandCall structure
commandCall parse(char *inp) {
    commandCall cmd = {0}; 

    // Handle simple commands
    if(strcmp(inp,"disable_output")==0 || strcmp(inp,"enable_output")==0|| strcmp(inp,"w")==0||strcmp(inp,"d")==0||strcmp(inp,"a")==0||strcmp(inp,"s")==0){
        strcpy(cmd.type,"cmd");
        strcpy(cmd.cmd,inp);

    }
    else if(sscanf(inp,"scroll_to %s",cmd.param1)==1){
        strcpy(cmd.type,"cmd");
        strcpy(cmd.cmd,"scroll_to");
        if (is_valid_cell(cmd.param1)){
            strcpy(cmd.type1,"cell");
        }
        else{
            strcpy(cmd.error,"Invalid cell");
        }
    }
    // Handle function commands
    else if (sscanf(inp, "%[^=]=%[^()](%[^:]:%[^)])", cmd.target, cmd.cmd, cmd.param1, cmd.param2) == 4) { 
         if (strcmp(cmd.cmd, "MAX") == 0 || strcmp(cmd.cmd, "MIN") == 0 ||
                   strcmp(cmd.cmd, "SUM") == 0 || strcmp(cmd.cmd, "AVG") == 0 ||
                   strcmp(cmd.cmd, "STDEV") == 0) {
            strcpy(cmd.type, "func");
                if (is_valid_val(cmd.param1)) {
                strcpy(cmd.type1, "val"); 
            } else if (is_valid_cell(cmd.param1)) {
                strcpy(cmd.type1, "cell");
            } else {
                strcpy(cmd.error, "Invalid param1");
                return cmd;
            }

            if (is_valid_val(cmd.param2)) {
                strcpy(cmd.type2, "val");
            } else if (is_valid_cell(cmd.param2)) {
                strcpy(cmd.type2, "cell");
            } else {
                strcpy(cmd.error, "Invalid param2");
                return cmd;
            }

            if(strcmp(cmd.type1,cmd.type2)!=0){
                strcpy(cmd.error, "param1 and param2 type dont match");
                return cmd;
            }
            
        } else {
            strcpy(cmd.error, "Unknown command");
        }
    } else if(sscanf(inp,"SLEEP(%s)",cmd.param1)==1){
        strcpy(cmd.type,"func");
        strcpy(cmd.cmd,"SLEEP");
        if (is_valid_val(cmd.param1)){
            strcpy(cmd.type1,"val");
        }
        else{
            strcpy(cmd.error,"Invalid param1");
        }

    } 
    // Handle arithmetic commands
    else if(sscanf(inp, "%[^=]=%[^+-*/]%[+-*/]%s", cmd.target, cmd.param1,cmd.cmd, cmd.param2) == 4){
            strcpy(cmd.type, "art");
            
            switch (cmd.cmd[0]) {
                case '+':
                    strcpy(cmd.cmd, "add");
                    break;
                case '-':
                    strcpy(cmd.cmd, "sub");
                    break;
                case '*':
                    strcpy(cmd.cmd, "mul");
                    break;
                case '/':
                    strcpy(cmd.cmd, "div");
                    break;
            }

            if (is_valid_val(cmd.param1)) {
                strcpy(cmd.type1, "val"); 
            } else if (is_valid_cell(cmd.param1)) {
                strcpy(cmd.type1, "cell");
            } else {
                strcpy(cmd.error, "Invalid param1");
                return cmd;
            }

            if (is_valid_val(cmd.param2)) {
                strcpy(cmd.type2, "val");
            } else if (is_valid_cell(cmd.param2)) {
                strcpy(cmd.type2, "cell");
            } else {
                strcpy(cmd.error, "Invalid param2");
                return cmd;
            } 
            
            }
    // Handle simple value/cell assignment
    else if (sscanf(inp, "%[^=]=%s", cmd.target, cmd.param1) == 2) {
        strcpy(cmd.type, "val");
        strcpy(cmd.cmd, "set");

        if (is_valid_val(cmd.param1)) {
            strcpy(cmd.type1, "val");
        } else if (is_valid_cell(cmd.param1)) {
            strcpy(cmd.type1, "cell");
        } else {
            strcpy(cmd.error, "Invalid value or cell in param1");
        }
    } else {
        strcpy(cmd.error, "Invalid input format");
    }

    return cmd;
}

//operations
int addition(int x1, int y1, int x2, int y2, int row, int col, cell arr[row][col])
{
    if (x1 >= row || x2 >= row || y1 >= col || y2 >= col)
    {
        return 0;
    }
    return arr[x1][y1].val + arr[x2][y2].val;
}
int subtraction(int x1, int y1, int x2, int y2, int row, int col, cell arr[row][col])
{
    if (x1 >= row || x2 >= row || y1 >= col || y2 >= col)
    {
        return 0;
    }
    return arr[x1][y1].val - arr[x2][y2].val;
}
int multiply(int x1, int y1, int x2, int y2, int row, int col, cell arr[row][col])
{
    if (x1 >= row || x2 >= row || y1 >= col || y2 >= col)
    {
        return 0;
    }
    return arr[x1][y1].val * arr[x2][y2].val;
}
int maximum(int x1, int y1, int x2, int y2, int row, int col, cell arr[row][col])
{
    if (x1 >= row || x2 >= row || y1 >= col || y2 >= col)
    {
        return 0;
    }
    if (arr[x1][y1].val >= arr[x2][y2].val)
    {
        return arr[x1][y1].val;
    }
    else
    {
        return arr[x2][y2].val;
    }
}
int maximumrange(int x1, int y1, int x2, int y2, int row, int col, cell arr[row][col])
{
    int max = arr[x1][y1].val;
    for (int i = x1; i <= x2; i++)
    {
        for (int j = y1; j < y2; j++)
        {
            if (max < arr[i][j].val)
            {
                max = arr[i][j].val;
            }
        }
    }
    return max;
}
int minimumrange(int x1, int y1, int x2, int y2, int row, int col, cell arr[row][col])
{
    int min = arr[x1][y1].val;
    for (int i = x1; i <= x2; i++)
    {
        for (int j = y1; j < y2; j++)
        {
            if (min > arr[i][j].val)
            {
                min = arr[i][j].val;
            }
        }
    }
    return min;
}
int sumrange(int x1, int y1, int x2, int y2, int row, int col, cell arr[row][col])
{
    int sum = 0;
    for (int i = x1; i <= x2; i++)
    {
        for (int j = y1; j < y2; j++)
        {
            sum += arr[i][j].val;
        }
    }
    return sum;
}
int avgrange(int x1, int y1, int x2, int y2, int row, int col, cell arr[row][col])
{
    int freq = (x2 - x1 + 1) * (y2 - y1 + 1);
    return sumrange(x1, y1, x2, y2, row, col, arr) / freq;
}
int stdev(int x1, int y1, int x2, int y2, int row, int col, cell arr[row][col])
{
    int mean = avgrange(x1, y1, x2, y2, row, col, arr);
    int sum = 0;
    for (int i = x1; i <= x2; i++)
    {
        for (int j = y1; j < y2; j++)
        {
            int diff = arr[i][j].val - mean;
            sum += (diff * diff);
        }
    }
    return sum;
}
int minimum(int x1, int y1, int x2, int y2, int row, int col, cell arr[row][col])
{
    if (x1 >= row || x2 >= row || y1 >= col || y2 >= col)
    {
        return 0;
    }
    if (arr[x1][y1].val <= arr[x2][y2].val)
    {
        return arr[x1][y1].val;
    }
    else
    {
        return arr[x2][y2].val;
    }
}
int max(int a, int b)
{
    if (a >= b)
    {
        return a;
    }
    return b;
}
int min(int a, int b)
{
    if (a <= b)
    {
        return a;
    }
    return b;
}

// to display the 10*10 matrix
void Display(int row, int col, cell arr[row][col], char x, int rowi, int coli, int lastrow, int lastcol)
{
    if (rowi != -1 && coli != -1)
    {
        for (int i = rowi; i < rowi + 10 && i < rowi; i++)
        {
            for (int j = coli; j < coli + 10 && j < coli; j++)
            {
                printf("%d ", arr[i][j].val);
            }
            printf("\n");
        }
    }
    else
    {
        if (x == 'w')
        {
            int top_left_row = max(lastrow - 10, 0);
            int top_left_col = lastcol;
            for (int i = top_left_row; i < top_left_row + 10 && i < row; i++)
            {
                for (int j = top_left_col; j < top_left_col + 10 && j < col; j++)
                {
                    printf("%d ", arr[i][j].val);
                }
                printf("\n");
            }
        }
        else if (x == 's')
        {
            int top_left_row = min(lastrow + 10, row);
            int top_left_col = lastcol;
            for (int i = top_left_row; i <= top_left_row + 10 && i < row; i++)
            {
                for (int j = top_left_col; j <= top_left_col + 10 && j < col; j++)
                {
                    printf("%d ", arr[i][j].val);
                }
                printf("\n");
            }
        }
        else if (x == 'a')
        {
            int top_left_row = lastrow;
            int top_left_col = max(lastcol - 10, 0);
            for (int i = top_left_row; i <= top_left_row + 10 && i < row; i++)
            {
                for (int j = top_left_col; j <= top_left_col + 10 && j < col; j++)
                {
                    printf("%d ", arr[i][j].val);
                }
                printf("\n");
            }
        }
        else
        {
            int top_left_row = lastrow;
            int top_left_col = min(lastcol + 10, col);
            for (int i = top_left_row; i <= top_left_row + 10 && i < row; i++)
            {
                for (int j = top_left_col; j <= top_left_col + 10 && j < col; j++)
                {
                    printf("%d ", arr[i][j].val);
                }
                printf("\n");
            }
        }
    }
}