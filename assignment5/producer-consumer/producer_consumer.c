#include <stdlib.h> /* required for rand() */
#include <stdio.h>
#include <unistd.h>
#include "buffer.h"

#define SLEEP_MAX 3;

void *producer(void *param) {
    buffer_item item;
    
    while (1) {
        int sleep_time = rand() % SLEEP_MAX;
        sleep(sleep_time);

        /* generate a random number */
        item = rand();
        if (insert_item(item))
            fprintf(stderr, "report error condition");
        else
            printf("producer produced %d∖n",item);
    }
}

void *consumer(void *param) {
    buffer_item item;

    while (1) {
        int sleep_time = rand() % SLEEP_MAX;
        sleep(sleep_time);
        
        if (remove_item(&item))
            fprintf(stderr, "report error condition");
        else
            printf("consumer consumed %d∖n",item);
    }
}