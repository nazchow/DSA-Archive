// Naziat Chowdhury
// P2
// COP3502C - GUHA
// September 24th, 2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines constants, maxium name length, queue numbers and constants for the processing time (30 + number of tickets * 5)
#define MAX_NAME_LEN 50
#define MAX_QUEUES 12
#define TICKET_TIME 5
#define BASE_TIME 30

// Defines the customer struct
// Members: array of characters to store the name (+1 for null terminator),
// Time when customer finishes checkout,
// Assigned line number.
// Customer's initial arrival time
typedef struct Customer {
    char name[MAX_NAME_LEN + 1];
    int checkoutTime;
    int lineNum;
    int tickets;
    int arrivalTime;
} Customer;

// Defines node structure for a linked list
// Contains pointer to the customer structure to store memory address
//  Contains pointer to another node of type llnode
typedef struct llnode {
    Customer* customerPtr;
    struct llnode* next;
} llnode;

// Defines customer queue structure
// Front and back pointers give access for enqueing and dequeueing
// Size member is for customers in the queue
typedef struct queueStruct {
    llnode* front;
    llnode* rear;
    int size;
} queueStruct;

// Customer function creates new custumber through initialization and values of the parameters for the name, tickets, line number, and arrival time of the customer
// Allocates memory, copies the name into the name member through string function
// Checkout time is initialized to 0 to indicate that the customer is not finished checking out
Customer* customerCreate(char name[], int tickets, int lineNum, int arrivalTime) {
    Customer* customerPtr = (Customer*)malloc(sizeof(Customer));
    strcpy(customerPtr->name, name);
    customerPtr->tickets = tickets;
    customerPtr->lineNum = lineNum;
    customerPtr->arrivalTime = arrivalTime;
    customerPtr->checkoutTime = 0;
    return customerPtr;
}

// Creates a new node for the linked list w/ pointers to the customer struct, pointer to the next node
// Allocates memory, initializes new node
llnode* llnodeCreate(Customer* customerPtr) {
    llnode* node = (llnode*)malloc(sizeof(llnode));
    node->customerPtr = customerPtr;
    node->next = NULL;
    return node;
}

// Initializes new queue w/ linked list
// Allocates memory
// Size set to 0 since queue starts off with no elements in it
queueStruct* queueStructFunc() {
    queueStruct* queue = (queueStruct*)malloc(sizeof(queueStruct));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

// Function to enqueue customers to the queue - creates a new node to hold information
// Checks if queue is empty w/ NULL, front and rear are set to the same node
// For queues with elements - links the new node to the back end of the queue
// Size incremented by 1 to track the elements in queue
void queueStructEnqueue(queueStruct* queue, Customer* customerPtr) {
    llnode* newNode = llnodeCreate(customerPtr);
    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    queue->size+=1;
}

// Function removes the front element in the queue and returns it 
// Returns NULL if empty
// Front of the queue points to the nxet node
// Rear pointer set to NULL after checking if the queue is empty after the dequeue operation, dequed node is freed 
// Size is decremented since element is removed
Customer* queueStructDequeue(queueStruct* queue) {
    if (queue->front == NULL) {
        return NULL;
    }
    llnode* temp = queue->front;
    Customer* customerPtr = temp->customerPtr;
    queue->front = temp->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    queue->size+= -1;
    return customerPtr;
}

// Function looks at the customer at the front of the queue 
// Checks to see if the queue is empty, returns the customer pointer to the front node if not empty
Customer* queueStructPeek(queueStruct* queue) {
    if (queue->front == NULL) {
        return NULL;
    }
    return queue->front->customerPtr;
}

// Checks if a given queue is empty by looking at the front pointer
int queueStructIsEmpty(queueStruct* queue) {
    return (queue->front == NULL);
}

// Returns the number of items in a certain queue
int queueStructGetSize(queueStruct* queue) {
    return queue->size;
}

// Function to insert a customer into the customers array
// Iterates through the erray from the end to the front as arrival time of the customer is greater than the new customer's arrival time and as the arrivaltime of the customer is the same as the new customer but the queue number is greater than that of new customer
// No conditions met: loop breaks and i points to the spot where new customer shoudl be inserted
void insertSorted(Customer** customers, Customer* newCustomer, int n) {
    int i;
    for (i = n - 1; (i >= 0 &&
                    (customers[i]->arrivalTime > newCustomer->arrivalTime ||
                    (customers[i]->arrivalTime == newCustomer->arrivalTime && 
                    customers[i]->lineNum > newCustomer->lineNum)));
         i+= -1) {
        customers[i + 1] = customers[i];
    }
    customers[i + 1] = newCustomer;
}

int main() {
    // Defines and reads in input for the number of custombers buyign tickets and booths operating that day (n and b)
    int n, b;
    scanf("%d %d", &n, &b);

    // Allocates and initializes an array of pointers
    // Loop starts and index i up to the number of booths operating
    // Function call, returns a pointer to queueStruct
    queueStruct** queues = (queueStruct**)malloc((b + 1) * sizeof(queueStruct*));
    for (int i = 1; i <= b; i++) {
        queues[i] = queueStructFunc();
    }

    // Allocates memory for an array of integers of size b+1
    int* booths = (int*)malloc((b + 1) * sizeof(int));
    for (int i = 1; i <= b; i++) {
        booths[i] = 1;
    }

    // Allocates memory for an array of pointers to customers
    Customer** customers = (Customer**)malloc(n * sizeof(Customer*));

    // Initialized number of customers
    int numCustomers = 0;

    for (int i = 0; i < n; i++) {
        char name[51];
        int tickets, arrivalTime;
        scanf("%s %d %d", name, &tickets, &arrivalTime);

        // Determines num based on 1st letter of customers nname
        int p = name[0] - 'A'; 
        int queueNum;
        
        // Queue number is p % 13, if it == 0 then the customer is assigned to the first nonempty queue w/ the least nubmer of customers
        if (p % 13 != 0) {
            queueNum = p % 13;
        } else {
            int minQueueSize = queueStructGetSize(queues[1]);
            queueNum = 1;

            for (int j = 1; j <= b; j++) { 
                int currentSize = queueStructGetSize(queues[j]);
                if (currentSize < minQueueSize) {
                    minQueueSize = currentSize;
                    queueNum = j;
                }
            }
        }
        // Function call
        Customer* newCustomer = customerCreate(name, tickets, queueNum, arrivalTime);
        
        // Puts the customer into the array based on arrival time and line num
        insertSorted(customers, newCustomer, numCustomers);
        numCustomers+= 1;

        // Enqueues the customer to queues based on min size
        int minBooth = 1;
        for (int j = 2; j <= b; j++) {
            if (queues[j]->size < queues[minBooth]->size) {
                minBooth = j;
            }
        }
        // Enequeues new customer to the booth, minBooth used an as index to pick certain booths
        queueStructEnqueue(queues[minBooth], newCustomer);
        booths[minBooth]+= 1;
    }
    // Initializes current time
    int currentTime = 0;
    // Iterates through booths
    for (int i = 1; i <= b; i++) {
        printf("Booth %d\n", i);

        while (!queueStructIsEmpty(queues[i])) {
            Customer* customerPtr = queueStructDequeue(queues[i]);

            // Checks if arrival time is greater than current time
            if (customerPtr->arrivalTime > currentTime) {
                currentTime = customerPtr->arrivalTime;
            }

            // Calculates total time and prints results, then frees memory
            int processingTime = BASE_TIME + customerPtr->tickets * TICKET_TIME;
            customerPtr->checkoutTime = currentTime + processingTime;
            currentTime = customerPtr->checkoutTime;
            
            printf("%s from line %d checks out at time %d.\n", customerPtr->name, customerPtr->lineNum, customerPtr->checkoutTime);

            free(customerPtr);
        }
        // Blank line outputted after each booth
        printf("\n");

        // Memory freed
        free(queues[i]);
    }

    free(queues);
    free(booths);
    free(customers);

    return 0;
}

