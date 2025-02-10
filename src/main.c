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
    scanf("%s",str);
    while(strcmp(str,"q")!=0){
        commandCall a;
        a=parse(str);
        char * ins1=a.type;
        if (strcmp(ins1,"val")==0){
            
        }
        
        scanf("%s",str);
    }
    return 0;
}