#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "pthreadpool.h"

struct thread_pool *thread_pool_init(int t_num,int job_num)
{
    struct thread_pool *pool = NULL;
    pool = (struct thread_pool *)malloc(sizeof(struct thread_pool));

    //if wrong

    pool->thread_num = t_num;
    pool->job_quene_cur_num = 0;
    pool->job_quene_max_num = job_num;
    pool->first = NULL;
    pool->end = NULL;
    pthread_mutex_init(&(pool->mutex), NULL); //初始化互斥信号量
    pthread_cond_init(&(pool->queue_empty), NULL);//初始化队列空条件变量
    pthread_cond_init(&(pool->queue_not_empty), NULL);//初始化队列不为空条件变量
    pthread_cond_init(&(pool->queue_not_full), NULL);//初始化队列不满条件变量
    pool->thread_ID = (pthread_t *)malloc(sizeof(pthread_t) * t_num);

    for(int i = 0;i < t_num; i++)
    {
        pthread_create(&(pool->thread_ID[i]), NULL, &thread_work, (void *)pool);
    }

    return pool;
}

void *thread_work(void *arg)
{
    struct thread_pool *pool = (struct thread_pool *)arg;
    struct job *work_job = NULL;

    while(1)
    {
        //lock
        pthread_mutex_lock(&(pool->mutex));
        do
        {
            //wait
            pthread_cond_wait(&(pool->queue_not_empty), &(pool->mutex));
        }while(pool->job_quene_cur_num <= 0);
       
        work_job = pool->first;
        pool->job_quene_cur_num--;
        if(pool->job_quene_cur_num == 0)
        {
            pool->first = pool->end = NULL;
        }
        else
        {
            pool->first = pool->first->next;
        }
        pthread_mutex_unlock(&(pool->mutex));
        //unlock

        (*(work_job->job_function))(work_job->arg);
        free(work_job);
        work_job = NULL;
    }

    return NULL;
}

int thread_add_job(struct thread_pool *pool,void *arg,void *job_fun(void *))
{
    struct job* end_job;
    pthread_mutex_lock(&(pool->mutex));
    if(pool->job_quene_cur_num == pool->job_quene_max_num)
    {
        //job quene full
        //drop the job
        return 0;
    }
    //lock
    end_job = (struct job*)malloc(sizeof(struct job));
    //if wrong
    end_job->next = NULL;
    if(pool->job_quene_cur_num == 0)
    {
        pool->first = end_job;
        pthread_cond_signal(&pool->queue_not_empty);
    }
    else
    {
        pool->end->next = end_job;
    }
    pool->job_quene_cur_num++;
    pool->end = end_job;
    end_job->job_function = job_fun;
    end_job->arg = arg;
    //ublock
    pthread_mutex_unlock(&(pool->mutex));
    /*if(pool->job_quene_cur_num == 1)
    {
        
        pthread_cond_broadcast(&(pool->queue_not_empty));  //队列空的时候，有任务来时就通知线程池中的线程：队列非空
    }*/
    return 1;
}