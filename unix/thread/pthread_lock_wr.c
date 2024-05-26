#include "../../apue.3e/apue.h"
#include <pthread.h>
#include <stdlib.h>
 
//作业结构体
struct job{
    struct job *j_next;
    struct job *j_prev;
    pthread_t j_id; //标记那个线程持有该作业
};

struct queue{
    struct job *q_head;
    struct job *q_tail;
    pthread_rwlock_t q_lock;
};

int
queue_init(struct queue *qp){
    int err;
    qp->q_head = NULL;
    qp->q_tail = NULL;
    err = pthread_rwlock_init(&qp->q_lock,NULL);
    if(err!=0)
        return(err);
    return(0);    
}

void
job_insert(struct queue *qp,struct job *jp){
    pthread_rwlock_wrlock(&qp->q_lock); //锁定队列
    jp->j_next = qp->q_head;
    jp->j_prev = NULL;
    if(qp->q_head != NULL){
        qp->q_head->j_prev = jp;
    }else{
        qp->q_tail = jp;
    }
    qp->q_head = jp;
        printf("插入作业到队列成功\n");
    pthread_rwlock_unlock(&qp->q_lock);
}

//追加节点到队列尾部
void
job_append(struct queue *qp,struct job *jp){
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = NULL;
    jp->j_prev = qp->q_tail;
    if(qp->q_tail != NULL)
        qp->q_tail->j_next  = jp;
    else
        qp->q_head = jp;
    qp->q_tail = jp;
    pthread_rwlock_unlock(&qp->q_lock);     
}

//移除节点
void
job_remove(struct queue *qp,struct job *jp){
    pthread_rwlock_wrlock(&qp->q_lock);
    if(jp == qp->q_head){
        qp->q_head = jp->j_next;
        if(qp->q_tail == jp){
            qp->q_tail = NULL;
        }else{
            jp->j_next->j_prev = jp->j_prev;
        }
    }else if(jp == qp->q_tail){
        qp->q_tail = jp->j_prev;
        jp->j_prev->j_next = jp->j_next;
    }else{
        jp->j_next->j_prev = jp->j_prev;
        jp->j_prev->j_next = jp->j_next;
    }
    pthread_rwlock_unlock(&qp->q_lock);
}

struct job *
job_find(struct queue *qp,pthread_t id){
    struct job *jp;
    if(pthread_rwlock_rdlock(&qp->q_lock)!=0)
        return(NULL);
    for(jp = qp->q_head;jp!=NULL;jp = jp->j_next)
        if(pthread_equal(jp->j_id,id))
            break;    
    pthread_rwlock_unlock(&qp->q_lock);
    return(jp);
}


int main(void){
    struct queue my_queue; //实例化队列
    queue_init(&my_queue);
    for(int i=0;i<5;i++){
    struct job new_job;
    new_job.j_id = pthread_self(); //实例化节点
    printf("输出当前线程id:%lu\n",new_job.j_id);
    job_insert(&my_queue,&new_job);//节点插入队列
    }



}




