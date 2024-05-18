#include "../../apue.3e/apue.h"
#include <pthread.h>

pthread_t  ntid;//存放新线程id

void
printids(const char *s)
{
    pid_t pid;
    pthread_t tid;

    pid = getpid(); // 获取进程id
    tid = pthread_self(); //获取线程id
    printf("%s  pid %lu tid %lu (0x%lx) \n",s,(unsigned long) pid,(unsigned long)tid,(unsigned long)tid);
    //%lu是格式化指定符号，用于在输入输出函数（printf）中表示long unsigned int类型的数据，即无符号长整数，10进制输出
    //%lx表示输出一个无符号长整数，为小写16进制。常用于调试和日志输出中展示内存地址或特定数值的16进制表示。

}

void *
thr_fn(void *arg)
{
    printids("new thread: ");
    return ((void *)0);
}


int main(void)
{
    int err;
    err = pthread_create(&ntid,NULL,thr_fn,NULL);
    if(err != 0)
        err_exit(err,"can't create thread");

    printids("main thread:");
    sleep(1);
    exit(0);        
}