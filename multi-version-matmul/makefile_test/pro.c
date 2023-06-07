#include<stdio.h>
#include<malloc.h>
#include"lib.h"

int main(){
    char* c;
    int len;
    //input size
    printf("=== Welcome to Wor test ===\n");
    printf("please input string size: ");
    scanf("%d",&len);
    c=(char*)malloc(sizeof(char)*len);
    printf("please input string: ");
    scanf("%s",c);
    printf("the string is: %s\n",c);
    printf("number = %d\n",string_to_number(c,len));
    return 0;
}