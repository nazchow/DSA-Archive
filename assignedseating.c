// Naziat Chowdhury
// 9/5/23
// Assignment P1
// COP3502C - Guha

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Defines constants
#define INITSIZE 10
#define MAXLEN 50
#define MAXROWS 100000

// Defines order struct
typedef struct order {
    int s_seat;
    int e_seat;
    char *name;
} order;

// Defines theaterrow struct
typedef struct theaterrow {
    order **list_orders;
    int max_size;
    int cur_size;
} theaterrow;

// Returns a pointer to an allocated order storing start seat, end seat, and name. 
// Memory allocated for order structure and for the name member (size using strlen function) and start and end seats initilized 
// This_name copied into name
order* make_order(int start, int end, char* this_name) {
    order *n_order = (order*)malloc(sizeof(order));
    n_order->s_seat = start;
    n_order->e_seat = end;
    n_order->name = (char*)malloc((strlen(this_name) + 1) * sizeof(char));
    strcpy(n_order->name, this_name);
    return n_order;
}

// Allocates  memory for one theaterrow, array of orders to a default max size  10, and sets current size to 0
// Also checks if memory allocation is successful, if successful returns a pointer to theaterrow
theaterrow* make_empty_row() {
    theaterrow *n_row = (theaterrow*)malloc(sizeof(theaterrow));
    if (n_row == NULL) {
        return NULL; 
    }
    n_row->list_orders = (order**)malloc(INITSIZE * sizeof(order*));
    if (n_row->list_orders == NULL) {
        return NULL; 
    }
    n_row->max_size = INITSIZE;
    n_row->cur_size = 0;
    return n_row;
}
// Check if order 1 and 2 ranges conflict, returns 1 if they share at least 1 seat in common and 0 otherwise
int conflict(order* order1, order* order2) {
    if (order1->e_seat >= order2->s_seat && order1->s_seat <= order2->e_seat) {
        return 1; 
    } 
    else {
        return 0;
    }
}
// Returns 1 if the order pointed to by this_order can be added to the row pointed to by this_row. 
// 1 returned if this_order has no seats in it part of any order on this_row
int can_add_order(theaterrow* this_row, order* this_order) {
    for (int i = 0; i < this_row->cur_size; i++) {
        if (conflict(this_order, this_row->list_orders[i])) {
            return 0; 
        }
    }
    return 1;
}
// Tries to add this_order to this_row. If successful, the order is added and 1 is returned. Otherwise, 0 is returned and no change is made. 
// If memory for this_row is full,the function doubles the max # of orders allocated
// Adjusts max_size and cur_size of this_row
int add_order(theaterrow* this_row, order* this_order) {
    if (can_add_order(this_row, this_order) == 0) {
    return 0;
}
    if (this_row->cur_size == this_row->max_size) {
        this_row->max_size *= 2;
        this_row->list_orders = (order**)realloc(this_row->list_orders, this_row->max_size * sizeof(order*));
        if (this_row->list_orders == NULL) {
            return 0;
        }
    }

    this_row->list_orders[this_row->cur_size++] = this_order;
    return 1; 
}
// If seat number in row number row is occupied, a pointer is returend to the owners name, otherwise NULL is returned (seat is unowned)
char* get_owner(theaterrow** theater, int row, int seat_num) {
    for (int i = 0; i <= row; i++) {
        if (theater[i] != NULL && seat_num >= theater[i]->list_orders[0]->s_seat && seat_num <= theater[i]->list_orders[0]->e_seat) {
            for (int j = 0; j < theater[i]->cur_size; j++) {
                if (seat_num >= theater[i]->list_orders[j]->s_seat && seat_num <= theater[i]->list_orders[j]->e_seat) {
                    return theater[i]->list_orders[j]->name;
                }
            }
        }
    }
    return NULL; 
}
// if seat_num in the row pointed to by this_row is occupied, a pointer is returned to the string storing the owner’s name. NULL returned if seat is unowned
char* get_row_owner(theaterrow* this_row, int seat_num) {
    for (int i = 0; i < this_row->cur_size; i++) {
        if (seat_num >= this_row->list_orders[i]->s_seat && seat_num <= this_row->list_orders[i]->e_seat) {
            return this_row->list_orders[i]->name;
        }
    }
    return NULL; 
}
// Returns 1 if the seat number is in the range of seats pointed to by myorder, returns 0 if outside the range
int contains(order* myorder, int seat_no) {
    if (seat_no >= myorder->s_seat && seat_no <= myorder->e_seat) {
        return 1; 
    } 
    else 
    {
        return 0; 
    }
}
// Frees memory allocated for the char array in this_order then the order struct itself
void free_order(order* this_order) {
    free(this_order->name);
    free(this_order);
}
// Iterates through list_orders array, frees all memory associated with this_row
void free_row(theaterrow* this_row) {
    for (int i = 0; i < this_row->cur_size; i++) {
        free_order(this_row->list_orders[i]);
    }
    free(this_row->list_orders);
    free(this_row);
}

int main() {
    // Allocates array where every element is a pointer to the theatre row, if statement checks if enough memory was allocated 
    theaterrow **amc_theatre = (theaterrow**)calloc(MAXROWS + 1, sizeof(theaterrow*));
    if (amc_theatre == NULL) {
        return 0; 
    }
    // Declares array with the maximum length - stores the input read from commmands
    char arr[MAXLEN];

    // Sentinel controlled loop, will run until QUIT is inputted - strcmp compares string inputted to "QUIT"
    while (1) {
        scanf("%s", arr);
        if (strcmp(arr, "QUIT") == 0) {
            break;
        }
        // If statement that compares the string inputted to "BUY" and then runs the BUY commands 
        // Reads in input, stores the row number, start and ending seat, and name
        if (strcmp(arr, "BUY") == 0) {
            int row, start, end;
            char name[MAXLEN];
            scanf("%d %d %d %s", &row, &start, &end, name);

            // Function call to allocate memory for the movie theatre row
            if (amc_theatre[row] == NULL) {
                amc_theatre[row] = make_empty_row();
            }
            // Initialized order function with the inputted seats and name
            // Prints success if order is added, failure if there is a conflict (free order is called)
            // If
            order *n_order = make_order(start, end, name);
            if (add_order(amc_theatre[row], n_order)) {
                printf("SUCCESS\n");
            } 
            else {
                printf("FAILURE\n");
                free_order(n_order);
            }
        } 
        // If statement that compares the string inputted to "LOOKUP" and then runs the LOOKUP commands
        // Reads in input, stores the row and seat
        // If the owner is found then the name is printed
        else if (strcmp(arr, "LOOKUP") == 0) {
            int row, seat;
            scanf("%d %d", &row, &seat);
            char *owner = get_owner(amc_theatre, row, seat);
            if (owner != NULL) {
                printf("%s\n", owner);
            } 
            else {
                printf("No one.\n");
            }
        }
    }
    // Loop iterates through 0 to MAXROWS, if a row in the array is allocated its pointer will not be null
    // Free row called to free the orders in the row
    for (int i = 0; i <= MAXROWS; i++) {
        if (amc_theatre[i] != NULL) {
            free_row(amc_theatre[i]);
        }
    }
    // Frees amc theatre array 
    free(amc_theatre);

    return 0;
}
