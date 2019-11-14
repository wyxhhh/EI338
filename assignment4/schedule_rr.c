#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "cpu.h"

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
    while (taskhead != NULL)
        run_task(&taskhead, &tasktail);
    return;
}
