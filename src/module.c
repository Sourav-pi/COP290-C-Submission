#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../Include/module.h"
#include "../Include/hash_set.h"
#include <math.h>
#define debug 0

// reverse a string in place
void rev(char *s)
{
    int l = 0;
    int r = strlen(s) - 1;
    char t;
    while (l < r)
    {
        t = s[l];
        s[l] = s[r];
        s[r] = t;
        l++;
        r--;
    }
}

int getOp(char* inp){
    if(strcmp(inp, "add") == 0) return ADD;
    if(strcmp(inp, "sub") == 0) return SUB;
    if(strcmp(inp, "mul") == 0) return MUL;
    if(strcmp(inp, "div") == 0) return DIV;
    if(strcmp(inp, "MAX") == 0) return MAX;
    if(strcmp(inp, "MIN") == 0) return MIN;
    if(strcmp(inp, "SUM") == 0) return SUM;
    if(strcmp(inp, "AVG") == 0) return AVG;
    if(strcmp(inp, "STDEV") == 0) return STDEV;
    if(strcmp(inp, "SLEEP") == 0) return SLEEP;
    return -1;
}

// checks if the cell is valid
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

// checks if the value is valid (number in string form)
int is_valid_val(char *str)
{
    if (*str == '-' || *str == '+')
    {
        str++;
    }
    while (*str)
    {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

// parses the input and returns the commandCall structure
char temp[6];

commandContainer parse(char *inp, int hasSign)
{
    char temp_target[10];
    char temp_param1[10];
    char temp_param2[10];
    commandCall cmd = {0};
    commandContainer cmdc;
    cmd.error = OK;
    char sym1[12];
    char sym2[12];

    // Handle simple commands
    if (strcmp(inp, "disable_output") == 0 )
    {
        cmd.type = CMD;
        cmd.cmd = DISABLE_OUTPUT;
    }
    else if (strcmp(inp, "enable_output") == 0)
    {
        cmd.type = CMD;
        cmd.cmd = ENABLE_OUTPUT;

    }
    else if ( strcmp(inp, "w") == 0)
    {
        cmd.type = CMD;
        cmd.cmd = W;
    }
    else if (strcmp(inp, "d") == 0 )
    {
        cmd.type = CMD;
        cmd.cmd = D;
    }
    else if ( strcmp(inp, "a") == 0 )
    {
        cmd.type = CMD;
        cmd.cmd = A;
    }
    else if (strcmp(inp, "s") == 0)
    {
        cmd.type = CMD;
        cmd.cmd = S;
    }

    else if (sscanf(inp, "scroll_to %s", temp_param1) == 1)
    {
        cmd.type = CMD;
        cmd.cmd = SCROLL_TO;
        if (is_valid_cell(temp_param1))
        {
            cmd.type1 = CELL;

        }
        else
        {
            cmd.error = INVALID;
            // printf("1");
        }
        cmd.param1 = encode_cell(temp_param1);
    }
    // Handle function commands
    else if (sscanf(inp, "%[^=]=%[^()](%[^:]:%[^)])", temp_target, temp, temp_param1, temp_param2) == 4)
    {
        // printf("temp_target:%s,temp:%s,temp_param1:%s,temp_param2:%s\n",temp_target,temp,temp_param1,temp_param2);
        if(is_valid_cell(temp_target)==0){
            cmd.error = INVALID;
            // printf("2");
            cmdc.cmd = cmd;
            return cmdc;
        }
        cmd.cmd = getOp(temp);
        if (cmd.cmd==1|| cmd.cmd == 0 || cmd.cmd == 2 || cmd.cmd == 4 || cmd.cmd == 5 || cmd.cmd == 3 )
        {
            cmd.type = FUNC;
            if (is_valid_cell(temp_param1))
            {
                cmd.type1=CELL;
            }
            else
            {
                cmd.error=INVALID;
                // printf("3");
                cmdc.cmd = cmd;
                return cmdc;
            }

           if (is_valid_cell(temp_param2))
            {
                cmd.type2=CELL;
            }
            else
            {
                cmd.error = INVALID;
                // printf("4");
                cmdc.cmd = cmd;
                return cmdc;
            }

            if(!is_valid_range(temp_param1,temp_param2)){
                cmd.error = INVALID;
                cmdc.cmd = cmd;
                return cmdc;
            }

            cmdc.target = encode_cell(temp_target);
            if(cmd.type1) {
                cmd.param1 = encode_cell(temp_param1);
            }
            else{
                cmd.param1 = atoi(temp_param1);
            }
            if(cmd.type2) {
                cmd.param2 = encode_cell(temp_param2);
            }
            else{
                cmd.param2 = atoi(temp_param2);
            }
        }
        else
        {
            cmd.error = INVALID;
        }
    }
    else if (sscanf(inp, "%[^=]=SLEEP(%[^)])", temp_target, temp_param1) == 2)
    {
        if(is_valid_cell(temp_target)==0){
            cmd.error = INVALID;
            // printf("6");
            cmdc.cmd = cmd;

            return cmdc;
        }
        cmd.type=FUNC;
        cmd.cmd = SLEEP;
        if (is_valid_val(temp_param1))
        {
            cmd.type1=VAL;
        }
        else if (is_valid_cell(temp_param1))
        {
            cmd.type1=CELL;
        }
        else
        {
            cmd.error = INVALID;
            // printf("7");
            cmdc.cmd = cmd;
            return cmdc;
        }
        cmdc.target = encode_cell(temp_target);
            if(cmd.type1) {
                cmd.param1 = encode_cell(temp_param1);
            }
            else{
                cmd.param1 = atoi(temp_param1);
            }
    }
    // Handle arithmetic commands
    else if (hasSign && sscanf(inp, "%[^=]=%[-+]%[^+-*/]%1[+-*/]%s", temp_target, sym1, temp_param1, temp, temp_param2) == 5)
    {
        if(is_valid_cell(temp_target)==0){
            cmd.error = INVALID;
            // printf("8");
            cmdc.cmd = cmd;
            return cmdc;
        }
        cmd.type = ART;

        switch (temp[0])
        {
        case '+':
            cmd.cmd=ADD;
            break;
        case '-':
            cmd.cmd = SUB;
            break;
        case '*':
            cmd.cmd = MUL;
            break;
        case '/':
            cmd.cmd = DIV;
            break;
        }

        if (is_valid_val(temp_param1))
        {
            cmd.type1=VAL;
        }
        else if (is_valid_cell(temp_param1))
        {
            cmd.type1=CELL;

        }
        else
        {
            cmd.error = INVALID;
            // printf("9");
            cmdc.cmd = cmd;
            return cmdc;
        }

        if (is_valid_val(temp_param2))
        {
            cmd.type2=VAL;
            
        }
        else if (is_valid_cell(temp_param2))
        {
            cmd.type2=CELL;
        }
        else
        {
            cmd.error=INVALID;
            cmdc.cmd = cmd;
            return cmdc;
        }

        if (strcmp(sym1, "-") == 0)
        {
            if (cmd.type1!=0)
            {
                cmd.error = INVALID;
                // printf("11");
                cmdc.cmd = cmd;
                return cmdc;
            }
            strcat(sym1, temp_param1);
            strcpy(temp_param1, sym1);
        }
        cmdc.target = encode_cell(temp_target);
            if(cmd.type1) {
                cmd.param1 = encode_cell(temp_param1);
            }
            else{
                cmd.param1 = atoi(temp_param1);
            }
            if(cmd.type2) {
                cmd.param2 = encode_cell(temp_param2);
            }
            else{
                cmd.param2 = atoi(temp_param2);
            }
    }
    else if (!hasSign && sscanf(inp, "%[^=]=%[^+-*/]%1[+-*/]%s", temp_target, temp_param1, temp, temp_param2) == 4)
    {
        // printf("temp_target:%s,temp_param1:%s,temp:%s,temp_param2:%s\n",temp_target,temp_param1,temp,temp_param2);
        if(is_valid_cell(temp_target)==0){
            cmd.error = INVALID;
            // printf("12");
            cmdc.cmd = cmd;
            return cmdc;
        }
        cmd.type = ART;

        switch (temp[0])
        {
        case '+':
            cmd.cmd = ADD;
            break;
        case '-':
            cmd.cmd = SUB;
            break;
        case '*':
            cmd.cmd = MUL;
            break;
        case '/':
            cmd.cmd = DIV;
            break;
        }

        if (is_valid_val(temp_param1))
        {
            cmd.type1 = VAL;
        }
        else if (is_valid_cell(temp_param1))
        {
            cmd.type1 = CELL;
        }
        else
        {
            cmd.error = INVALID;
            // printf("13");
            cmdc.cmd = cmd;
            return cmdc;
        }

        if (is_valid_val(temp_param2))
        {
            cmd.type2 = VAL;
        }
        else if (is_valid_cell(temp_param2))
        {
            cmd.type2 = CELL;
        }
        else
        {
            cmd.error = INVALID;
            // printf("14");
            cmdc.cmd = cmd;
            return cmdc;
        }
        cmdc.target = encode_cell(temp_target);
            if(cmd.type1) {
                cmd.param1 = encode_cell(temp_param1);
            }
            else{
                cmd.param1 = atoi(temp_param1);
            }
            if(cmd.type2) {
                cmd.param2 = encode_cell(temp_param2);
            }
            else{
                cmd.param2 = atoi(temp_param2);
            }
    }

    // Handle simple value/cell assignment
    else if (sscanf(inp, "%[^=]=%s", temp_target, temp_param1) == 2)
    {
        if(is_valid_cell(temp_target)==0){
            cmd.error = INVALID;
            // printf("15");
            cmdc.cmd = cmd;
            return cmdc;
        }
        cmd.type = VAL;
// set
        if (is_valid_val(temp_param1))
        {
            cmd.type1 = VAL;
        }
        else if (is_valid_cell(temp_param1))
        {
            cmd.type1 = CELL;
        
        }
        else
        {
            cmd.error = INVALID;
            // printf("16");
        }
    }
    else
    {
        cmd.error = INVALID;
        // printf("17");
    }
    cmdc.target = encode_cell(temp_target);
            if(cmd.type1) {
                cmd.param1 = encode_cell(temp_param1);
            }
            else{
                cmd.param1 = atoi(temp_param1);
            }
    // print cmdc formatted
    // printf("cmdc.target:%d,cmdc.cmd.type:%d,cmdc.cmd.cmd:%d,cmdc.cmd.type1:%d,cmdc.cmd.type2:%d,cmdc.cmd.error:%d,cmdc.cmd.isDivByZero:%d,cmdc.cmd.param1:%d,cmdc.cmd.param2:%d\n",cmdc.target,cmdc.cmd.type,cmdc.cmd.cmd,cmdc.cmd.type1,cmdc.cmd.type2,cmdc.cmd.error,cmdc.cmd.isDivByZero,cmdc.cmd.param1,cmdc.cmd.param2);
    cmdc.cmd = cmd;
    return cmdc;
}

// operations
int addition(int x1, int y1, int x2, int y2, int row, int col, cell **arr)
{
    if (x1 >= row || x2 >= row || y1 >= col || y2 >= col)
    {
        return 0;
    }
    return arr[x1][y1].val + arr[x2][y2].val;
}
int subtraction(int x1, int y1, int x2, int y2, int row, int col, cell **arr)
{
    if (x1 >= row || x2 >= row || y1 >= col || y2 >= col)
    {
        return 0;
    }
    return arr[x1][y1].val - arr[x2][y2].val;
}
int multiply(int x1, int y1, int x2, int y2, int row, int col, cell **arr)
{
    if (x1 >= row || x2 >= row || y1 >= col || y2 >= col)
    {
        return 0;
    }
    return arr[x1][y1].val * arr[x2][y2].val;
}
// int maximum(int x1, int y1, int x2, int y2, int row, int col, cell **arr)
// {
//     if (x1 >= row || x2 >= row || y1 >= col || y2 >= col)
//     {
//         return 0;
//     }
//     if (arr[x1][y1].val >= arr[x2][y2].val)
//     {
//         return arr[x1][y1].val;
//     }
//     else
//     {
//         return arr[x2][y2].val;
//     }
// }
int maximumrange(int x1, int y1, int x2, int y2, int row, int col, cell **arr, cell *tgt)
{
    int max = arr[x1][y1].val;
    for (int i = x1; i <= x2; i++)
    {
        for (int j = y1; j <= y2; j++)
        {
            if(arr[i][j].cmd.isDivByZero){
                tgt->cmd.isDivByZero=1;
                return 0;
            }
            if (max < arr[i][j].val)
            {
                max = arr[i][j].val;
            }
        }
    }
    return max;
}
int minimumrange(int x1, int y1, int x2, int y2, int row, int col, cell **arr, cell *tgt)
{
    int min = arr[x1][y1].val;
    for (int i = x1; i <= x2; i++)
    {
        for (int j = y1; j <= y2; j++)
        {
            if(arr[i][j].cmd.isDivByZero){
                tgt->cmd.isDivByZero=1;
                return 0;
            }
            if (min > arr[i][j].val)
            {
                min = arr[i][j].val;
            }
        }
    }
    return min;
}
int sumrange(int x1, int y1, int x2, int y2, int row, int col, cell **arr, cell *tgt)
{
    int sum = 0;
    for (int i = x1; i <= x2; i++)
    {
        for (int j = y1; j <= y2; j++)
        {
            if(arr[i][j].cmd.isDivByZero){
                tgt->cmd.isDivByZero=1;
                return 0;
            }
            sum += arr[i][j].val;
        }
    }
    return sum;
}
int avgrange(int x1, int y1, int x2, int y2, int row, int col, cell **arr, cell *tgt)
{
    int freq = (x2 - x1 + 1) * (y2 - y1 + 1);
    return sumrange(x1, y1, x2, y2, row, col, arr, tgt) / freq;
}
int stdev(int x1, int y1, int x2, int y2, int row, int col, cell **arr, cell *tgt)
{
    int mean = avgrange(x1, y1, x2, y2, row, col, arr, tgt);
    double sum_of_sq = 0;
    int n = (x2 - x1 + 1) * (y2 - y1 + 1);
    for (int i = x1; i <= x2; i++)
    {
        for (int j = y1; j <= y2; j++)
        {
            if(arr[i][j].cmd.isDivByZero){
                tgt->cmd.isDivByZero=1;
                return 0;
            }
            sum_of_sq+=(arr[i][j].val - mean)*(arr[i][j].val - mean);
        }
    }
    sum_of_sq/=n;
    return (int)round(sqrt(sum_of_sq));
}
// int minimum(int x1, int y1, int x2, int y2, int row, int col, cell **arr)
// {
//     if (x1 >= row || x2 >= row || y1 >= col || y2 >= col)
//     {
//         return 0;
//     }
//     if (arr[x1][y1].val <= arr[x2][y2].val)
//     {
//         return arr[x1][y1].val;
//     }
//     else
//     {
//         return arr[x2][y2].val;
//     }
// }
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
void col_convert(int col, char *str)
{
    int i = 0;
    while (col > 0)
    {
        int rem = col % 26;
        if (rem == 0)
        {
            str[i++] = 'Z';
            col = (col / 26) - 1;
        }
        else
        {
            str[i++] = (rem - 1) + 'A';
            col = col / 26;
        }
    }
    str[i] = '\0';
    rev(str);
}
// to display the 10*10 matrix
void Display(int row, int col, cell **arr, int rowi, int coli)
{
    printf("\t");
    for (int i = coli; i < coli + 10 && i < col; i++)
    {
        char str[4];
        col_convert(i + 1, str);
        printf("%s\t", str);
    }
    printf("\n");

    for (int i = rowi; i < rowi + 10 && i < row; i++)
    {
        printf("%d\t", i + 1);
        for (int j = coli; j < coli + 10 && j < col; j++)
        {
            if (arr[i][j].cmd.isDivByZero == 1)
            {
                printf("ERR\t");
            }
            else
            {

                printf("%d\t", arr[i][j].val);
            }
        }
        printf("\n");
    }
}

// converts the cell to index
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

cell *create_new_cell()
{
    cell *new_cell = (cell *)malloc(sizeof(cell));
    if (new_cell == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_cell->val = 0;
    memset(&(new_cell->cmd), 0, sizeof(commandCall));
    new_cell->dep = create_hashset();
    return new_cell;
}

void free_cell(cell *c)
{
    free_hashset(c->dep);
    free(c);
}

int topological_sort_util(int v, HashSet *visited, HashSet *stack, Node**sorted_cell, cell **arr)
{  
    Node* sorted_cells = *sorted_cell;
    if (contains(stack, v))
    {
        return 1;
    }

    if (!contains(visited, v))
    {
        insert(visited, v);
        insert(stack, v);
        coordinate dep_coord = decode_cell(v);
        cell *par = &arr[dep_coord.x][dep_coord.y];
        HashSet *set = par->dep;
        for (int i = 0; i < HASHSET_SIZE; i++)
        { // Loop through all buckets
            Node *current = set->buckets[i];
            while (current)
            { // Traverse the linked list in each bucket
                // callback(current->value);  // Call the callback function with the string
                int x = topological_sort_util(current->value, visited, stack, &sorted_cells, arr);
                if (x == 1)
                {
                    return 1;
                }
                current = current->next;
            }
        }

        remove_string(stack, v);
        Node *new_str = (Node *)malloc(sizeof(Node));
        // strcpy(new_str, v);
        // printf("v:%d\n",v);
        new_str->value=v;
        new_str->next=sorted_cells;
        sorted_cells=new_str;
        *sorted_cell = sorted_cells;
    }
    return 0;
}

Node* topological_sort(int cell_name, cell **arr, int row, int col)
{
    // printf("cell_name:%d\n",cell_name);
    coordinate coord = decode_cell(cell_name);
    cell *start = &arr[coord.x][coord.y];

    int total_cells = row * col;

    HashSet *visited = create_hashset();

    HashSet *stack = create_hashset();

    Node *sorted_cells = NULL;

    // if (sorted_cells == NULL)
    // {
    //     fprintf(stderr, "Memory allocation failed\n");
    //     exit(EXIT_FAILURE);
    // }
    // int index = 0;

    int is_cycle = topological_sort_util(cell_name, visited, stack, &sorted_cells, arr);
    if (is_cycle == 1)
    {
        // printf("Circular dependency\n");
        return NULL;
    }

    // free(sorted_cells);
    // if (debug)
    //     printf("index:%d\n", index);
    free_hashset(visited);
    free_hashset(stack);

    // for (int i = 0; i < index / 2; i++)
    // {
    //     char *temp = sorted_cells[i];
    //     sorted_cells[i] = sorted_cells[index - i - 1];
    //     sorted_cells[index - i - 1] = temp;
    // }
    // if (debug)
    // {
    //     printf("Sorted cells: ");
    //     for (int i = 0; i < index; i++)
    //     {
    //         printf("%s\n", sorted_cells[i]);
    //     }
    // }
    
    return sorted_cells;
}

void update(cell *tgt, cell **arr, int row, int col)
{
    commandCall parsed_inp = tgt->cmd;
    // printf("updated %d\n",parsed_inp.target);
    tgt->cmd.isDivByZero = 0;
    if (parsed_inp.type== 0)
    {
        if (parsed_inp.type1== 0)
        {
            tgt->val =  (parsed_inp.param1);
        }
        else
        {
            coordinate source1 = decode_cell(parsed_inp.param1);
            if (arr[source1.x][source1.y].cmd.isDivByZero)
            {
                tgt->cmd.isDivByZero = 1;
                tgt->val = 0;
            }
            else
            {
            tgt->val = arr[source1.x][source1.y].val;}
        }
    }
    else if (parsed_inp.type== 1)
    {
        if (parsed_inp.type1 == 0)
        {
            if (parsed_inp.type2 == 0)
            {
                if (parsed_inp.cmd== 0)
                {
                    tgt->val =  (parsed_inp.param1) +  (parsed_inp.param2);
                }
                else if (parsed_inp.cmd== 1)
                {
                    tgt->val =  (parsed_inp.param1) -  (parsed_inp.param2);
                }
                else if (parsed_inp.cmd== 2)
                {
                    tgt->val =  (parsed_inp.param1) *  (parsed_inp.param2);
                }
                else
                {
                    if( (parsed_inp.param2)==0){
                        tgt->val = 0;
                        tgt->cmd.isDivByZero = 1;
                    }
                    else{
                        tgt->val =  (parsed_inp.param1) /  (parsed_inp.param2);
                    }
                }
            }
            else
            {
                coordinate source2 = decode_cell(parsed_inp.param2);
                if (parsed_inp.cmd== 0)
                {
                    tgt->val =  (parsed_inp.param1) + arr[source2.x][source2.y].val;
                }
                else if (parsed_inp.cmd== 1)
                {
                    tgt->val =  (parsed_inp.param1) - arr[source2.x][source2.y].val;
                }
                else if (parsed_inp.cmd== 2) {
                    tgt->val =  (parsed_inp.param1) * arr[source2.x][source2.y].val;
                }
                else
                {
                    if(arr[source2.x][source2.y].val == 0){
                        tgt->cmd.isDivByZero = 1;
                        tgt->val = 0;
                    }
                    else{
                        tgt->val =  (parsed_inp.param1) / arr[source2.x][source2.y].val;
                    }
                }
            }
        }
        else
        {
            coordinate source1 = decode_cell(parsed_inp.param1);
            if(arr[source1.x][source1.y].cmd.isDivByZero){
                tgt->cmd.isDivByZero = 1;
                tgt->val = 0;
            }

            if (parsed_inp.type2== 0)
            {
                coordinate source1 = decode_cell(parsed_inp.param1);
                if (parsed_inp.cmd== 0)
                {
                    tgt->val =  (parsed_inp.param2) + arr[source1.x][source1.y].val;
                }
                else if (parsed_inp.cmd== 1)
                {
                    tgt->val = arr[source1.x][source1.y].val -  (parsed_inp.param2);
                }
                else if (parsed_inp.cmd== 2){
                    tgt->val =  (parsed_inp.param2) * arr[source1.x][source1.y].val;
                }
                else
                {
                    if( (parsed_inp.param2)==0){
                        tgt->val = 0;
                        tgt->cmd.isDivByZero = 1;
                    }
                    else{
                        tgt->val = arr[source1.x][source1.y].val /  (parsed_inp.param2);
                    }
                }
            }
            else
            {
                coordinate source1 = decode_cell(parsed_inp.param1);
                coordinate source2 = decode_cell(parsed_inp.param2);
                if(arr[source2.x][source2.y].cmd.isDivByZero){
                    tgt->cmd.isDivByZero = 1;
                    tgt->val = 0;
                }
                if (parsed_inp.cmd== 0)
                {
                    tgt->val = addition(source1.x, source1.y, source2.x, source2.y, row, col, arr);
                }
                else if (parsed_inp.cmd== 1)
                {
                    tgt->val = subtraction(source1.x, source2.y, source2.x, source2.y, row, col, arr);
                }
                else if (parsed_inp.cmd== 2)
                {
                    tgt->val = multiply(source1.x, source2.y, source2.x, source2.y, row, col, arr);
                }
                else if (parsed_inp.cmd== 3)
                {
                    if(arr[source2.x][source2.y].val == 0){
                        tgt->cmd.isDivByZero = 1;
                        tgt->val = 0;
                    }
                    else{
                        tgt->val = arr[source1.x][source1.y].val / arr[source2.x][source2.y].val;
                    }
                }
                else
                {
                    if(arr[source2.x][source2.y].val == 0){
                        tgt->cmd.isDivByZero = 1;
                        tgt->val = 0;
                    }
                    else{
                        tgt->val = arr[source1.x][source1.y].val / arr[source2.x][source2.y].val;
                    }
                }
            }
        }
    }
    else
    {
        if (parsed_inp.cmd== 1)
        {
            coordinate x1 = decode_cell(parsed_inp.param1);
            coordinate x2 = decode_cell(parsed_inp.param2);
            tgt->val = (maximumrange(x1.x, x1.y, x2.x, x2.y, row, col, arr, tgt));
        }
        else if (parsed_inp.cmd== 0)
        {
            coordinate x1 = decode_cell(parsed_inp.param1);
            coordinate x2 = decode_cell(parsed_inp.param2);
            tgt->val = (minimumrange(x1.x, x1.y, x2.x, x2.y, row, col, arr, tgt));
        }
        else if (parsed_inp.cmd== 2)
        {
            coordinate x1 = decode_cell(parsed_inp.param1);
            coordinate x2 = decode_cell(parsed_inp.param2);
            tgt->val = (sumrange(x1.x, x1.y, x2.x, x2.y, row, col, arr, tgt));
        }
        else if (parsed_inp.cmd== 3)
        {
            coordinate x1 = decode_cell(parsed_inp.param1);
            coordinate x2 = decode_cell(parsed_inp.param2);
            tgt->val = (avgrange(x1.x, x1.y, x2.x, x2.y, row, col, arr, tgt));
        }
        else if (parsed_inp.cmd== 4)
        {
            coordinate x1 = decode_cell(parsed_inp.param1);
            coordinate x2 = decode_cell(parsed_inp.param2);
            tgt->val = (stdev(x1.x, x1.y, x2.x, x2.y, row, col, arr, tgt));
        }
        else if (parsed_inp.cmd== 5)
        {
            if (parsed_inp.type1 != 0)
            {
                coordinate source1 = decode_cell(parsed_inp.param1);
                tgt->val = arr[source1.x][source1.y].val;
            }
        }
    }
}

int isSigned(char *inp)
{
    char *pt = inp;
    while (*pt != '=' && *pt != '\0')
    {
        pt++;
    }
    if (*pt == '\0')
    {
        return 0;
    }
    pt++;
    if (*pt == '\0')
    {
        return 0;
    }
    if (*pt == '-' || *pt == '+')
    {
        return 1;
    }
    return 0;
}

int encode_cell(char* cell_str){
    coordinate c =convert_to_index(cell_str);
    // printf("%d %d\n",c.x,c.y);
    // printf("%d \n", c.x*(100000)+c.y);
    return c.x*(100000)+c.y;
}

coordinate decode_cell(int cellcode){
    coordinate c;
    c.x = cellcode/100000;
    c.y = cellcode%100000;
    return c;
}

int is_valid_range(char* cell1, char* cell2){
    coordinate c1 = convert_to_index(cell1);
    coordinate c2 = convert_to_index(cell2);
    if(c1.x > c2.x || c1.y > c2.y){
        return 0;
    }
    return 1;
}