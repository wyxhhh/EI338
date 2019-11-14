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

// invoke the scheduler
void schedule() {
    traverse(taskhead);
    return;
}
