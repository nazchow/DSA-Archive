// Naziat Chowdhury
// 11/23/23
// Assignment P6 
// COP3502 - Guha

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines constants
#define MAXLEN 19
#define TABLESIZE 300007

// Given structs and hash function
typedef struct item {
    char name[MAXLEN + 1];
    int quantity;
    int saleprice;
} item;


typedef struct node {
    item *iPtr;
    struct node *next;
} node;

typedef struct hashtable {
    node **lists;
    int size;
} hashtable;

int hashfunc(char *word, int size) {
    int len = strlen(word);
    int res = 0;
    for (int i = 0; i < len; i++) {
        res = (1151 * res + (word[i] - 'a')) % size;
    }
    return res;
}

// Struct for proceessing commands
typedef struct command {
    char type[MAXLEN + 1];
    char name[MAXLEN + 1];
    int quantity;
    int price;
} command;


// Initialization function
// Allocates memory, sets size, initializes pointers, returns pointer to table 
hashtable *initialize(int size) {
    hashtable *hashP = (hashtable *)malloc(sizeof(hashtable));
    hashP->size = size;
    hashP->lists = (node **)malloc(sizeof(node *) * size);
    for (int i = 0; i < size; i++) {
        hashP->lists[i] = NULL;
    }
    return hashP;
}

// Function for creating new node in LL
// Allocates memory, links node to item, returns pointer to node
node *nodeFunc(item *i) {
    node *newNode = (node *)malloc(sizeof(node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->iPtr = i;
    newNode->next = NULL;
    return newNode;
}

// Insert function for updating hash table
// Gets hash index for item, traverses linked list, counter to track operations (complexity) made
// If there is item with same name quantity and cash are updated and the status of the inventory is printed
void insert(hashtable *hashP, char *name, int quantity, int totalPrice, int *totalComplex, int *totalCash) {
    int indexN = hashfunc(name, hashP->size);
    node *currentPtr = hashP->lists[indexN];
    int complexity = 1;

    while (currentPtr != NULL) {
        if (strcmp(currentPtr->iPtr->name, name) == 0) {
            currentPtr->iPtr->quantity += quantity;
            *totalCash -= totalPrice;
            printf("%s %d %d\n", name, currentPtr->iPtr->quantity, *totalCash);
            *totalComplex += complexity;
            return;
        }
        complexity++;
        currentPtr = currentPtr->next;
    }

    // Creates new item and node if item isn't found, node inserted to the start of the list, cash adjusted
    item *newItem = (item *)malloc(sizeof(item));
    strcpy(newItem->name, name);
    newItem->quantity = quantity;
    newItem->saleprice = totalPrice / quantity;

    node *newNode = nodeFunc(newItem);
    newNode->next = hashP->lists[indexN];
    hashP->lists[indexN] = newNode;

    *totalCash -= totalPrice;
    printf("%s %d %d\n", name, quantity, *totalCash);
    *totalComplex += complexity;
}

// Function for selling items- updates cash and keeps track of complexity
// Iterates through LL, variable for quantity sold declared, finds revenue (items * price)
// When item is sold quantity is decreased by the sold but not deleted from hashtable if quantity = 0
// Selling more than whats available: all available items sold and cash is updated
void sell(hashtable *hashP, char *name, int quantity, int *totalCash, int *totalComplex) {
    int index = hashfunc(name, hashP->size);
    node *current = hashP->lists[index];
    int complexity = 1;

    while (current != NULL) {
        if (strcmp(current->iPtr->name, name) == 0) {
            int soldItems;
            if (quantity <= current->iPtr->quantity) {
                soldItems = quantity;
            } 
            else {
                soldItems = current->iPtr->quantity;
            }
            int revenue = soldItems * current->iPtr->saleprice;
            current->iPtr->quantity -= soldItems;
            *totalCash += revenue;
            printf("%s %d %d\n", name, current->iPtr->quantity, *totalCash);
            *totalComplex += complexity;
            return;
        }
        complexity++;
        current = current->next;
    }
}

// Function to update selling price
// Complexity initialization and updates, determines index, iterates through LL, if name is found price updates, searches until end
// newPrice is the saleprice if found
void changePrice(hashtable *hashP, char *name, int newPrice, int *totalComplex) {
    int indexN = hashfunc(name, hashP->size);
    node *currentPtr = hashP->lists[indexN];
    int complexity = 1;

    while (currentPtr != NULL) {
        if (strcmp(currentPtr->iPtr->name, name) == 0) {
            currentPtr->iPtr->saleprice = newPrice;
            *totalComplex += complexity;
            return;
        }
        complexity++;
        currentPtr = currentPtr->next;
    }
}

// Function to search items in hash table
// Iterates LL, if name found the function returns pointer to the item, if not found reaching end of list, current pointer is NULL
item* find(hashtable *hashP, char *name) {
    int indexN = hashfunc(name, hashP->size);
    node *currentPtr = hashP->lists[indexN];

    while (currentPtr != NULL) {
        if (strcmp(currentPtr->iPtr->name, name) == 0) {
            return currentPtr->iPtr;
        }
        currentPtr = currentPtr->next;
    }

    return NULL;
}

int main() {
    // Reads in commands
    int n;
    scanf("%d", &n);

    // Initializes cash, complexity, hash table
    int totalCash = 100000; 
    int totalComplex = 0;
    hashtable *hashP = initialize(TABLESIZE);
    command *commands = (command *)malloc(sizeof(command) * n);

    // Reads + processes commands, buffers input and processes based on type, calls function appropriately
    for (int i = 0; i < n; i++) {
        scanf("%s", commands[i].type);
        if (strcmp(commands[i].type, "buy") == 0 || strcmp(commands[i].type, "sell") == 0) {
            scanf("%s %d %d", commands[i].name, &commands[i].quantity, &commands[i].price);
        } 
        else if (strcmp(commands[i].type, "change_price") == 0) {
            scanf("%s %d", commands[i].name, &commands[i].price);
        }
    }

    for (int i = 0; i < n; i++) {
        if (strcmp(commands[i].type, "buy") == 0) {
            insert(hashP, commands[i].name, commands[i].quantity, commands[i].price, &totalComplex, &totalCash);
        } 
        else if (strcmp(commands[i].type, "sell") == 0) {
            sell(hashP, commands[i].name, commands[i].quantity, &totalCash, &totalComplex);
        } 
        else if (strcmp(commands[i].type, "change_price") == 0) {
            changePrice(hashP, commands[i].name, commands[i].price, &totalComplex);
        }
    }

    // Outputs total cash and complexity
    printf("%d\n%d\n", totalCash, totalComplex);

    // Freeing memory
    for (int i = 0; i < TABLESIZE; i++) {
        node *currentPtr = hashP->lists[i];
        while (currentPtr != NULL) {
            node *temp = currentPtr;
            currentPtr = currentPtr->next;
            free(temp->iPtr);
            free(temp);
        }
    }
    free(hashP->lists);
    free(hashP);
    free(commands);

    return 0;
}
