#ifndef QUEUE_H
#define QUEUE_H

typedef struct Node{
    int data;
    Node * next;
} Node;


struct Queue{
    Node * head;
    Node * tail;
} Queue;

void Queue__put(Queue * self, int value){
    if (self->head == NULL){
        self->head = malloc(sizeof(Node));
        self->head->data = value;
        self->tail = self->head;
        self->tail->next = NULL;
        return;
    }
    self->tail->next = malloc(sizeof(Node));
}

int Queue__pop(Queue * self){
    int val = self->head->data;
    Node * tmp = self->head->next;
    free(self->head);
    head = tmp;
    return val;
}

#endif
