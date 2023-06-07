#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<fcntl.h>

#define ERR_EXIT(m) \
    do{ \
        perror(m); \
        exit(EXIT_FAILURE); \
    }while(0)

#define PERM S_IRUSR|S_IWUSR|IPC_CREAT

#define MAX_SIZE 1000

int main(int argc,char* argv[]){
    struct stat fileattr;
    char pathname[80],cmd_str[80];
    int ret,fd;

    if(argc<2){
        printf("Usage: ./a.out pathname\n");
        return EXIT_FAILURE;
    }
    strcpy(pathname,argv[1]);

    if(stat(pathname,&fileattr)==-1){
        ret=creat(pathname,O_RDWR);
        if(ret==-1){
            ERR_EXIT("create()");
        }
        printf("random file object created.\n");
    }

    fd=open(pathname,O_RDWR,0666);

    for(int i=0;i<MAX_SIZE;i++){
        int num=rand()%1000;
        char string[1000];
        sprintf(string,"%d\n",num);
        printf("%s",string);
        if(write(ret,string,strlen(string))==-1){
            ERR_EXIT("write()");
        }
    }

    return 0;
}