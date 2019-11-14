#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "list.h"

static struct node* taskhead;
static struct node* tasktail;

// add a task to the list 
void add(char *name, int priority, int burst) {
    Task* newTask = malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    if (tasktail == NULL)
        tasktail = malloc(sizeof(struct node));
    insertEnd(&tasktail, newTask);
    if (taskhead == NULL)
        taskhead = tasktail;
    return;
}

void run_shortest_task(struct node* head) {
    struct node* tmp = head;
    Task* t = tmp->task;
    while (tmp != NULL) {
        if (tmp->task->burst < t->burst)
            t = tmp->task;
        tmp = tmp->next;
    }
    printf("[%s] [%d] [%d]\n",t->name, t->priority, t->burst);
    delete(&taskhead, t);
}  

// invoke the scheduler
void schedule() {
    while (taskhead != NULL)
        run_shortest_task(taskhead);
    return;
}
