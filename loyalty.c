// Naziat Chowdhury
// 11/7/23
// Assignment P5
// COP3502C - Guha

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines constants and strucs 
#define getMaxLEN 19

typedef struct customer {
    char name[getMaxLEN + 1];
    int points;
} customer;

typedef struct treenode {
    customer* cPtr;
    int size;
    struct treenode* left;
    struct treenode* right;
} treenode;

// Function to see if a letter is lowercase
int lowercaseFunc(char c) {
    return (c >= 'a' && c <= 'z');
}
// Function pointer type for comparing
typedef int (*compFunc)(const void*, const void*);

// array stores pointer to the customer struct
// Global counter variable to initialize array and count the customer number in the array
customer** custarray;
int arraySize = 0; 

// Function for creating a new node in the BST for the customer
treenode* createNode(char* name, int points) {
    treenode* newNode = (treenode*)malloc(sizeof(treenode));
    newNode->cPtr = (customer*)malloc(sizeof(customer));
    strncpy(newNode->cPtr->name, name, getMaxLEN);
    newNode->cPtr->points = points;
    newNode->size = 1;
    newNode->left = newNode->right = NULL;
    return newNode;
}
// Function that inserts a new customer into BST
treenode* insertNode(treenode* root, customer* cust) {
    if (root == NULL) {
        return createNode(cust->name, cust->points);
    }
    int compRes = strcmp(cust->name, root->cPtr->name);
    if (compRes < 0) {
        root->left = insertNode(root->left, cust);
    } 
    else if (compRes > 0) {
        root->right = insertNode(root->right, cust);
    } 
    else {
        root->cPtr->points += cust->points;
    }

    int leftSize = 0;
    int rightSize = 0;

    // Check if left child exists
    if (root->left != NULL) {
        leftSize = root->left->size;
    }

    // Checks if right child exists
    if (root->right != NULL) {
        rightSize = root->right->size;
    }
    // Update node
    root->size = 1 + leftSize + rightSize;
    return root;
}

// Function adds points to the customer, inserts customer into tree
treenode* addPoints(treenode* root, char* name, int points) {
    // Create a new customer
    customer* newCust = (customer*)malloc(sizeof(customer));
    if (!newCust) {
        return root;
    }

    strncpy(newCust->name, name, getMaxLEN);
    newCust->name[getMaxLEN] = '\0';
    newCust->points = points;

    return insertNode(root, newCust);
}

// Function subtracts points from customer, makes sure that points dont exceed 0
treenode* subPoints(treenode* root, char* name, int points) {
    if (root == NULL) {
        return NULL;
    }

    int compRes = strcmp(name, root->cPtr->name);
    if (compRes < 0) {
        root->left = subPoints(root->left, name, points);
    } 
    else if (compRes > 0) {
        root->right = subPoints(root->right, name, points);
    } 
    else {
        if (root->cPtr->points > points) {
            root->cPtr->points -= points;
        } 
        else {
            // customer has fewer points than specified, set points to 0.
            root->cPtr->points = 0;
        }
    }
    // Size of node to root update
    root->size = 1;
    // Adds L child size to root size
    if (root->left != NULL) {
        root->size += root->left->size;
    }
    // Adds R child size to root size
    if (root->right != NULL) {
        root->size += root->right->size;
    }   
    return root;
}

// Function for finding the maximum
treenode* getMax(treenode* node) {
    while (node && node->right != NULL) {
        node = node->right;
    }
    return node;
}

// Function deletes customer from tree, compares name with current name, iterates through tree
treenode* delete(treenode* root, char* name) {
    if (root == NULL) {
        printf("%s not found\n", name);
        return NULL;
    }

    int compRes = strcmp(name, root->cPtr->name);
    if (compRes < 0) {
        root->left = delete(root->left, name);
    } 
    else if (compRes > 0) {
        root->right = delete(root->right, name);
    } 
    else {
        // No left child case
        if (root->left == NULL) {
            treenode* tempVar = root->right;
            printf("%s deleted\n", name);
            free(root->cPtr);
            free(root);
            return tempVar;
        } 
        // No right child case
        else if (root->right == NULL) {
            treenode* tempVar = root->left;
            printf("%s deleted\n", name);
            free(root->cPtr);
            free(root);
            return tempVar;
        } 
        // 2 child case - replaced with getMax node in the left subtree
        else {
            treenode* tempVar = root->left;
            while (tempVar->right != NULL) {
                tempVar = tempVar->right;
            }
            strcpy(root->cPtr->name, tempVar->cPtr->name);
            root->left = delete(root->left, tempVar->cPtr->name);
        }
    }
    int leftSize = 0, rightSize = 0;
    // Calculate size of the left subtree if it exists
    if (root->left != NULL) {
        leftSize = root->left->size;
    }   
    // Size of right subtree 
    if (root->right != NULL) {
        rightSize = root->right->size;
    }
    // Total the sizes of L and R subtrees
    root->size = 1 + leftSize + rightSize;
    return root;
}

// Function that searches for customer in the BST and finds depth
treenode* searchDepthFunc(treenode* root, char* name, int* depth) {
    if (root == NULL) {
        return NULL;
    }

    int compRes = strcmp(name, root->cPtr->name);
    if (compRes == 0) {
        return root;
    } 
    else if (compRes < 0) {
        if (depth) (*depth)++;
        return searchDepthFunc(root->left, name, depth);
    } 
    else {
        if (depth) (*depth)++;
        return searchDepthFunc(root->right, name, depth);
    }
}
// Function counts the num of cusutomers with names alphabetically before the actual name
int countSmaller(treenode* root, char* name) {
    int k = 0;
    while (root) {
        int compRes = strcmp(name, root->cPtr->name);
        if (compRes <= 0) {
            root = root->left;
        } else {
            // Calculate the count
            if (root->left != NULL) {
                k += 1 + root->left->size;
            } else {
                k += 1;
            }
            root = root->right;
        }
    }
    return k;
}


// Merge function
void merge(customer** array, int left, int mid, int right, compFunc compare) {
    int leftSub = mid - left + 1;
    int rightSub = right - mid;

    customer** L = (customer**)malloc(leftSub * sizeof(customer*));
    customer** R = (customer**)malloc(rightSub * sizeof(customer*));

    for (int i = 0; i < leftSub; i++) {
        L[i] = array[left + i];
    }
    for (int i = 0; i < rightSub; i++) {
        R[i] = array[mid + 1 + i];
    }

    int i = 0, j = 0, k = left;
    while (i < leftSub && j < rightSub) {
        if (compare(&L[i], &R[j]) <= 0) {
            array[k] = L[i];
            i++;
        } 
        else {
            array[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < leftSub) {
        array[k] = L[i];
        i++;
        k++;
    }

    while (j < rightSub) {
        array[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

// Uses merge sort algo to sort customers by names and points
void mergeSort(customer** array, int left, int right, compFunc compare) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(array, left, mid, compare);
        mergeSort(array, mid + 1, right, compare);
        merge(array, left, mid, right, compare);
    }
}
// Name compare function
int compName(const void* first, const void* second) {
    customer* firstCustomer = *(customer**)first;
    customer* secondCustomer = *(customer**)second;
    return strcmp(firstCustomer->name, secondCustomer->name);
}

// Point compare function 
int compPoints(const void* first, const void* second) {
    customer* firstCustomer = *(customer**)first;
    customer* secondCustomer = *(customer**)second;

    int firstPoints = firstCustomer->points;
    int secondPoints = secondCustomer->points;

    if (firstPoints != secondPoints) {
        return secondPoints - firstPoints;
    }
    return compName(first, second);
}

// Inorder traversal of the BST, fills the customer array
void inorder(treenode* root) {
    if (root != NULL) {
        inorder(root->left);
        custarray[arraySize++] = root->cPtr;
        inorder(root->right);
    }
}

// Function to free tree memory
void freeTree(treenode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root->cPtr);
        free(root);
    }
}

int main() {
    // Asks and reads in the number of commands to go through
    int n;
    scanf("%d", &n);

    // Initial BST root
    treenode* root = NULL;

    // For loop to process each command, ONE AT A TIME
    for (int i = 0; i < n; i++) {
        char command[30];
        scanf("%s", command);

        // adding command
        if (strcmp(command, "add") == 0) {
            char name[getMaxLEN + 1];
            int points;
            scanf("%s %d", name, &points);

            // Check if the name is lowercase and contains only alphabetic characters
            int validName = 1;
            for (int i = 0; name[i] != '\0'; i++) {
                if (!lowercaseFunc(name[i])) {
                    validName = 0;
                    break;
                }
            }

            if (validName && points >= 0 && points <= 100) {
                customer* cust = (customer*)malloc(sizeof(customer));
                strncpy(cust->name, name, getMaxLEN);
                cust->points = points;
                root = insertNode(root, cust);
                printf("%s %d\n", name, cust->points);
            } 
            else {
                return 0;
            }
        } 

        // Subtracting command
        if (strcmp(command, "sub") == 0) {
            char name[getMaxLEN + 1];
            int points;
            scanf("%s %d", name, &points);

            // Search for the customer in the tree
            treenode* found = searchDepthFunc(root, name, NULL);

            // If the customer is found, subtract points
            if (found) {
                root = subPoints(root, name, points);
                printf("%s %d\n", name, found->cPtr->points);
            } 
            else {
                printf("%s not found\n", name);
            }
        }
        // Deleting commmand
        else if (strcmp(command, "del") == 0) {
            char name[getMaxLEN + 1];
            scanf("%s", name);
            root = delete(root, name);
            treenode* found = searchDepthFunc(root, name, NULL);
            if (found) {
                printf("%s deleted\n", name);
            }
        } 
        // Search command
        else if (strcmp(command, "search") == 0) {
            char name[getMaxLEN + 1];
            scanf("%s", name);
            int depth = 0;
            treenode* found = searchDepthFunc(root, name, &depth);
            if (found) {
                printf("%s %d %d\n", name, found->cPtr->points, depth);
            } 
            else {
                printf("%s not found\n", name);
            }
        } 
        // Count smaller command
        else if (strcmp(command, "count_smaller") == 0) {
            char name[getMaxLEN + 1];
            scanf("%s", name);
            int count = countSmaller(root, name);
            printf("%d\n", count);
        }
    }

    // Create and fill array with pointers to customer structs
    // Sort array by points, breaking ties by names.
    custarray = (customer**)malloc(arraySize * sizeof(customer*));
    inorder(root);
    mergeSort(custarray, 0, arraySize - 1, compPoints);

    // Freeing memory  
    for (int i = 0; i < arraySize; i++) {
        printf("%s %d\n", custarray[i]->name, custarray[i]->points);
        free(custarray[i]); 
    }
    free(custarray);
    freeTree(root);

    return 0;
}
