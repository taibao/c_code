#include "../../apue.3e/apue.h"
#include <pthread.h>

struct foo{
    int a,b,c,d;
};

void 
printfoo(const char *s,const struct foo *fp)
{
    printf("%s",s);
    printf(" structure at 0x%lx\n",(unsigned long)fp); //输出结构体实例地址
    printf(" foo.a = %d\n",fp->a);
    printf(" foo.b = %d\n",fp->b);
    printf(" foo.c = %d\n",fp->c);
    printf(" foo.d = %d\n",fp->d);
}

void *
thr_fn1(void *arg)
{
    struct foo f1 = {1,2,3,4}; //结构体赋值
    printfoo("thread 1:\n",&f1);
    pthread_exit((void *)&f1); //创建线程返回结构体对象f1
}


void *
thr_fn2(void *arg)
{
    printf("thread 2:ID is %lu\n",(unsigned long)pthread_self());
    pthread_exit((void *)0);
}

int 
main(void)
{
    int err;
    pthread_t tid1,tid2;
    struct foo *fp;

    err = pthread_create(&tid1,NULL,thr_fn1,NULL);
     
    if (err !=0)
        err_exit(err,"can't create thread 1");

    err = pthread_join(tid1,(void *)&fp);    
    if(err != 0)
        err_exit(err,"can't join with thread 1");
    sleep(1);
    printf("parent starting second thread\n");
    err = pthread_create(&tid2,NULL, thr_fn2,NULL);
    if (err !=0)


        err_exit(err,"can't create thread 2");
    sleep(1);
    printfoo("parent:\n",fp);
    exit(0);
}




