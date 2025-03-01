#include<stdio.h>
#include<string.h>

int rowmax= 999;
int colmax= 18278;
typedef struct 
{
    int x;
    int y;
}coordinate;

coordinate convert_to_index(char *str)
{
    char rev_str[strlen(str) + 1];
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        rev_str[i] = str[len - i - 1];
    }
    rev_str[len] = '\0';
    int return_index[2] = {0, 0};
    int mul_fact = 10;
    int curr = 1;
    for (int i = 0; i < len; i++)
    {
        int val = rev_str[i];
        if (val < 65)
        {
            return_index[0] += (val - 48) * curr;
            curr *= mul_fact;
        }
        else
        {
            if (mul_fact == 10)
            {
                curr = 1;
                mul_fact = 26;
            }
            return_index[1] += (val - 64) * curr;
            curr *= mul_fact;
        }
    }
    coordinate c = {return_index[0] - 1, return_index[1] - 1};
    // printf("c.x:%d,c.y:%d\n", c.x, c.y);
    return c;
}

int is_valid_cell(char *str)
{
    int len = strlen(str);
    if (len < 2 || len > 6)
    {
        return 0;
    }
    int i = 0;
    while (i < len && str[i] >= 'A' && str[i] <= 'Z')
    {
        i++;
    }
    if (i < 1 || i > 3)
    {
        return 0;
    }
    int st = i;
    while (i < len && str[i] >= '0' && str[i] <= '9')
    {

        i++;
    }
    if (i - st < 1 || i - st > 3)
    {
        return 0;
    }
    if(i == len){
        coordinate c = convert_to_index(str);
        if(c.x >= rowmax || c.y >= colmax){
            return 0;
        }
        else{
            return 1;
        }
    }
    return 0;

}
typedef struct
{   
    // val -> cmd is ""
    // art -> cmd is  add, sub, mul, div
    // func -> cmd is MIN,MAX,SUM,AVG,STDEV,SLEEP
    // cmd -> cmd is scroll_to, disable_output, enable_output, w, d, a, s  
     
    unsigned int type : 2;  // val, art, func, cmd    
    unsigned int cmd : 3;   // add, sub, mul, div, MIN, MAX, SUM, AVG, STDEV, SLEEP, scroll_to, disable_output, enable_output, w, d, a, s
    unsigned int type1 : 1; // val, cell
    unsigned int type2 : 1; // val, cell
    unsigned int error : 2;
    unsigned int isDivByZero:1;
    // char target[7];
    // char param1[7];         // any cell or value or "" 
    int param1;
    // char param2[7];         // any cell or value or ""
    int param2;
} commandCall;
typedef struct
{
    int val;
    commandCall cmd;
    int* dep;
} cell;
typedef struct temp
{
    int target;

} Temp;



int main(int argc, char const *argv[])
{
    printf("%lu\n", sizeof(cell));
    return 0;
}
