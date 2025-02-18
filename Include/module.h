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

typedef struct 
{
    int x;
    int y;
}coordinate;

char *slice(const char *str, int start, int end);
twostring split(const char *inp, char delim);
commandCall parse(const char *inp);
int is_valid_cell(const char *str);
int  addition (int x1,int y1,int x2,int y2,int row,int col,int arr[row][col]);
int  subtraction (int x1,int y1,int x2,int y2,int row,int col,int arr[row][col]);
int  multiply (int x1,int y1,int x2,int y2,int row,int col,int arr[row][col]);
int  maximum (int x1,int y1,int x2,int y2,int row,int col,int arr[row][col]);
int  minimum (int x1,int y1,int x2,int y2,int row,int col,int arr[row][col]);
int max(int a , int b);
int min(int a , int b);
void Display (int row, int col, int arr[row][col],char x,int rowi, int coli,int lastrow,int lastcol);

#endif 