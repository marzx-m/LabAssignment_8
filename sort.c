/*
Mariam Befekadu
COP 3502C
Lab Assignment 8
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[51];
    int tickets;
    int line;
    int arrivalTime;
} Customer;

typedef struct Node {
    Customer *customer;
    struct Node *next;
} Node;

typedef struct {
    Node *front;
    Node *rear;
    int size;
} Queue;

typedef struct {
    Queue *queues[13];
} Booth;

Customer *createCustomer(char name[51], int tickets, int line, int arrivalTime) {
    Customer *newCustomer = (Customer *)malloc(sizeof(Customer));
    strcpy(newCustomer->name, name);
    newCustomer->tickets = tickets;
    newCustomer->line = line;
    newCustomer->arrivalTime = arrivalTime;
    return newCustomer;
}

Queue *createQueue() {
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

void enqueue(Queue *queue, Customer *customer) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->customer = customer;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }

    queue->size++;
}

Customer *dequeue(Queue *queue) {
    if (queue->front == NULL) {
        return NULL; 
    }

    Node *temp = queue->front;
    Customer *customer = temp->customer;

    queue->front = temp->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    queue->size--;

    return customer;
}

Customer *peek(Queue *queue) {
    if (queue->front == NULL) {
        return NULL; 
    }

    return queue->front->customer;
}

int isEmpty(Queue *queue) {
    return queue->front == NULL;
}

int getSize(Queue *queue) {
    return queue->size;
}

void initializeBooths(Booth *booths) {
    for (int i = 1; i <= 12; i++) {
        booths->queues[i] = createQueue();
    }
}

void freeBooths(Booth *booths) {
    for (int i = 1; i <= 12; i++) {
        free(booths->queues[i]);
    }
}

int main() {
    int n, b;
    scanf("%d %d", &n, &b);

    Booth booths;
    initializeBooths(&booths);

    for (int i = 0; i < n; i++) {
        char name[51];
        int tickets, time;
        scanf("%s %d %d", name, &tickets, &time);

        int p = name[0] - 'A';
        int queueNumber = (p % 13 != 0) ? (p % 13) : 12;

        enqueue(booths.queues[queueNumber], createCustomer(name, tickets, queueNumber, time));
    }

        for (int i = 1; i <= b; i++) {
          printf("Booth %d\n", i);

          int totalQueues = 12 / b;
          int extraQueues = 12 % b;
          int assignedQueues = (i <= extraQueues) ? (totalQueues + 1) : totalQueues;

          int startQueue = (i - 1) * totalQueues + (i <= extraQueues ? i : extraQueues) + 1;

          for (int j = startQueue; j < startQueue + assignedQueues; j++) {
          Queue *currentQueue = booths.queues[j];

          while (!isEmpty(currentQueue)) {
              Customer *customer = dequeue(currentQueue);

              int processingTime = 30 + customer->tickets * 5;
              int checkoutTime = customer->arrivalTime + processingTime;

              printf("%s from line %d checks out at time %d.\n", customer->name,
                     customer->line, checkoutTime);
              free(customer); 
          }
      }

      printf("\n");
  }

    freeBooths(&booths);

    return 0;
}