#ifndef MODULE_H
#define MODULE_H



typedef struct
{   
    // val -> cmd is ""
    // art -> cmd is  add, sub, mul, div
    // func -> cmd is MIN,MAX,SUM,AVG,STDEV,SLEEP
    // cmd -> cmd is scroll_to, disable_output, enable_output, w, d, a, s  
    
    char type[5];     // val, art, func, cmd
    char cmd[15];      // add, sub, mul, div, MIN, MAX, SUM, AVG, STDEV, SLEEP, scroll_to, disable_output, enable_output, w, d, a, s
    char target[8];
    char param1[8];   // any cell or value or ""
    char type1[5];    // val, cell
    char param2[8];   // any cell or value or ""
    char type2[5];    // val, cell
    char error[10];
} commandCall;

#include "hash_set.h"
typedef struct
{
    int val;
    commandCall cmd;
    HashSet* dep;
} cell;

typedef struct 
{
    int x;
    int y;
}coordinate;

commandCall parse(char *inp);
int is_valid_cell(const char *str);
int addition (int x1,int y1,int x2,int y2,int row,int col,cell** arr);
int subtraction (int x1,int y1,int x2,int y2,int row,int col,cell** arr);
int multiply (int x1,int y1,int x2,int y2,int row,int col,cell** arr);
int maximum (int x1,int y1,int x2,int y2,int row,int col,cell** arr);
int minimum (int x1,int y1,int x2,int y2,int row,int col,cell** arr);
int max(int a , int b);
int min(int a , int b);
void Display(int row, int col, cell** arr, int rowi, int coli);
coordinate convert_to_index(char *str);
cell* create_new_cell() ;
void free_cell(cell* c);
void update(cell *tgt, cell **arr, int row, int col);
char **topological_sort(char *cell_name, cell **arr, int row, int col);

#endif 