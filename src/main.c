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
            cell new_cell={i*j,new_command,create_hashset()};
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
        start = clock();
        if(strcmp(parsed_inp.type,"cmd")==0){
            // scroll_to not working
            if(strcmp(parsed_inp.cmd,"scroll_to")==0){
                coordinate c=convert_to_index(parsed_inp.param1);
                printf("%d %d\n",c.x,c.y);
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

        }
        else if(strcmp(parsed_inp.type,"art")==0){
            
        }
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