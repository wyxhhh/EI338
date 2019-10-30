#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int nums[100]; // the array need to be sorted
int value = 0;

void *mergeSort(void* headTailPoint);


int main(int argc, char *argv[])
{
    int tmp, N;
    scanf("%d", &N);
    for (int i = 0; i < N; ++i)
        scanf("%d", &nums[i]);
    int headTailPoint[2] = {0, N};

	pthread_t tid; pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_create(&tid,&attr,mergeSort,headTailPoint);
    pthread_join(tid,NULL);

    for (int i = 0; i < N; ++i)
        printf("%d ", nums[i]);
}


void *mergeSort(void* headTailPoint) {
    int head = ((int*) headTailPoint)[0], tail = ((int*) headTailPoint)[1];
    if (head+1 >= tail)
        return NULL;
    int midPoint = (head + tail) / 2;
    
    // initialize two threads to perform merge sort
	pthread_t tid1, tid2; pthread_attr_t attr1, attr2;

    int ht1[2] = {head, midPoint};
    pthread_attr_init(&attr1);
    pthread_create(&tid1,&attr1,mergeSort,ht1);
    pthread_join(tid1,NULL);
    int ht2[2] = {midPoint, tail};
    pthread_attr_init(&attr2);
    pthread_create(&tid2,&attr2,mergeSort,ht2);
    pthread_join(tid2,NULL);

    // merge sorted list
    int tmp[100];
    int i = head, j = midPoint, k = 0;
    while (i < midPoint && j < tail) {
        if (nums[i] < nums[j])
            tmp[k++] = nums[i++];
        else
            tmp[k++] = nums[j++];
    }
    while (i < midPoint) 
        tmp[k++] = nums[i++];
    while (j < tail)
        tmp[k++] = nums[j++];
    for (i = 0; i < k; ++i) {
        nums[i+head] = tmp[i];
    }

    return NULL;
}