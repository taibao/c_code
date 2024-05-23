#include "../../apue.3e/apue.h"
#include <pthread.h>
#include <stdlib.h>

struct foo{
    int f_count;
    pthread_mutex_t f_lock;
    int f_id;
};

struct foo *
foo_alloc(int id) //allocate the object
{
    struct foo *fp;
    if ((fp = malloc(sizeof(struct foo))) != NULL){
        fp->f_count = 1;
        fp->f_id = id;
        if(pthread_mutex_init(&fp->f_lock,NULL) !=0){
            free(fp);
            return (NULL);
        }
    }
    return(fp);
}

void
foo_hold(struct foo *fp){
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
}


//释放对象引用
void
foo_rele(struct foo *fp)
{
    pthread_mutex_lock(&fp->f_lock);
    if(--fp->f_count == 0){
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    }else{
        pthread_mutex_unlock(&fp->f_lock);
    }
}


int main() {
    struct foo *fp = foo_alloc(1);
    if (fp != NULL) {
        foo_hold(fp);
        foo_hold(fp);
        printf("Held, count should be 3\n");
        foo_rele(fp);
        foo_rele(fp);
        foo_rele(fp); // This should free the structure since count will reach 0
    } else {
        fprintf(stderr, "Failed to allocate foo\n");
    }
    return 0;
}










