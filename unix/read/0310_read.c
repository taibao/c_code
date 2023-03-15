#include "apue.h"
#define BUFFSIZE 4096

// ./a.out > filename 
//将用户输入写入到文件

//./a.out < 1.txt > 2.txt
//将1.txt 的内容写入到2.txt
int main(void){
    int n;
    char buf[BUFFSIZE];

    while((n = read(STDIN_FILENO,buf,BUFFSIZE)) > 0)
        if (write(STDOUT_FILENO,buf,n)!=n)
            err_sys("write error");
    if (n<0)
        err_sys("read error");
    exit(0);
}
