#include "../../apue.3e/apue.h"
#include <pthread.h>

//线程1
void *
thr_fn1(void *arg)
{
    printf("thread 1 returning\n");
    return((void *)1); //thread 1 return 程序
}


void *
thr_fn2(void *arg)
{
    printf("thread2 exiting\n");
    pthread_exit((void *)2); //thread 2 exit 程序
}

int
main(void)
{
    int err;
    pthread_t tid1,tid2;
    void *tret; //可接收任意类型数据
    err = pthread_create(&tid1,NULL,thr_fn1,NULL);
    if(err!=0)
        err_exit(err,"can't create thread 1"); 

    err  = pthread_create(&tid2,NULL,thr_fn2,NULL);
    if(err != 0)
        err_exit(err,"can't create thread 2");
    
    err = pthread_join(tid1,&tret);
    if (err !=0)
        err_exit(err,"can't join with thread 1");

    printf("thread 1 exit code %ld\n",(long)tret);
    err = pthread_join(tid2,&tret);

    if(err != 0)
        err_exit(err, "can't join with thread 2");

    printf("thread 2 exit code %ld\n",(long)tret);
    exit(0);
}


