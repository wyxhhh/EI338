/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// the work queue
task worktodo[QUEUE_SIZE];
static int b = 0;
static int e = 0;
static int work_cnt = 0;

// queue mutex and semaphore
pthread_mutex_t q_mutex;
sem_t sempahore;

// the worker bee
pthread_t bee[NUMBER_OF_THREADS];

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
    pthread_mutex_lock(&q_mutex);
    if (work_cnt == QUEUE_SIZE) {
        pthread_mutex_unlock(&q_mutex);
        return 1;
    }
    worktodo[e++] = t;
    e %= QUEUE_SIZE; work_cnt++;
    pthread_mutex_unlock(&q_mutex);
    return 0;
}

// remove a task from the queue
task dequeue() 
{
    pthread_mutex_lock(&q_mutex);
    task t = worktodo[b++];
    b %= QUEUE_SIZE; work_cnt--;
    pthread_mutex_unlock(&q_mutex);
    return t;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    while (TRUE) {
        sem_wait(&sempahore);
        task t = dequeue();
        execute(t.function, t.data);
    }

    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    task work;
    work.function = somefunction;
    work.data = p;
    int err = enqueue(work);
    if (err) return err;
    sem_post(&sempahore);
    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    sem_init(&sempahore, 0, 0);
    pthread_mutex_init(&q_mutex, NULL);
    for (int i = 0; i < NUMBER_OF_THREADS; ++i)
        pthread_create(&bee[i],NULL,worker,NULL);
}

// shutdown the thread pool
void pool_shutdown(void)
{
    for (int i = 0; i < NUMBER_OF_THREADS; ++i)
    {
        pthread_cancel(bee[i]);
        pthread_join(bee[i],NULL);
    }
}
