#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../Include/module.h"
#include "../Include/hash_set.h"


int main(int argc, char *argv[])
{
    coordinate disp_c = {0, 0};
    int is_disp = 1;
    int row=atoi(argv[1]),col=atoi(argv[2]);
    cell** arr = (cell**)malloc(row*sizeof(cell*));
    for (int i = 0; i < row; i++)
    {
        arr[i] = (cell*)malloc(col*sizeof(cell));
    }
    
    for (int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            commandCall new_command={0,};
            cell new_cell={0,new_command,create_hashset()};
            arr[i][j]=new_cell;
        }
    }

    char raw_inp[100]="";
    clock_t start, end;
    double time =0.0;
    char status[100]="ok";
    commandCall parsed_inp;
    while(strcmp(raw_inp,"q")!=0){
        if(is_disp && strcmp(parsed_inp.cmd,"enable_output")!=0){
            Display(row,col,arr,disp_c.x,disp_c.y);
        }
        printf("[%.2f] (%s) > ",time,status);
        scanf("%s",raw_inp);
        parsed_inp=parse(raw_inp);
        // printf("error: %s\n",parsed_inp.error);
        printf("parsed_inp:\n");
        printf("cmd: %s\n", parsed_inp.cmd);
        printf("type: %s\n", parsed_inp.type);
        printf("type1: %s\n", parsed_inp.type1);
        printf("type2: %s\n", parsed_inp.type2);
        printf("param1: %s\n", parsed_inp.param1);
        printf("param2: %s\n", parsed_inp.param2);
        printf("target: %s\n", parsed_inp.target);
        printf("error: %s\n", parsed_inp.error);
        if(strcmp(parsed_inp.error,"")!=0){
            strcpy(status,parsed_inp.error);
            printf("error: %s\n",parsed_inp.error);
            continue;
        }
        start = clock();
        if(strcmp(parsed_inp.type,"cmd")==0){
            // scroll_to not working
            if(strcmp(parsed_inp.cmd,"scroll_to")==0){
                coordinate c=convert_to_index(parsed_inp.param1);
                // printf("%d %d\n",c.x,c.y);
                if(c.x<row && c.y<col){
                    disp_c.x=c.x;
                    disp_c.y=c.y;
                }
                else{
                    strcpy(status,"Invalid cell");
                }
            }
            else if(strcmp(parsed_inp.cmd,"disable_output")==0){
                is_disp=0;
            }
            else if(strcmp(parsed_inp.cmd,"enable_output")==0){
                is_disp=1;
            }
            else if(strcmp(parsed_inp.cmd,"w")==0){
                disp_c.x=max(0,disp_c.x-10);
            }
            else if(strcmp(parsed_inp.cmd,"d")==0){
                disp_c.y=min(col-1,disp_c.y+10);
            }
            else if(strcmp(parsed_inp.cmd,"a")==0){
                disp_c.y=max(0,disp_c.y-10);
            }
            else if(strcmp(parsed_inp.cmd,"s")==0){
                disp_c.x=min(row-1,disp_c.x+10);
            }
        }
        else if(strcmp(parsed_inp.type,"func")==0){
            // if(strcmp(parsed_inp.cmd,"MAX")==0){
            //     int x1=convert_to_index(parsed_inp.param1).x;
            //     int y1=convert_to_index(parsed_inp.param1).y;
            //     int x2=convert_to_index(parsed_inp.param2).x;
            //     int y2=convert_to_index(parsed_inp.param2).y;
            //     if(x1<row && x2<row && y1<col && y2<col){
            //         printf("%d\n",maximum(x1,y1,x2,y2,row,col,arr));
            //     }
            //     else{
            //         printf("Invalid cell\n");
            //     }
            // }
            // else if(strcmp(parsed_inp.cmd,"MIN")==0){
            //     int x1=convert_to_index(parsed_inp.param1).x;
            //     int y1=convert_to_index(parsed_inp.param1).y;
            //     int x2=convert_to_index(parsed_inp.param2).x;
            //     int y2=convert_to_index(parsed_inp.param2).y;
            //     if(x1<row && x2<row && y1<col && y2<col){
            //         printf("%d\n",minimum(x1,y1,x2,y2,row,col,arr));
            //     }
            //     else{
            //         printf("Invalid cell\n");
            //     }
            // }
            // else if(strcmp(parsed_inp.cmd,"SUM")==0){
            //     int x1=convert_to_index(parsed_inp.param1).x;
            //     int y1=convert_to_index(parsed_inp.param1).y;
            //     int x2=convert_to_index(parsed_inp.param2).x;
            //     int y2=convert_to_index(parsed_inp.param2).y;
            //     if(x1<row && x2<row && y1<col && y2<col){
            //         printf("%d\n",addition(x1,y1,x2,y2,row,col,arr));
            //     }
            //     else{
            //         printf("Invalid cell\n");
            //     }
            // }
            // else if(strcmp(parsed_inp.cmd,"AVG")==0){
            //     int x1=convert_to_index(parsed_inp.param1).x;
            //     int y1=convert_to_index(parsed_inp.param1).y;
            //     int x2=convert_to_index(parsed_inp.param2).x;
            //     int y2=convert_to_index(parsed_inp.param2).y;
            //     if(x1<row && x2<row && y1<col && y2<col){
            //         printf("%d\n",(addition(x1,y1,x2,y2,row,col,arr))/(2));
            //     }
            //     else{
            //         printf("Invalid cell\n");
            //     }
            // }
            // else if(strcmp(parsed_inp.cmd,"STDEV")==0){
            //     int x1=convert_to_index(parsed_inp.param1).x;
            //     int y1=convert_to_index(parsed_inp.param1).y;
            //     int x2=convert_to_index(parsed_inp.param2).x;
            //     int y2=convert_to_index(parsed_inp.param2).y;
            //     if(x1<row && x2<row && y1<col && y2<col){
            //         printf("%d\n",(addition(x1,y1,x2,y2,row,col,arr))/(2));
            //     }
            //     else{
            //         printf("Invalid cell\n");
            //     }
            // }
            // else if(strcmp(parsed_inp.cmd,"SLEEP")==0){
            //     // time+=atof(parsed_inp.param1);
            //     sleep(atof(parsed_inp.param1));
                
            // }
        }
        else if(strcmp(parsed_inp.type,"val")==0){
            char *ins1=parsed_inp.type1;
            coordinate c= convert_to_index(parsed_inp.target);
            cell* tgt =&arr[c.x][c.y];
            tgt->cmd = parsed_inp;
            // printf("hello:");
            // printf("hello:%d,%d\n",c.x,c.y);
            if(strcmp(ins1 ,"val")==0){
                tgt->val=atoi(parsed_inp.param1);
                // printf("vaLLl: ÷%d\n",tgt->val);
            }else{
                coordinate source1=convert_to_index(parsed_inp.param1);
                tgt->val=arr[source1.x][source1.y].val;
                cell source1_cell=arr[source1.x][source1.y];
                insert(source1_cell.dep,parsed_inp.target);
                // iterate_hashset(source1_cell.dep,print_element);

                
            }
        }
        else if(strcmp(parsed_inp.type,"art")==0){
            coordinate c = convert_to_index(parsed_inp.target);
            cell* tgt = &arr[c.x][c.y];
            tgt->cmd = parsed_inp;
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
                    cell source2_cell = arr[source2.x][source2.y];
                    insert(source2_cell.dep, parsed_inp.target);
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
                cell source1_cell = arr[source1.x][source1.y];
                insert(source1_cell.dep, parsed_inp.target);

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
                    cell source2_cell = arr[source2.x][source2.y];
                    insert(source2_cell.dep, parsed_inp.target);
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


        // printf("parsed_inp:\n");
        char** sorted_cell_names = topological_sort(parsed_inp.target,arr,row,col);
        for (int i = 0; sorted_cell_names[i] != NULL; i++) {
            coordinate c=convert_to_index(sorted_cell_names[i]);
            cell* cell_ptr=&arr[c.x][c.y];
            printf("%s ",sorted_cell_names[i]);
            update(cell_ptr,arr,row,col);
            free(sorted_cell_names[i]);
        }
        printf("\n");
        free(sorted_cell_names);
        end = clock();
        time = ((double)(end - start)) / CLOCKS_PER_SEC;
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