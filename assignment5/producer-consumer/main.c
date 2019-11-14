#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_t producers[MAX_NUMBER_OF_THREADS];
pthread_t consumers[MAX_NUMBER_OF_THREADS];

int main(int argc, char *argv[]) {
    /* 1. Get command line arguments argv[1],argv[2],argv[3] */
    int sleep_time = atoi(argv[1]);
    int producer_cnt = atoi(argv[2]);
    int consumer_cnt = atoi(argv[3]);

    /* 2. Initialize buffer */
    buffer_init();

    /* 3. Create producer thread(s) */
    for (int i = 0; i < producer_cnt; ++i) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }

    /* 4. Create consumer thread(s) */
    for (int i = 0; i < consumer_cnt; ++i) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    /* 5. Sleep */
    sleep(sleep_time);

    /* 6. Exit */
    for (int i = 0; i < producer_cnt; ++i) {
        pthread_cancel(producers[i]);
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < consumer_cnt; ++i) {
        pthread_cancel(consumers[i]);
        pthread_join(consumers[i], NULL);
    }

    return 0;
}