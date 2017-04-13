/*
任务队列
*/
struct job
{
    void* (*job_function)(void *);//任务函数
    int arg;//参数
    struct job *next;
};

/*
线程池结构体
*/
struct thread_pool
{
    int thread_num;//线程个数
    int job_quene_max_num;//任务队列最大长度
    int job_quene_cur_num;//任务队列当前长度

    struct job *first;//任务队列首指针
    struct job *end;//任务队列尾指针

    pthread_mutex_t mutex;            //互斥信号量
    pthread_cond_t queue_empty;       //队列为空的条件变量
    pthread_cond_t queue_not_empty;   //队列不为空的条件变量
    pthread_cond_t queue_not_full;    //队列不为满的条件变量

    pthread_t *thread_ID;//所以线程ID
};

struct thread_pool *thread_pool_init(int t_num,int job_num);    
/**
*Purpose  初始化线程池
*Params   
*   @int t_num  线程池最大线程数量
*   @int job_num 任务队列长度
*Return  线程池地址
**/

void *thread_work(void *arg);
/**
*Purpose  工作函数
*Params   
*   @void *arg  任务函数
*Return  NULL
**/
int thread_add_job(struct thread_pool *pool,int arg,void *job_fun(void *));
/**
*Purpose    向任务队列中添加任务
*Params   
*   @struct thread_pool *pool   线程池地址
*   @int arg    任务函数需要的参数
*   @void *job_fun(void *)      任务函数
*Return  1 添加成功
**/