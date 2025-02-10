#ifndef MODULE_H
#define MODULE_H

typedef struct
{   
    // val -> cmd is ""
    // art -> cmd is  add, sub, mul, div
    // func -> cmd is MIN,MAX,SUM,AVG,STDEV,SLEEP

    char type[100];     // val, art, func, cmd
    char cmd[100];      // add, sub, mul, div, MIN, MAX, SUM, AVG, STDEV, SLEEP scroll_to, disable_output, enable_output, w, d, a, s
    char target[100];

    char param1[100];   // any cell or value or ""
    char type1[100];    // val, cell

    char param2[100];   // any cell or value or ""
    char type2[100];    // val, cell

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

typedef struct 
{
    int x;
    int y;
}coordinate;

commandCall parse(char *inp);
int is_valid_cell(const char *str);
int  addition (int x1,int y1,int x2,int y2,int row,int col,cell arr[row][col]);
int  subtraction (int x1,int y1,int x2,int y2,int row,int col,cell arr[row][col]);
int  multiply (int x1,int y1,int x2,int y2,int row,int col,cell arr[row][col]);
int  maximum (int x1,int y1,int x2,int y2,int row,int col,cell arr[row][col]);
int  minimum (int x1,int y1,int x2,int y2,int row,int col,cell arr[row][col]);
int max(int a , int b);
int min(int a , int b);
void Display (int row, int col, cell arr[row][col],char x,int rowi, int coli,int lastrow,int lastcol);
coordinate convert_to_index(char *str);

#endif 