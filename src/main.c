#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Include/module.h"
#include "../Include/hash_set.h"


int main(int argc, char *argv[])
{
    int row=atoi(argv[1]),col=atoi(argv[2]);
    cell arr[row][col];
    for (int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            cell new_cell={0,"","",""};
            arr[i][j]=new_cell;
        }
    }
    char str[100];
    while(str[0]!='q'){
        
    }
    return 0;
}