#include "apue.h"
int main(void)
{!
    //获取命令行输入
    while((c=getc(stdin)) != EOF)
        if(putc(c,stdout) == EOF)
            err_sys("output error");

    if (ferror(stdin))
        err_sys("input error");
    //退出
    exit(0);
}

