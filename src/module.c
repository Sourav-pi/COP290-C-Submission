#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../Include/module.h"
#include "../Include/hash_set.h"

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

// checks if the cell is valid
int is_valid_cell(const char *str)
{
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

// checks if the value is valid (number in string form)
int is_valid_val(char *str)
{
    while (*str)
    {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

// parses the input and returns the commandCall structure
commandCall parse(char *inp)
{
    commandCall cmd = {0};

    // Handle simple commands
    if (strcmp(inp, "disable_output") == 0 || strcmp(inp, "enable_output") == 0 || strcmp(inp, "w") == 0 || strcmp(inp, "d") == 0 || strcmp(inp, "a") == 0 || strcmp(inp, "s") == 0)
    {
        strcpy(cmd.type, "cmd");
        strcpy(cmd.cmd, inp);
    }
    else if (sscanf(inp, "scroll_to %s", cmd.param1) == 1)
    {
        strcpy(cmd.type, "cmd");
        strcpy(cmd.cmd, "scroll_to");
        if (is_valid_cell(cmd.param1))
        {
            strcpy(cmd.type1, "cell");
        }
        else
        {
            strcpy(cmd.error, "Invalid cell");
        }
    }
    // Handle function commands
    else if (sscanf(inp, "%[^=]=%[^()](%[^:]:%[^)])", cmd.target, cmd.cmd, cmd.param1, cmd.param2) == 4)
    {
        if (strcmp(cmd.cmd, "MAX") == 0 || strcmp(cmd.cmd, "MIN") == 0 ||
            strcmp(cmd.cmd, "SUM") == 0 || strcmp(cmd.cmd, "AVG") == 0 ||
            strcmp(cmd.cmd, "STDEV") == 0)
        {
            strcpy(cmd.type, "func");
            if (is_valid_val(cmd.param1))
            {
                strcpy(cmd.type1, "val");
            }
            else if (is_valid_cell(cmd.param1))
            {
                strcpy(cmd.type1, "cell");
            }
            else
            {
                strcpy(cmd.error, "Invalid param1");
                return cmd;
            }

            if (is_valid_val(cmd.param2))
            {
                strcpy(cmd.type2, "val");
            }
            else if (is_valid_cell(cmd.param2))
            {
                strcpy(cmd.type2, "cell");
            }
            else
            {
                strcpy(cmd.error, "Invalid param2");
                return cmd;
            }

            if (strcmp(cmd.type1, cmd.type2) != 0)
            {
                strcpy(cmd.error, "param1 and param2 type dont match");
                return cmd;
            }
        }
        else
        {
            strcpy(cmd.error, "Unknown command");
        }
    }
    else if (sscanf(inp, "SLEEP(%s)", cmd.param1) == 1)
    {
        strcpy(cmd.type, "func");
        strcpy(cmd.cmd, "SLEEP");
        if (is_valid_val(cmd.param1))
        {
            strcpy(cmd.type1, "val");
        }
        else
        {
            strcpy(cmd.error, "Invalid param1");
        }
    }
    // Handle arithmetic commands
    else if (sscanf(inp, "%[^=]=%[^+-*/]%[+-*/]%s", cmd.target, cmd.param1, cmd.cmd, cmd.param2) == 4)
    {
        strcpy(cmd.type, "art");

        switch (cmd.cmd[0])
        {
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

        if (is_valid_val(cmd.param1))
        {
            strcpy(cmd.type1, "val");
        }
        else if (is_valid_cell(cmd.param1))
        {
            strcpy(cmd.type1, "cell");
        }
        else
        {
            strcpy(cmd.error, "Invalid param1");
            return cmd;
        }

        if (is_valid_val(cmd.param2))
        {
            strcpy(cmd.type2, "val");
        }
        else if (is_valid_cell(cmd.param2))
        {
            strcpy(cmd.type2, "cell");
        }
        else
        {
            strcpy(cmd.error, "Invalid param2");
            return cmd;
        }
    }
    // Handle simple value/cell assignment
    else if (sscanf(inp, "%[^=]=%s", cmd.target, cmd.param1) == 2)
    {
        strcpy(cmd.type, "val");
        strcpy(cmd.cmd, "set");

        if (is_valid_val(cmd.param1))
        {
            strcpy(cmd.type1, "val");
        }
        else if (is_valid_cell(cmd.param1))
        {
            strcpy(cmd.type1, "cell");
        }
        else
        {
            strcpy(cmd.error, "Invalid value or cell in param1");
        }
    }
    else
    {
        strcpy(cmd.error, "Invalid input format");
    }

    return cmd;
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
int maximum(int x1, int y1, int x2, int y2, int row, int col, cell **arr)
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
int maximumrange(int x1, int y1, int x2, int y2, int row, int col, cell **arr)
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
int minimumrange(int x1, int y1, int x2, int y2, int row, int col, cell **arr)
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
int sumrange(int x1, int y1, int x2, int y2, int row, int col, cell **arr)
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
int avgrange(int x1, int y1, int x2, int y2, int row, int col, cell **arr)
{
    int freq = (x2 - x1 + 1) * (y2 - y1 + 1);
    return sumrange(x1, y1, x2, y2, row, col, arr) / freq;
}
int stdev(int x1, int y1, int x2, int y2, int row, int col, cell **arr)
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
int minimum(int x1, int y1, int x2, int y2, int row, int col, cell **arr)
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
            printf("%d\t", arr[i][j].val);
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


void topological_sort_util(char *v, HashSet *visited, HashSet *stack, char **sorted_cells, int *index, cell **arr)
{
    
    if (contains(stack, v))
    {
        fprintf(stderr, "Cycle detected in the graph\n");
        exit(EXIT_FAILURE);
    }

    if (!contains(visited, v))
    {
        insert(visited, v);
        insert(stack, v);
        coordinate dep_coord = convert_to_index(v);
        cell *par = &arr[dep_coord.x][dep_coord.y];
        HashSet *set = par->dep;
        for (int i = 0; i < HASHSET_SIZE; i++)
        { // Loop through all buckets
            Node *current = set->buckets[i];
            while (current)
            { // Traverse the linked list in each bucket
                // callback(current->value);  // Call the callback function with the string
                topological_sort_util(current->value, visited, stack, sorted_cells, index, arr);
                current = current->next;
            }
        }

        remove_string(stack, v);
        char *new_str = (char *)malloc(strlen(v) + 1);
        strcpy(new_str, v);
        sorted_cells[(*index)++] = new_str;
    }
}

char **topological_sort(char *cell_name, cell **arr, int row, int col)
{
    coordinate coord = convert_to_index(cell_name);
    cell *start = &arr[coord.x][coord.y];
    printf("hello1\n");
    int total_cells = row*col;
    printf("hello2\n");
    HashSet *visited = create_hashset();
    printf("hello3\n");
    HashSet *stack = create_hashset();
    printf("hello4\n");
    char **sorted_cells = (char **)malloc(total_cells * sizeof(char *));
    printf("hello5\n");
    if (sorted_cells == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    int index = 0;
    printf("hello6\n");
    
    topological_sort_util(cell_name, visited, stack, sorted_cells, &index, arr);
    printf("hello7\n");

    // free(sorted_cells);
    free_hashset(visited);
    free_hashset(stack);
    printf("index:%d\n", index);

    for (int i = 0; i < index / 2; i++)
    {
        char *temp = sorted_cells[i];
        sorted_cells[i] = sorted_cells[index - i - 1];
        sorted_cells[index - i - 1] = temp;
    }
    printf("hello8\n");
    return sorted_cells;
}

void update(cell *tgt, cell **arr, int row, int col)
{
    commandCall parsed_inp = tgt->cmd;
    if (strcmp(parsed_inp.type, "val") == 0)
    {
        if (strcmp(parsed_inp.type1, "val") == 0)
        {
            tgt->val = atoi(parsed_inp.param1);
        }
        else
        {
            coordinate source1 = convert_to_index(parsed_inp.param1);
            tgt->val = arr[source1.x][source1.y].val;
        }
    }
    else if (strcmp(parsed_inp.type, "art") == 0)
    {
        if (strcmp(parsed_inp.type1, "val") == 0)
        {
            if (strcmp(parsed_inp.type2, "val") == 0)
            {
                if (strcmp(parsed_inp.cmd, "add") == 0)
                {
                    tgt->val = atoi(parsed_inp.param1) + atoi(parsed_inp.param2);
                }
                else if (strcmp(parsed_inp.cmd, "sub") == 0)
                {
                    tgt->val = atoi(parsed_inp.param1) - atoi(parsed_inp.param2);
                }
                else if (strcmp(parsed_inp.cmd, "mul") == 0)
                {
                    tgt->val = atoi(parsed_inp.param1) * atoi(parsed_inp.param2);
                }
                else
                {
                    tgt->val = atoi(parsed_inp.param1) / atoi(parsed_inp.param2);
                }
            }
            else
            {
                coordinate source2 = convert_to_index(parsed_inp.param2);
                if (strcmp(parsed_inp.cmd, "add") == 0)
                {
                    tgt->val = atoi(parsed_inp.param1) + arr[source2.x][source2.y].val;
                }
                else if (strcmp(parsed_inp.cmd, "sub") == 0)
                {
                    tgt->val = atoi(parsed_inp.param1) - arr[source2.x][source2.y].val;
                }
                else if (strcmp(parsed_inp.cmd, "mul") == 0)
                {
                    tgt->val = atoi(parsed_inp.param1) * arr[source2.x][source2.y].val;
                }
                else
                {
                    tgt->val = atoi(parsed_inp.param1) / arr[source2.x][source2.y].val;
                }
            }
        }
        else
        {
            coordinate source1 = convert_to_index(parsed_inp.param1);

            if (strcmp(parsed_inp.type2, "val") == 0)
            {
                coordinate source1 = convert_to_index(parsed_inp.param1);
                if (strcmp(parsed_inp.cmd, "add") == 0)
                {
                    tgt->val = atoi(parsed_inp.param2) + arr[source1.x][source1.y].val;
                }
                else if (strcmp(parsed_inp.cmd, "sub") == 0)
                {
                    tgt->val = atoi(parsed_inp.param2) - arr[source1.x][source1.y].val;
                }
                else if (strcmp(parsed_inp.cmd, "mul") == 0)
                {
                    tgt->val = atoi(parsed_inp.param2) * arr[source1.x][source1.y].val;
                }
                else
                {
                    tgt->val = atoi(parsed_inp.param2) / arr[source1.x][source1.y].val;
                }
            }
            else
            {
                coordinate source1 = convert_to_index(parsed_inp.param1);
                coordinate source2 = convert_to_index(parsed_inp.param2);
                if (strcmp(parsed_inp.cmd, "add") == 0)
                {
                    tgt->val = addition(source1.x, source1.y, source2.x, source2.y, row, col, arr);
                }
                else if (strcmp(parsed_inp.cmd, "sub") == 0)
                {
                    tgt->val = subtraction(source1.x, source2.y, source2.x, source2.y, row, col, arr);
                }
                else if (strcmp(parsed_inp.cmd, "mul") == 0)
                {
                    tgt->val = multiply(source1.x, source2.y, source2.x, source2.y, row, col, arr);
                }
            }
        }
    }
}