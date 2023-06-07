#include"lib.h"
#include<string.h>

int string_to_number(char *string,int size){
    int sum=0;
    for(int i=0;i<size;i++){
        if('0'<=string[i] && '9'>=string[i]){
            sum=sum*10+(int)(string[i]-'0');
        }
        else break;
    }
    return sum;
}