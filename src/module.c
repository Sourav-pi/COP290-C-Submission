#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../Include/module.h"
#include "../Include/hash_set.h"


//reverse a string in place
void rev(char* s) {
    int l = 0;
    int r = strlen(s) - 1;
    char t;
    while (l < r) {
        t = s[l];
        s[l] = s[r];
        s[r] = t;
        l++;
        r--;
    }
}

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
void col_convert(int col,char* str){
    int i=0;
    while(col>0){
        int rem=col%26;
        if(rem==0){
            str[i++]='Z';
            col=(col/26)-1;
        }
        else{
            str[i++]=(rem-1)+'A';
            col=col/26;
        }
    }
    str[i]='\0';
    rev(str);
}
// to display the 10*10 matrix
void Display(int row, int col, cell** arr, int rowi, int coli)
{
        printf("\t");
        for (int i = coli; i < coli + 10 && i < col; i++)
        {
            char str[4];
            col_convert(i+1,str);
            printf("%s\t",str); 
        }
        printf("\n");
        
        for (int i = rowi; i < rowi + 10 && i < row; i++)
        {
            printf("%d\t", i+1);
            for (int j = coli; j < coli + 10 && j < col; j++)
            {
                printf("%d\t", arr[i][j].val);
            }
            printf("\n");
        }
}

//converts the cell to index
coordinate convert_to_index(char *str)
{
    char rev_str[strlen(str) + 1];
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        rev_str[i] = str[len - i - 1];
    }
    rev_str[len] = '\0';
    int return_index[2]={0,0};
    int mul_fact=10;
    int curr=1;
    for (int i = 0; i < len; i++) {
        int val = rev_str[i];
        if (val<65){
            return_index[0]+=(val-48)*curr;
            curr*=mul_fact;
        }
        else{
            if (mul_fact==10){
                curr=1; 
                mul_fact=26;
            }
            return_index[1]+=(val-64)*curr;
            curr*=mul_fact;
        }
        
    }
    coordinate c={return_index[0],return_index[1]};
    return c;
}

cell* create_new_cell() {
    cell* new_cell = (cell*)malloc(sizeof(cell));
    if (new_cell == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_cell->val = 0;
    memset(&(new_cell->cmd), 0, sizeof(commandCall));
    new_cell->dep = create_hashset(); 
    return new_cell;
}

void free_cell(cell* c) {
    free_hashset(c->dep);
    free(c);
}
// coordinate cell_name_to_coordinates(const char *cell_name) {
//     coordinate coord = {0, 0};
//     int len = strlen(cell_name);
//     int i = 0;

//     // Extract column part (letters)
//     while (i < len && isalpha(cell_name[i])) {
//         coord.y = coord.y * 26 + (toupper(cell_name[i]) - 'A' + 1);
//         i++;
//     }
//     coord.y--; // Convert to 0-based index

//     // Extract row part (numbers)
//     while (i < len && isdigit(cell_name[i])) {
//         coord.x = coord.x * 10 + (cell_name[i] - '0');
//         i++;
//     }
//     coord.x--; // Convert to 0-based index

//     return coord;
// }
