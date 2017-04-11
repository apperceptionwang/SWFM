struct job
{
    void* (*job_function)(void *);
    void *arg;
    struct job *next;
};

struct thread_pool
{
    int thread_num;
    int job_quene_max_num;
    int job_quene_cur_num;

    struct job *first;
    struct job *end;

    pthread_mutex_t mutex;            //互斥信号量
    pthread_cond_t queue_empty;       //队列为空的条件变量
    pthread_cond_t queue_not_empty;   //队列不为空的条件变量
    pthread_cond_t queue_not_full;    //队列不为满的条件变量

    pthread_t *thread_ID;
};

struct thread_pool *thread_pool_init(int t_num,int job_num);
void *thread_work(void *arg);
int thread_add_job(struct thread_pool *pool,void *arg,void *job_fun(void *));