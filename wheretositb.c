// Naziat Chowdhury
// 10/2/23
// COP3502C - Guha
// P3 - Where to Sit? Program 2

#include <stdio.h>
#include <string.h>

// Variable and array declaration and initialization
int n, p;
char names[10][20];
int popcorn[10];
int notNextTo[10][10];
int numValid = 0; 

// Function that checks the valid orderings of the permutation given the input
// Checks if person has popcorn to the left and right
// Checks indicies and whether or not to people can be next to each other, returns 0 if a pair cannot be next to each other 
int validOrd(int *permutation){
    for(int i = 0; i < n; i++){
        int leftPopcorn = (i > 0 && popcorn[permutation[i-1]]);
        int rightPopcorn = (i < n-1 && popcorn[permutation[i+1]]);
        if(!popcorn[permutation[i]] && !leftPopcorn && !rightPopcorn){
            return 0;
        }
    }

    for(int i = 0; i < n-1; i++){
        if(notNextTo[permutation[i]][permutation[i+1]]){
            return 0;
        }
    }
    return 1;
}

// Function finds and prints valid permutation
// Base case: if a valid permutation is found the function stops checking
// numValid is set to 1 if a valid permutation is found and it will print the names related with the permutation
// Backtracks for other options for the current position, permutation[index] changes
void printPermutations(int *permutation, int index){
    if(numValid) {  // Skip checking once a valid arrangement is found
        return;
    }

    if(index == n){
        if(validOrd(permutation)){
            numValid = 1;
            for(int i = 0; i < n; i++){
                printf("%s\n", names[permutation[i]]);
            }
        }
        return;
    }

    for(int i = 0; i < n; i++){
        int used = 0;
        for(int j = 0; j < index; j++){
            if(permutation[j] == i){
                used = 1;
                break;
            }
        }

        if(!used){
            permutation[index] = i;
            printPermutations(permutation, index+ 1);
        }
    }
}

int main(){
    // Takes in input for the number of people attending the movie + the number of pairs of people who don't want to sit with each other
    scanf("%d %d", &n, &p);

    // Takes in input for names and popcorn and stores it into two arrays each iteration
    for(int i = 0; i < n; i++){
        scanf("%s %d", names[i], &popcorn[i]);
    }

    // Reads in names
    // Character arrays hold strings up to 19 characters (20 to account for the null terminator)
    // Integer variables are initialized to -1 and stores the indices for the names in the array
    // String function used to compare name1 and name2  to strings in the array
    // notnextto array is updated to indicate that the names at position index1 and index2 cannot be sat next to each other
    for(int i = 0; i < p; i++){
        char name1[20], name2[20];
        scanf("%s %s", name1, name2);

        int index1 = -1, index2 = -1;
        for(int j = 0; j < n; j++){
            if(strcmp(name1, names[j]) == 0) index1 = j;
            if(strcmp(name2, names[j]) == 0) index2 = j;
        }
        notNextTo[index1][index2] = notNextTo[index2][index1] = 1;
    }
    // Permutation array is declared
    int permutation[10];

    // Function call
    printPermutations(permutation, 0);

    return 0;
}
