/* buffer.h */
typedef int buffer_item;

#define BUFFER_SIZE 5
#define MAX_NUMBER_OF_THREADS 5 

void buffer_init();
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void *producer(void *param);
void *consumer(void *param);