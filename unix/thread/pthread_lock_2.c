#include "../../apue.3e/apue.h"
#include <pthread.h>
#include <stdlib.h>
 
#define NHASH 29 //哈希表的最大数量
#define HASH(id) (((unsigned long)id)%NHASH) //定义宏生成哈希值

struct foo *fh[NHASH];
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo{
    int f_count;
    pthread_mutex_t f_lock;
    int f_id ;
    struct foo *f_next; //protected by hashlock
};

struct foo *
foo_alloc(int id){
    struct foo *fp;
    int idx;
    if((fp=malloc(sizeof(struct foo))) != NULL){
        fp->f_count = 1;
        fp->f_id = id;
        //如果创建锁失败就释放变量
        if(pthread_mutex_init(&fp->f_lock,NULL) !=0){
            free(fp);
            return(NULL);
        }
        idx = HASH(id);
        pthread_mutex_lock(&hashlock);
        fp->f_next = fh[idx];
        fh[idx] = fp;
        pthread_mutex_lock(&fp->f_lock);
        pthread_mutex_unlock(&hashlock);
        pthread_mutex_unlock(&fp->f_lock);
    }
    return(fp);
}

void
foo_hold(struct foo *fp)
{
    pthread_mutex_lock(&hashlock);
    fp->f_count++;
    pthread_mutex_unlock(&hashlock);
}


struct foo *
foo_find(int id){
    struct foo *fp;
    pthread_mutex_lock(&hashlock);
    for(fp = fh[HASH(id)];fp!=NULL;fp =  fp->f_next){
        if(fp->f_id == id){
            fp->f_count++;//只用hashlock来锁住
            break;
        }
    }
    pthread_mutex_unlock(&hashlock);
    return(fp);
}

//每次释放一个引用计数
void
foo_rele(struct foo *fp){
    struct foo *tfp;
    int idx; //初始化为0
    fprintf(stderr,"输出当前的引用计数%d\n",fp->f_count);
    pthread_mutex_lock(&hashlock);
    //引用计数为1了就回收此fp资源
    if(--fp->f_count == 0){
        idx = HASH(fp->f_id);
        tfp = fh[idx];
        if(tfp == fp){
            fh[idx] = fp->f_next;
        }else{
            while(tfp->f_next!=fp)
                tfp =  tfp->f_next;
            tfp->f_next = fp->f_next;
        }

        pthread_mutex_unlock(&hashlock);
        pthread_mutex_destroy(&fp->f_lock);//回收fp中的锁
        free(fp);

    }else{
        pthread_mutex_unlock(&hashlock);
    }
}

int main(void)
{
    struct foo *fp;
    int id = 123; // 一个示例ID

    // 分配一个foo对象
    fp = foo_alloc(id);
    if (fp == NULL) {
        fprintf(stderr, "Failed to allocate foo object\n");
        return EXIT_FAILURE;
    }
    printf("Allocated foo with ID %d\n", fp->f_id);

    // 查找刚刚分配的foo对象
    fp = foo_find(id);
    if (fp != NULL) {
        printf("Found foo with ID %d\n", fp->f_id);
    } else {
        printf("Failed to find foo with ID %d\n", id);
        return EXIT_FAILURE;
    }

    // 增加引用计数
    foo_hold(fp);
    printf("Held foo with ID %d, count is now %d\n", fp->f_id, fp->f_count);

    // 释放foo对象
    foo_rele(fp);
    foo_rele(fp);
    foo_rele(fp);
    printf("Released foo with ID %d\n", id);

    // 可选：再次查找以确认对象已被正确释放（应该找不到）
    fp = foo_find(id);
    if (fp == NULL) {
        printf("Foo with ID %d correctly released\n", id);
    } else {
        printf("Warning: Foo with ID %d still exists!\n", id);
    }

    return EXIT_SUCCESS;
}







