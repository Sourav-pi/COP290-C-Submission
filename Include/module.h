#ifndef MODULE_H
#define MODULE_H

#include "hash_set.h"

#define  VAL 0;
#define  ART 1;
#define  FUNC 2;
#define  CMD 3;

#define  ADD 0;
#define  SUB 1;
#define  MUL 2;
#define  DIV 3;
#define  MIN 0;
#define  MAX 1;
#define  SUM 2;
#define  AVG 3;
#define  STDEV 4;
#define  SLEEP 5;
#define  SCROLL_TO 0;
#define  DISABLE_OUTPUT 1;
#define  ENABLE_OUTPUT 2;
#define  W 3;
#define  D 4;
#define  A 5;
#define  S 6;

#define  VAL 0;
#define  CELL 1;

#define OK 0;
#define INVALID 1;
#define CYCLE 2;

typedef struct
{   
    // val -> cmd is ""
    // art -> cmd is  add, sub, mul, div
    // func -> cmd is MIN,MAX,SUM,AVG,STDEV,SLEEP
    // cmd -> cmd is scroll_to, disable_output, enable_output, w, d, a, s  
     
    unsigned int type : 2;  // val, art, func, cmd    
    unsigned int cmd : 3;   // add, sub, mul, div, MIN, MAX, SUM, AVG, STDEV, SLEEP, scroll_to, disable_output, enable_output, w, d, a, s
    char target[7];
    char param1[7];         // any cell or value or "" 
    unsigned int type1 : 1; // val, cell
    char param2[7];         // any cell or value or ""
    unsigned int type2 : 1; // val, cell
    unsigned int error : 2;
} commandCall;


typedef struct
{
    int val;
    commandCall cmd;
    HashSet* dep;
    unsigned int isDivByZero : 1;
} cell;

typedef struct 
{
    int x;
    int y;
}coordinate;

commandCall parse(char *inp, int hasSign);
int is_valid_cell(char *str);
int addition (int x1,int y1,int x2,int y2,int row,int col,cell** arr);
int subtraction (int x1,int y1,int x2,int y2,int row,int col,cell** arr);
int multiply (int x1,int y1,int x2,int y2,int row,int col,cell** arr);
int max(int a , int b);
int min(int a , int b);
void Display(int row, int col, cell** arr, int rowi, int coli);
coordinate convert_to_index(char *str);
cell* create_new_cell() ;
void free_cell(cell* c);
void update(cell *tgt, cell **arr, int row, int col);
char **topological_sort(char *cell_name, cell **arr, int row, int col);
int isSigned(char* inp);
extern int rowmax ;
extern int colmax ;

#endif 
