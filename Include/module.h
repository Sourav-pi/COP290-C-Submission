#ifndef MODULE_H
#define MODULE_H

#include "hash_set.h"


// for commandCall.type
#define  VAL 0
#define  ART 1
#define  FUNC 2
#define  CMD 3

// for commandCall.cmd
    // when type is ART
#define  ADD 0
#define  SUB 1
#define  MUL 2
#define  DIV 3
    // when type is FUNC
#define  MIN 0
#define  MAX 1
#define  SUM 2
#define  AVG 3
#define  STDEV 4
#define  SLEEP 5
    // when type is CMD
#define  SCROLL_TO 0
#define  DISABLE_OUTPUT 1
#define  ENABLE_OUTPUT 2
#define  W 3
#define  D 4
#define  A 5
#define  S 6

// for commandCall.type1 and commandCall.type2
#define  VAL 0
#define  CELL 1

// for commandCall.error
#define OK 0
#define INVALID 1
#define CYCLE 2

typedef struct
{   
    // When type is
    // VAL  -> cmd is ""
    // ART  -> cmd is ADD,SUB,MUL,DIV
    // FUNC -> cmd is MIN,MAX,SUM,AVG,STDEV,SLEEP
    // CMD  -> cmd is SCROLL_TO,DISABLE_OUTPUT,ENABLE_OUTPUT,W,D,A,S
     
    unsigned int type : 2;  // VAL, ART, FUNC, CMD  
    unsigned int cmd : 3;   // ADD, SUB, MUL, DIV, MIN, MAX, SUM, AVG, STDEV, SLEEP, SCROLL_TO, DISABLE_OUTPUT, ENABLE_OUTPUT, W, D, A, S
    unsigned int type1 : 1; // VAL, CELL
    unsigned int type2 : 1; // VAL, CELL
    unsigned int error : 2; // OK, INVALID, CYCLE
    char isDivByZero;       // 0 or 1
    int param1;             // any cell (encoded) or value 
    int param2;             // any cell (encoded) or value 
} commandCall;

typedef struct {
    int target;             // any cell (encoded) 
    commandCall cmd;

} commandContainer;

typedef struct
{
    int val;                // value of cell
    commandCall cmd;
    HashSet* dep;           // HashSet of int (encoded cells)
} cell;

typedef struct 
{
    int x;
    int y;
}coordinate;

commandContainer parse(char *inp, int hasSign);
int is_valid_cell(char *str);
int is_valid_val(char *str);
int is_valid_range(char* cell1, char* cell2);
int addition (int x1,int y1,int x2,int y2,int row,int col,cell** arr);
int subtraction (int x1,int y1,int x2,int y2,int row,int col,cell** arr);
int multiply (int x1,int y1,int x2,int y2,int row,int col,cell** arr);
int max(int a , int b);
int min(int a , int b);
void Display(int row, int col, cell** arr, int rowi, int coli);
coordinate convert_to_index(char *str);
void update(cell *tgt, cell **arr, int row, int col);
Node*topological_sort(int cell_name, cell **arr, int row, int col);
int isSigned(char* inp);
int encode_cell(char* cell_str);
coordinate decode_cell(int cellcode);
void remove_whitespace(char *str);
int getOp(char* inp);
extern int rowmax ;
extern int colmax ;
extern float sleep_time;

#endif 
