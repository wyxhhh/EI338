#include "buffer.h"
#include <pthread.h>
#include <semaphore.h>

/* the buffer */
buffer_item buffer[BUFFER_SIZE];

// mutex lock
pthread_mutex_t buffer_mutex;

// semaphore
sem_t empty, full;

// tail pointer
static int cnt;

void buffer_init() {
    pthread_mutex_init(&buffer_mutex, NULL);
    sem_init(&empty, 0, 0);
    sem_init(&full, 0, BUFFER_SIZE);
    cnt = 0;
}

int insert_item(buffer_item item) {
    /* insert item into buffer
    return 0 if successful, otherwise
    return -1 indicating an error condition */
    sem_wait(&empty);
    pthread_mutex_lock(&buffer_mutex);

    int err = 0;
    if (cnt == BUFFER_SIZE)
        err = -1;
    else
        buffer[cnt++] = item;

    pthread_mutex_unlock(&buffer_mutex);
    sem_post(&full);
    return err;
}

int remove_item(buffer_item *item) {
    /* remove an object from buffer
    placing it in item
    return 0 if successful, otherwise
    return -1 indicating an error condition */
    sem_wait(&full);
    pthread_mutex_lock(&buffer_mutex);

    int err = 0;
    if (cnt == 0) {
        err = -1;
    }
    else
        (*item) = buffer[--cnt];

    pthread_mutex_unlock(&buffer_mutex);
    sem_post(&empty);
    return err;
}
