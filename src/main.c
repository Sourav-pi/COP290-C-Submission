#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../Include/module.h"
#include "../Include/hash_set.h"

#define debug 0

#define MAXROWS 999
#define MAXCOLS 18278

int rowmax = 0;
int colmax = 0;

int main(int argc, char *argv[])
{
    // if (debug)
        // printf("%lu\n", sizeof(cell));
    // if (debug)
        // printf("%lu\n", sizeof(int));
    // if (debug)
        // printf("%lu\n", sizeof(HashSet));
    // if (debug)
        // printf("%lu\n", sizeof(commandCall));
        // printf("%lu\n", sizeof(Node));

    coordinate disp_c = {0, 0};
    int is_disp = 1;
    if(argc!=3)
    {
        printf("Invalid number of arguments\n");
        return 0;
    }
    int row = atoi(argv[1]), col = atoi(argv[2]);
    if(row<=0 || col<=0 || row>MAXROWS || col>MAXCOLS)
    {
        printf("Number of rows or columns out of range\n");
        return 0;
    }
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
    commandContainer command;
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

        command = parse(raw_inp, isSigned(raw_inp));
        parsed_inp = command.cmd;

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
        if (parsed_inp.type!=CMD)
        {
            tar_cod = decode_cell(command.target);
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
                        remove_string(temp->dep, command.target);
                    }
                }
            }
            else
            {
                if (old.type1 == CELL)
                {
                    old_cod_1 = decode_cell(old.param1);
                    old_cell_1 = &arr[old_cod_1.x][old_cod_1.y];
                    remove_string(old_cell_1->dep, command.target);
                }
                if (old.type2 == CELL)
                {
                    old_cod_2 = decode_cell(old.param2);
                    old_cell_2 = &arr[old_cod_2.x][old_cod_2.y];
                    remove_string(old_cell_2->dep, command.target);
                }
            }

            if (parsed_inp.error==INVALID)
            {
                strcpy(status, "invalid");
                // printf("error: %s\n", parsed_inp.error);
                continue;
            }
            else if (parsed_inp.error==CYCLE ) {
                strcpy(status, "circular dependency");
                // printf("error: %s\n", parsed_inp.error);
                continue;
            }
        }
        // save old end

        start = clock();
        if (parsed_inp.type==CMD)
        {
            // scroll_to not working
            if (parsed_inp.cmd==SCROLL_TO)
            {
                coordinate c = decode_cell(parsed_inp.param1);
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
            else if (parsed_inp.cmd==DISABLE_OUTPUT)
            {
                is_disp = 0;
            }
            else if (parsed_inp.cmd==ENABLE_OUTPUT)
            {
                is_disp = 1;
            }
            else if (parsed_inp.cmd==W)
            {
                disp_c.x = max(0, disp_c.x - 10);
            }
            else if (parsed_inp.cmd==D)
            {
                disp_c.y = min(col - 1, disp_c.y + 10);
            }
            else if (parsed_inp.cmd==A)
            {
                disp_c.y = max(0, disp_c.y - 10);
            }
            else if (parsed_inp.cmd==S)
            {
                disp_c.x = min(row - 1, disp_c.x + 10);
            }
            // Ensure disp_c.x and disp_c.y are within valid ranges
            disp_c.x = max(0, min(row - 1, disp_c.x));
            disp_c.y = max(0, min(col - 1, disp_c.y));
            end = clock();
            time = ((double)(end - start)) / CLOCKS_PER_SEC;
            continue;
        }
        else if (parsed_inp.type== FUNC)
        {
            coordinate c = decode_cell(command.target);
            cell *tgt = &arr[c.x][c.y];
            tgt->cmd = parsed_inp;
            if (parsed_inp.cmd==SLEEP)
            {
                if (parsed_inp.type1==VAL)
                {
                    sleep_time = max(0, parsed_inp.param1);
                    sleep(sleep_time);
                    tgt->val =  (parsed_inp.param1);
                    tgt->dep = create_hashset();
                }
                else
                {
                    coordinate source1 = decode_cell(parsed_inp.param1);
                    sleep_time = max(arr[source1.x][source1.y].val, 0);
                    sleep(sleep_time);
                    coordinate x1 = decode_cell(parsed_inp.param1);
                    cell *temp = &arr[x1.x][x1.y];
                    insert(temp->dep, command.target);
                }
            }
            else if (parsed_inp.cmd==MAX)
            {
                coordinate x1 = decode_cell(parsed_inp.param1);
                coordinate x2 = decode_cell(parsed_inp.param2);
                for (int i = x1.x; i <= x2.x; i++)
                {
                    for (int j = x1.y; j <= x2.y; j++)
                    {
                        cell *temp = &arr[i][j];
                        insert(temp->dep, command.target);
                    }
                }
            }
            else if (parsed_inp.cmd==MIN)
            {
                coordinate x1 = decode_cell(parsed_inp.param1);
                coordinate x2 = decode_cell(parsed_inp.param2);
                // tgt->val=minimumrange(x1.x,x1.y,x2.x,x2.y,row,col,arr);
                for (int i = x1.x; i <= x2.x; i++)
                {
                    for (int j = x1.y; j <= x2.y; j++)
                    {
                        cell *temp = &arr[i][j];
                        insert(temp->dep, command.target);
                    }
                }
            }
            else if (parsed_inp.cmd==SUM)
            {
                coordinate x1 = decode_cell(parsed_inp.param1);
                coordinate x2 = decode_cell(parsed_inp.param2);
                // tgt->val=(additionrange(x1.x,x1.y,x2.x,x2.y,row,col,arr));
                for (int i = x1.x; i <= x2.x; i++)
                {
                    for (int j = x1.y; j <= x2.y; j++)
                    {
                        cell *temp = &arr[i][j];
                        insert(temp->dep, command.target);
                    }
                }
            }
            else if (parsed_inp.cmd==AVG)
            {
                coordinate x1 = decode_cell(parsed_inp.param1);
                coordinate x2 = decode_cell(parsed_inp.param2);
                // tgt->val=(additionrange(x1.x,x1.y,x2.x,x2.y,row,col,arr))/2;
                for (int i = x1.x; i <= x2.x; i++)
                {
                    for (int j = x1.y; j <= x2.y; j++)
                    {
                        cell *temp = &arr[i][j];
                        insert(temp->dep, command.target);
                    }
                }
            }
            else if (parsed_inp.cmd == STDEV)
            {
                coordinate x1 = decode_cell(parsed_inp.param1);
                coordinate x2 = decode_cell(parsed_inp.param2);
                // tgt->val=(additionrange(x1.x,x1.y,x2.x,x2.y,row,col,arr))/2;
                for (int i = x1.x; i <= x2.x; i++)
                {
                    for (int j = x1.y; j <= x2.y; j++)
                    {
                        cell *temp = &arr[i][j];
                        insert(temp->dep, command.target);
                    }
                }
            }
        }
        else if (parsed_inp.type == VAL)
        {
            coordinate c = decode_cell(command.target);
            cell *tgt = &arr[c.x][c.y];
            tgt->cmd = parsed_inp;
            if (parsed_inp.type1 == VAL)
            {
                // tgt->val =  (parsed_inp.param1);
                // printf("vaLLl: ÷%d\n",tgt->val);
            }
            else
            {
                coordinate source1 = decode_cell(parsed_inp.param1);
                // if (arr[source1.x][source1.y].cmd.isDivByZero)
                // {
                //     tgt->cmd.isDivByZero = 1;
                //     tgt->val = 0;
                //     continue;
                // }
                // else
                // {
                //     tgt->val = arr[source1.x][source1.y].val;
                //     tgt->cmd.isDivByZero = 0;
                // }
                cell source1_cell = arr[source1.x][source1.y];
                insert(source1_cell.dep, command.target);
                // iterate_hashset(source1_cell.dep,print_element);
            }
        }
        else if (parsed_inp.type == ART)
        {
            coordinate c = decode_cell(command.target);
            cell *tgt = &arr[c.x][c.y];
            tgt->cmd = parsed_inp;
            if (parsed_inp.type1== VAL)
            {
                if (parsed_inp.type2== VAL)
                {
                    if (parsed_inp.cmd == ADD)
                    {
                        // tgt->val = (parsed_inp.param1) + (parsed_inp.param2);
                    }
                    else if (parsed_inp.cmd==SUB)
                    {
                        // tgt->val = (parsed_inp.param1) - (parsed_inp.param2);
                    }
                    else if (parsed_inp.cmd== MUL)
                    {
                        // tgt->val = (parsed_inp.param1) * (parsed_inp.param2);
                    }
                    else if (parsed_inp.cmd == DIV)
                    {
                        // if ((parsed_inp.param2) == 0)
                        // {
                        //     tgt->cmd.isDivByZero = 1;
                        //     tgt->val = 0;
                        //     // continue;
                        // }
                        // else
                        // {
                        //     tgt->val =  (parsed_inp.param1) /  (parsed_inp.param2);
                        //     tgt->cmd.isDivByZero = 0;
                        // }
                    }
                    // else
                    // {
                    //     tgt->val =  (parsed_inp.param1) /  (parsed_inp.param2);
                    // }
                }
                else
                {
                    coordinate source2 = decode_cell(parsed_inp.param2);
                    cell source2_cell = arr[source2.x][source2.y];
                    insert(source2_cell.dep, command.target);
                    // if (source2_cell.cmd.isDivByZero)
                    // {
                    //     tgt->cmd.isDivByZero = 1;
                    //     tgt->val = 0;
                    //     continue;
                    // }
                    // else
                    // {
                    //     // tgt->cmd.isDivByZero = 0;

                    //     if (parsed_inp.cmd == ADD)
                    //     {
                    //         tgt->val =  (parsed_inp.param1) + arr[source2.x][source2.y].val;
                    //     }
                    //     else if (parsed_inp.cmd== SUB)
                    //     {
                    //         tgt->val =  (parsed_inp.param1) - arr[source2.x][source2.y].val;
                    //     }
                    //     else if (parsed_inp.cmd== MUL)
                    //     {
                    //         tgt->val =  (parsed_inp.param1) * arr[source2.x][source2.y].val;
                    //     }
                    //     else if (parsed_inp.cmd== DIV)
                    //     {
                    //         if (arr[source2.x][source2.y].val == 0)
                    //         {
                    //             tgt->cmd.isDivByZero = 1;
                    //             tgt->val = 0;
                    //             continue;
                    //         }
                    //         else
                    //         {
                    //             tgt->cmd.isDivByZero=0;
                    //             tgt->val =  (parsed_inp.param1) / arr[source2.x][source2.y].val;
                    //         }
                    //     }
                    // }
                    // else
                    // {
                    //     tgt->val =  (parsed_inp.param1) / arr[source2.x][source2.y].val;
                    // }
                }
            }
            else
            {
                coordinate source1 = decode_cell(parsed_inp.param1);
                cell source1_cell = arr[source1.x][source1.y];
                insert(source1_cell.dep, command.target);

                if (parsed_inp.type2== VAL)
                 {
                //     if (source1_cell.cmd.isDivByZero)
                //     {
                //         tgt->cmd.isDivByZero = 1;
                //         tgt->val = 0;
                //         continue;
                //     }
                //     else
                //     {
                //         tgt->cmd.isDivByZero = 0;

                //         if (parsed_inp.cmd == ADD)
                //         {
                //             tgt->val =  (parsed_inp.param2) + arr[source1.x][source1.y].val;
                //         }
                //         else if (parsed_inp.cmd == SUB)
                //         {
                //             tgt->val = arr[source1.x][source1.y].val -  (parsed_inp.param2);
                //         }
                //         else if (parsed_inp.cmd == MUL)
                //         {
                //             tgt->val =  (parsed_inp.param2) * arr[source1.x][source1.y].val;
                //         }
                //         else if (parsed_inp.cmd== DIV)
                //         {
                //             if ( (parsed_inp.param2) == 0)
                //             {
                //                 tgt->cmd.isDivByZero = 1;
                //                 tgt->val = 0;
                //                 continue;
                //             }
                //             else
                //             {
                //                 tgt->cmd.isDivByZero = 0;
                //                 tgt->val = arr[source1.x][source1.y].val /  (parsed_inp.param2);
                //             }
                //         }
                //     }
                //     // else
                //     // {
                //     //     tgt->val =  (parsed_inp.param2) / arr[source1.x][source1.y].val;
                //     // }
                 }
                else
                {
                    coordinate source1 = decode_cell(parsed_inp.param1);
                    coordinate source2 = decode_cell(parsed_inp.param2);
                    cell source2_cell = arr[source2.x][source2.y];
                    insert(source2_cell.dep, command.target);
                    // if (source1_cell.cmd.isDivByZero || source2_cell.cmd.isDivByZero)
                    // {
                    //     tgt->cmd.isDivByZero = 1;
                    //     tgt->val = 0;
                    //     continue;
                    // }
                    // else
                    // {
                    //     tgt->cmd.isDivByZero = 0;
                    //     if ( parsed_inp.cmd == ADD)
                    //     {
                    //         tgt->val = addition(source1.x, source1.y, source2.x, source2.y, row, col, arr);
                    //     }
                    //     else if (parsed_inp.cmd == SUB)
                    //     {
                    //         tgt->val = subtraction(source1.x, source2.y, source2.x, source2.y, row, col, arr);
                    //     }
                    //     else if (parsed_inp.cmd== MUL)
                    //     {
                    //         tgt->val = multiply(source1.x, source2.y, source2.x, source2.y, row, col, arr);
                    //     }
                    //     else if (parsed_inp.cmd== DIV)
                    //     {
                    //         if (arr[source2.x][source2.y].val == 0)
                    //         {
                    //             tgt->cmd.isDivByZero = 1;
                    //             tgt->val = 0;
                    //             continue;
                    //         }
                    //         else
                    //         {
                    //             tgt->cmd.isDivByZero = 0;
                    //             tgt->val =  (parsed_inp.param1) / arr[source2.x][source2.y].val;
                    //         }
                    //     }
                    // }
                    // else
                    // {
                    //     tgt->val =  (parsed_inp.param1) / arr[source2.x][source2.y].val;
                    // }
                }
            }
        }

        // printf("parsed_inp:\n");
        if (parsed_inp.type != CMD)
        {

            
            Node* sorted_cell_names = topological_sort(command.target, arr, row, col);
            Node* temp = sorted_cell_names;
            if (sorted_cell_names == NULL)
            {
                strcpy(status, "Circular dependency");
                if (parsed_inp.type == FUNC)
                {
                    // printf("XX2/n");
                    coordinate x1 = decode_cell(parsed_inp.param1);
                    coordinate x2 = decode_cell(parsed_inp.param2);
                    for (int i = x1.x; i <= x2.x; i++)
                    {
                        for (int j = x1.y; j <= x2.y; j++)
                        {
                            // printf("i:%d,j:%d\n",i,j);
                            cell *temp = &arr[i][j];
                            remove_string(temp->dep, command.target);
                        }
                    }
                }
                else
                {
                    if (parsed_inp.type1 == CELL)
                    {
                        old_cod_1 = decode_cell(parsed_inp.param1);
                        old_cell_1 = &arr[old_cod_1.x][old_cod_1.y];
                        remove_string(old_cell_1->dep, command.target);
                    }
                    if ( old.type2== CELL)
                    {
                        old_cod_2 = decode_cell(parsed_inp.param2);
                        old_cell_2 = &arr[old_cod_2.x][old_cod_2.y];
                        remove_string(old_cell_2->dep, command.target);
                    }
                }
                // if (debug)
                //     printf("Circular dependency\n");
                tar_cell->cmd = old;
                tar_cell->val = old_val;
                if (old.type == 2)
                {
                    coordinate x1 = decode_cell(old.param1);
                    coordinate x2 = decode_cell(old.param2);
                    for (int i = x1.x; i <= x2.x; i++)
                    {
                        for (int j = x1.y; j <= x2.y; j++)
                        {
                            cell *temp = &arr[i][j];
                            insert(temp->dep, command.target);
                        }
                    }
                }
                else
                {
                    if (old.type1 == 1)
                    {
                        coordinate old_cod_1 = decode_cell(old.param1);
                        cell *old_cell_1 = &arr[old_cod_1.x][old_cod_1.y];
                        insert(old_cell_1->dep, command.target);
                    }
                    if (old.type2== 1)
                    {
                        coordinate old_cod_2 = decode_cell(old.param2);
                        cell *old_cell_2 = &arr[old_cod_2.x][old_cod_2.y];
                        insert(old_cell_2->dep, command.target);
                    }
                }
            }
            else
            {
                while(sorted_cell_names!= NULL)
                {
                    // printf("sorted_cell_names:%d\n", sorted_cell_names->value);
                    coordinate c = decode_cell(sorted_cell_names->value);
                    cell *cell_ptr = &arr[c.x][c.y];
                    // if (debug)
                    //     printf("%s ", sorted_cell_names[i]);
                    update(cell_ptr, arr, row, col);
                    Node* temp = sorted_cell_names;
                    sorted_cell_names = sorted_cell_names->next;

                    
                    free(temp);
                    
                }
                // if (debug)
                //     printf("\n");
                free(sorted_cell_names);
            }
        }
        end = clock();
        time = (((double)(end - start)) / CLOCKS_PER_SEC) + sleep_time;
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