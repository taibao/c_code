#include "../../apue.3e/apue.h"
#include <pthread.h>

pthread_t  ntid;//存放新线程id
pthread_t  ntid2;//存放新线程id


//输出当前程序执行所在的进程和线程
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

//新建线程的回调函数
void *
thr_fn(void *arg)
{
    printids("new thread: ");
    return ((void *)0); //在c语言中表示一个空指针，指向地址0的指针，void *是一个通用指针类型，可以转换为任何其他对象的指针类型。意思等于null
    // return NULL;

}


//编译命令要记得带 -lpthread
//gcc create_thread.c -o create_thread -lpthread
int main(void)
{
    int err;
    err = pthread_create(&ntid,NULL,thr_fn,NULL);
    if(err != 0)
        err_exit(err,"can't create thread");

    err = pthread_create(&ntid2,NULL,thr_fn,NULL);
    if(err != 0)
        err_exit(err,"can't create thread2");

    printids("main thread:");
    sleep(1);
    exit(0);        
}