// Naziat Chowdhury
// 8/21/23
// Assignment P0
// COP3502C - Guha

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initializes frequencies with 0 and tracks letters from the first string that are needed for the 2nd
int countLetters(char *str1, char *str2) {
    int initial[26] = {0};
    int lettersNeeded = 0;

    // Iterates through the length of string 1 (not including spaces)
    for (int i = 0; i < strlen(str1); i++) {
        if (str1[i] != ' ') {
            initial[str1[i] - 'A']++; // Subtracts the value of A from index val of characters in string 1 
        }
    }
    // Iterates through the length of 2nd string without including spaces.
    for (int i = 0; i < strlen(str2); i++) {
        if (str2[i] == ' ') {
            continue;
        }
        // Subtracts value of A from the array element for index val. for loop checks for # occurences of letters.
        int indexNum = str2[i] -'A';
        if (initial[indexNum] > 0) {
            initial[indexNum]--;
        }
        // Letters needed incremented and returned if there are no iterations of counting left
        else {
            lettersNeeded++;
        }
    }
    return lettersNeeded;
    }

int main() {
    // Asks and reads in user input
    printf("Enter original message of the sign: ");
    char *str1 = malloc(1000002 * sizeof(char)); // Allocates memory
    fgets(str1, 100002, stdin);
    
    printf("Enter resulting message of the sign: ");
    char *str2 = malloc(1000002 * sizeof(char));  // Allocates memory
    fgets(str2, 100002, stdin);

    // Function call for extra letters needed to make the second string
    int amount = countLetters(str1, str2) ;
    printf("%d\n", amount);

    // Frees dynamic memory 
    free(str1);
    free(str2);

    return 0;
}