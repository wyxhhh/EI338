#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

static struct node* taskhead[MAX_PRIORITY+MIN_PRIORITY];
static struct node* tasktail[MAX_PRIORITY+MIN_PRIORITY];

// add a task to the list 
void add(char *name, int priority, int burst) {
    Task* newTask = malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    if (tasktail[priority] == NULL)
        tasktail[priority] = malloc(sizeof(struct node));
    insertEnd(&tasktail[priority], newTask);
    if (taskhead[priority] == NULL)
        taskhead[priority] = tasktail[priority];
    return;
}

void run_task(struct node** head, struct node** tail) {
    Task *t = (*head)->task;
    if (t->burst > QUANTUM) {
        run(t, QUANTUM);
        t->burst -= QUANTUM;
        (*tail)->next = (*head);
        struct node *p = (*head)->next;
        (*head) = p;
        (*tail) = (*tail)->next;
        (*tail)->next = NULL;
    }
    else {
        run(t, t->burst);
        struct node *p = (*head)->next;
        free(*head);
        (*head) = p;
    }
}

// invoke the scheduler
void schedule() {
    for (int i = MAX_PRIORITY; i >= MIN_PRIORITY; --i)
        while (taskhead[i] != NULL)
            run_task(&taskhead[i], &tasktail[i]);
        return;
}
