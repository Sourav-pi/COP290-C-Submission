#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../Include/module.h"
#include "../Include/hash_set.h"
#include <sys/resource.h>

#define debug 0

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
#define  scroll_to 0;
#define  disable_output 1;
#define  enable_output 2;
#define  W 3;
#define  D 4;
#define  A 5;
#define  S 6;

int rowmax = 0;
int colmax = 0;

void print_memory_usage()
{
    // if (debug)
    // {

    //     struct rusage usage;
    //     getrusage(RUSAGE_SELF, &usage);
    //     printf("Max memory used: %ld MB\n", usage.ru_maxrss / (1024 * 1024));
    // }
}

int main(int argc, char *argv[])
{
    // if (debug)
    //     printf("%lu\n", sizeof(cell));
    // if (debug)
    //     printf("%lu\n", sizeof(int));
    // if (debug)
    //     printf("%lu\n", sizeof(HashSet));
    // if (debug)
    //     printf("%lu\n", sizeof(commandCall));

    coordinate disp_c = {0, 0};
    int is_disp = 1;
    int row = atoi(argv[1]), col = atoi(argv[2]);
    rowmax = row;
    colmax = col;
    cell **arr = (cell **)malloc(row * sizeof(cell *));
    for (int i = 0; i < row; i++)
    {
        arr[i] = (cell *)malloc(col * sizeof(cell));
    }

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            commandCall new_command = {
                0,
            };
            cell new_cell = {0, new_command, create_hashset()};
            arr[i][j] = new_cell;
        }
    }

    char raw_inp[32] = "";
    clock_t start, end;
    double time = 0.0;
    char status[20] = "ok";
    commandCall parsed_inp;
    coordinate tar_cod;
    cell *tar_cell;
    commandCall old;
    coordinate old_cod_1;
    cell *old_cell_1;
    coordinate old_cod_2;
    cell *old_cell_2;
    int old_val;
    float sleep_time = 0.0;

    while (strcmp(raw_inp, "q") != 0)
    {
        if (is_disp != 0)
        {
            Display(row, col, arr, disp_c.x, disp_c.y);
        }
        printf("[%.2f] (%s) > ", time, status);
        fgets(raw_inp, sizeof(raw_inp), stdin);
        raw_inp[strlen(raw_inp) - 1] = '\0';
        strcpy(status, "ok");
        sleep_time = 0.0;

        parsed_inp = parse(raw_inp, isSigned(raw_inp));
        // printf("error: %s\n",parsed_inp.error);
        // debug
        // if (debug)
        //     printf("parsed_inp:\n");
        // if (debug)
        //     printf("cmd: %s\n", parsed_inp.cmd);
        // if (debug)
        //     printf("type: %s\n", parsed_inp.type);
        // if (debug)
        //     printf("type1: %s\n", parsed_inp.type1);
        // if (debug)
        //     printf("type2: %s\n", parsed_inp.type2);
        // if (debug)
        //     printf("param1: %s\n", parsed_inp.param1);
        // if (debug)
        //     printf("param2: %s\n", parsed_inp.param2);
        // if (debug)
        //     printf("target: %s\n", parsed_inp.target);
        // if (debug)
        //     printf("error: %s\n", parsed_inp.error);

        if (parsed_inp.error==1)
        {
            strcpy(status, "Invalid");
            // if (debug)
            //     ("error: %s\n", parsed_inp.error);
            continue;
        }
        // save old start
        if (parsed_inp.type!=3)
        {
            tar_cod = decode_cell(parsed_inp.target);
            tar_cell = &arr[tar_cod.x][tar_cod.y];

            old = tar_cell->cmd;
            old_val = tar_cell->val;
            if (old.type == 2)
            {
                coordinate x1 = decode_cell(old.param1);
                coordinate x2 = decode_cell(old.param2);
                for (int i = x1.x; i <= x2.x; i++)
                {
                    for (int j = x1.y; j <= x2.y; j++)
                    {
                        cell *temp = &arr[i][j];
                        remove_string(temp->dep, old.target);
                    }
                }
            }
            else
            {
                if (old.type1 == 1)
                {
                    old_cod_1 = convert_to_index(old.param1);
                    old_cell_1 = &arr[old_cod_1.x][old_cod_1.y];
                    remove_string(old_cell_1->dep, old.target);
                }
                if (old.type2 == 1)
                {
                    old_cod_2 = convert_to_index(old.param2);
                    old_cell_2 = &arr[old_cod_2.x][old_cod_2.y];
                    remove_string(old_cell_2->dep, old.target);
                }
            }

            if (parsed_inp.error==1)
            {
                strcpy(status, "invalid");
                // printf("error: %s\n", parsed_inp.error);
                continue;
            }
            else if (parsed_inp.error==2) {
                strcpy(status, "circular dependency");
                // printf("error: %s\n", parsed_inp.error);
                continue;
            }
        }
        // save old end

        start = clock();
        if (parsed_inp.type==3)
        {
            // scroll_to not working
            if (parsed_inp.cmd==0)
            {
                coordinate c = convert_to_index(parsed_inp.param1);
                // printf("%d %d\n",c.x,c.y);
                if (c.x < row && c.y < col)
                {
                    disp_c.x = c.x;
                    disp_c.y = c.y;
                }
                else
                {
                    strcpy(status, "Invalid cell");
                }
            }
            else if (parsed_inp.cmd==1)
            {
                is_disp = 0;
            }
            else if (parsed_inp.cmd==2)
            {
                is_disp = 1;
            }
            else if (parsed_inp.cmd==3)
            {
                disp_c.x = max(0, disp_c.x - 10);
            }
            else if (parsed_inp.cmd==4)
            {
                disp_c.y = min(col - 1, disp_c.y + 10);
            }
            else if (parsed_inp.cmd==5)
            {
                disp_c.y = max(0, disp_c.y - 10);
            }
            else if (parsed_inp.cmd==6)
            {
                disp_c.x = min(row - 1, disp_c.x + 10);
            }
            // Ensure disp_c.x and disp_c.y are within valid ranges
            disp_c.x = max(0, min(row - 1, disp_c.x));
            disp_c.y = max(0, min(col - 1, disp_c.y));
            end = clock();
            time = ((double)(end - start)) / CLOCKS_PER_SEC;
            print_memory_usage();
            continue;
        }
        else if (parsed_inp.type== 2)
        {
            coordinate c = convert_to_index(parsed_inp.target);
            cell *tgt = &arr[c.x][c.y];
            tgt->cmd = parsed_inp;
            if (parsed_inp.cmd==5)
            {
                if (parsed_inp.type1==0)
                {
                    sleep_time = max(0, atoi(parsed_inp.param1));
                    sleep(sleep_time);
                    tgt->val = atoi(parsed_inp.param1);
                    tgt->dep = create_hashset();
                }
                else
                {
                    coordinate source1 = convert_to_index(parsed_inp.param1);
                    sleep_time = max(arr[source1.x][source1.y].val, 0);
                    sleep(sleep_time);
                    coordinate x1 = convert_to_index(parsed_inp.param1);
                    cell *temp = &arr[x1.x][x1.y];
                    insert(temp->dep, parsed_inp.target);
                }
            }
            else if (parsed_inp.cmd==1)
            {
                coordinate x1 = convert_to_index(parsed_inp.param1);
                coordinate x2 = convert_to_index(parsed_inp.param2);
                for (int i = x1.x; i <= x2.x; i++)
                {
                    for (int j = x1.y; j <= x2.y; j++)
                    {
                        cell *temp = &arr[i][j];
                        insert(temp->dep, parsed_inp.target);
                    }
                }
            }
            else if (parsed_inp.cmd==0)
            {
                coordinate x1 = convert_to_index(parsed_inp.param1);
                coordinate x2 = convert_to_index(parsed_inp.param2);
                // tgt->val=minimumrange(x1.x,x1.y,x2.x,x2.y,row,col,arr);
                for (int i = x1.x; i <= x2.x; i++)
                {
                    for (int j = x1.y; j <= x2.y; j++)
                    {
                        cell *temp = &arr[i][j];
                        insert(temp->dep, parsed_inp.target);
                    }
                }
            }
            else if (parsed_inp.cmd==2)
            {
                coordinate x1 = convert_to_index(parsed_inp.param1);
                coordinate x2 = convert_to_index(parsed_inp.param2);
                // tgt->val=(additionrange(x1.x,x1.y,x2.x,x2.y,row,col,arr));
                for (int i = x1.x; i <= x2.x; i++)
                {
                    for (int j = x1.y; j <= x2.y; j++)
                    {
                        cell *temp = &arr[i][j];
                        insert(temp->dep, parsed_inp.target);
                    }
                }
            }
            else if (parsed_inp.cmd==3)
            {
                coordinate x1 = convert_to_index(parsed_inp.param1);
                coordinate x2 = convert_to_index(parsed_inp.param2);
                // tgt->val=(additionrange(x1.x,x1.y,x2.x,x2.y,row,col,arr))/2;
                for (int i = x1.x; i <= x2.x; i++)
                {
                    for (int j = x1.y; j <= x2.y; j++)
                    {
                        cell *temp = &arr[i][j];
                        insert(temp->dep, parsed_inp.target);
                    }
                }
            }
            else if (parsed_inp.cmd == 4)
            {
                coordinate x1 = convert_to_index(parsed_inp.param1);
                coordinate x2 = convert_to_index(parsed_inp.param2);
                // tgt->val=(additionrange(x1.x,x1.y,x2.x,x2.y,row,col,arr))/2;
                for (int i = x1.x; i <= x2.x; i++)
                {
                    for (int j = x1.y; j <= x2.y; j++)
                    {
                        cell *temp = &arr[i][j];
                        insert(temp->dep, parsed_inp.target);
                    }
                }
            }
        }
        else if (parsed_inp.type == 0)
        {
            coordinate c = convert_to_index(parsed_inp.target);
            cell *tgt = &arr[c.x][c.y];
            tgt->cmd = parsed_inp;
            if (parsed_inp.type1 == 0)
            {
                tgt->val = atoi(parsed_inp.param1);
                // printf("vaLLl: ÷%d\n",tgt->val);
            }
            else
            {
                coordinate source1 = convert_to_index(parsed_inp.param1);
                if (arr[source1.x][source1.y].isDivByZero)
                {
                    tgt->isDivByZero = 1;
                    tgt->val = 0;
                    continue;
                }
                else
                {
                    tgt->val = arr[source1.x][source1.y].val;
                    tgt->isDivByZero = 0;
                }
                cell source1_cell = arr[source1.x][source1.y];
                insert(source1_cell.dep, parsed_inp.target);
                // iterate_hashset(source1_cell.dep,print_element);
            }
        }
        else if (parsed_inp.type == 1)
        {
            coordinate c = convert_to_index(parsed_inp.target);
            cell *tgt = &arr[c.x][c.y];
            tgt->cmd = parsed_inp;
            if (parsed_inp.type1== 0)
            {
                if (parsed_inp.type2== 0)
                {
                    if (parsed_inp.cmd == 0)
                    {
                        tgt->val = atoi(parsed_inp.param1) + atoi(parsed_inp.param2);
                    }
                    else if (parsed_inp.cmd==1)
                    {
                        tgt->val = atoi(parsed_inp.param1) - atoi(parsed_inp.param2);
                    }
                    else if (parsed_inp.cmd== 2)
                    {
                        tgt->val = atoi(parsed_inp.param1) * atoi(parsed_inp.param2);
                    }
                    else if (parsed_inp.cmd == 3)
                    {
                        if (atoi(parsed_inp.param2) == 0)
                        {
                            tgt->isDivByZero = 1;
                            tgt->val = 0;
                            continue;
                        }
                        else
                        {
                            tgt->val = atoi(parsed_inp.param1) / atoi(parsed_inp.param2);
                            tgt->isDivByZero = 0;
                        }
                    }
                    // else
                    // {
                    //     tgt->val = atoi(parsed_inp.param1) / atoi(parsed_inp.param2);
                    // }
                }
                else
                {
                    coordinate source2 = convert_to_index(parsed_inp.param2);
                    cell source2_cell = arr[source2.x][source2.y];
                    insert(source2_cell.dep, parsed_inp.target);
                    if (source2_cell.isDivByZero)
                    {
                        tgt->isDivByZero = 1;
                        tgt->val = 0;
                        continue;
                    }
                    else
                    {
                        tgt->isDivByZero = 0;

                        if (parsed_inp.cmd == 0)
                        {
                            tgt->val = atoi(parsed_inp.param1) + arr[source2.x][source2.y].val;
                        }
                        else if (parsed_inp.cmd== 1)
                        {
                            tgt->val = atoi(parsed_inp.param1) - arr[source2.x][source2.y].val;
                        }
                        else if (parsed_inp.cmd== 2)
                        {
                            tgt->val = atoi(parsed_inp.param1) * arr[source2.x][source2.y].val;
                        }
                        else if (parsed_inp.cmd== 3)
                        {
                            if (arr[source2.x][source2.y].val == 0)
                            {
                                tgt->isDivByZero = 1;
                                tgt->val = 0;
                                continue;
                            }
                            else
                            {
                                tgt->isDivByZero=0;
                                tgt->val = atoi(parsed_inp.param1) / arr[source2.x][source2.y].val;
                            }
                        }
                    }
                    // else
                    // {
                    //     tgt->val = atoi(parsed_inp.param1) / arr[source2.x][source2.y].val;
                    // }
                }
            }
            else
            {
                coordinate source1 = convert_to_index(parsed_inp.param1);
                cell source1_cell = arr[source1.x][source1.y];
                insert(source1_cell.dep, parsed_inp.target);

                if (parsed_inp.type2== 0)
                {
                    if (source1_cell.isDivByZero)
                    {
                        tgt->isDivByZero = 1;
                        tgt->val = 0;
                        continue;
                    }
                    else
                    {
                        tgt->isDivByZero = 0;

                        if (parsed_inp.cmd == 0)
                        {
                            tgt->val = atoi(parsed_inp.param2) + arr[source1.x][source1.y].val;
                        }
                        else if (parsed_inp.cmd == 1)
                        {
                            tgt->val = arr[source1.x][source1.y].val - atoi(parsed_inp.param2);
                        }
                        else if (parsed_inp.cmd == 2)
                        {
                            tgt->val = atoi(parsed_inp.param2) * arr[source1.x][source1.y].val;
                        }
                        else if (parsed_inp.cmd== 3)
                        {
                            if (atoi(parsed_inp.param2) == 0)
                            {
                                tgt->isDivByZero = 1;
                                tgt->val = 0;
                                continue;
                            }
                            else
                            {
                                tgt->isDivByZero = 0;
                                tgt->val = arr[source1.x][source1.y].val / atoi(parsed_inp.param2);
                            }
                        }
                    }
                    // else
                    // {
                    //     tgt->val = atoi(parsed_inp.param2) / arr[source1.x][source1.y].val;
                    // }
                }
                else
                {
                    coordinate source1 = convert_to_index(parsed_inp.param1);
                    coordinate source2 = convert_to_index(parsed_inp.param2);
                    cell source2_cell = arr[source2.x][source2.y];
                    insert(source2_cell.dep, parsed_inp.target);
                    if (source1_cell.isDivByZero || source2_cell.isDivByZero)
                    {
                        tgt->isDivByZero = 1;
                        tgt->val = 0;
                        continue;
                    }
                    else
                    {
                        tgt->isDivByZero = 0;
                        if ( parsed_inp.cmd == 0)
                        {
                            tgt->val = addition(source1.x, source1.y, source2.x, source2.y, row, col, arr);
                        }
                        else if (parsed_inp.cmd == 1)
                        {
                            tgt->val = subtraction(source1.x, source2.y, source2.x, source2.y, row, col, arr);
                        }
                        else if (parsed_inp.cmd== 2)
                        {
                            tgt->val = multiply(source1.x, source2.y, source2.x, source2.y, row, col, arr);
                        }
                        else if (parsed_inp.cmd== 3)
                        {
                            if (arr[source2.x][source2.y].val == 0)
                            {
                                tgt->isDivByZero = 1;
                                tgt->val = 0;
                                continue;
                            }
                            else
                            {
                                tgt->isDivByZero = 0;
                                tgt->val = atoi(parsed_inp.param1) / arr[source2.x][source2.y].val;
                            }
                        }
                    }
                    // else
                    // {
                    //     tgt->val = atoi(parsed_inp.param1) / arr[source2.x][source2.y].val;
                    // }
                }
            }
        }

        // printf("parsed_inp:\n");
        if (parsed_inp.type != 3)
        {
            char **sorted_cell_names = topological_sort(parsed_inp.target, arr, row, col);
            if (sorted_cell_names == NULL)
            {
                strcpy(status, "Circular dependency");
                if (parsed_inp.type == 2)
                {
                    // printf("XX2/n");
                    coordinate x1 = convert_to_index(parsed_inp.param1);
                    coordinate x2 = convert_to_index(parsed_inp.param2);
                    for (int i = x1.x; i <= x2.x; i++)
                    {
                        for (int j = x1.y; j <= x2.y; j++)
                        {
                            // printf("i:%d,j:%d\n",i,j);
                            cell *temp = &arr[i][j];
                            remove_string(temp->dep, parsed_inp.target);
                        }
                    }
                }
                else
                {
                    if (parsed_inp.type1 == 1)
                    {
                        old_cod_1 = convert_to_index(parsed_inp.param1);
                        old_cell_1 = &arr[old_cod_1.x][old_cod_1.y];
                        remove_string(old_cell_1->dep, parsed_inp.target);
                    }
                    if ( old.type2== 1)
                    {
                        old_cod_2 = convert_to_index(parsed_inp.param2);
                        old_cell_2 = &arr[old_cod_2.x][old_cod_2.y];
                        remove_string(old_cell_2->dep, parsed_inp.target);
                    }
                }
                // if (debug)
                //     printf("Circular dependency\n");
                tar_cell->cmd = old;
                tar_cell->val = old_val;
                if (old.type == 2)
                {
                    coordinate x1 = convert_to_index(old.param1);
                    coordinate x2 = convert_to_index(old.param2);
                    for (int i = x1.x; i <= x2.x; i++)
                    {
                        for (int j = x1.y; j <= x2.y; j++)
                        {
                            cell *temp = &arr[i][j];
                            insert(temp->dep, old.target);
                        }
                    }
                }
                else
                {
                    if (old.type1 == 1)
                    {
                        coordinate old_cod_1 = convert_to_index(old.param1);
                        cell *old_cell_1 = &arr[old_cod_1.x][old_cod_1.y];
                        insert(old_cell_1->dep, old.target);
                    }
                    if (old.type2== 1)
                    {
                        coordinate old_cod_2 = convert_to_index(old.param2);
                        cell *old_cell_2 = &arr[old_cod_2.x][old_cod_2.y];
                        insert(old_cell_2->dep, old.target);
                    }
                }
            }
            else
            {
                for (int i = 0; sorted_cell_names[i] != NULL; i++)
                {
                    coordinate c = convert_to_index(sorted_cell_names[i]);
                    cell *cell_ptr = &arr[c.x][c.y];
                    // if (debug)
                    //     printf("%s ", sorted_cell_names[i]);
                    update(cell_ptr, arr, row, col);
                    free(sorted_cell_names[i]);
                }
                // if (debug)
                //     printf("\n");
                free(sorted_cell_names);
            }
        }
        end = clock();
        time = (((double)(end - start)) / CLOCKS_PER_SEC) + sleep_time;
        print_memory_usage();
    }
    // Freeing the memory
    for (int i = 0; i < row; i++)
    {
        free_hashset(arr[i]->dep);
        free(arr[i]);
    }
    free(arr);
    return 0;
}