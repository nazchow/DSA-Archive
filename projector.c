// Naziat Chowdhury
// 10/25/23
// COP3502C - Guha
// P4 Projector Problems

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Defines constants
#define PI 3.14159265358979
#define TWO_PI 6.283185307179

// Struct for people w/ certain angle
typedef struct peopleGroup {
    double angle;
    int people;
} peopleGroup;

// Function merges array halfs
// Size of each half found, then temp arrays created to store the main arrays that will be merged, copied to array
merge(peopleGroup arr[], int left, int middle, int right) {
    int i, j, k;
    int firstSize = middle - left + 1;
    int secondSize = right - middle;

    peopleGroup tempLeft[firstSize], tempRight[secondSize];
    for (i = 0; i < firstSize; i++) {
        tempLeft[i] = arr[left + i];
    }
    for (j = 0; j < secondSize; j++) {
        tempRight[j] = arr[middle + 1 + j];
    }
    i = 0;
    j = 0;
    k = left;

    // Merges halfs back to the array - remaining is copied 
    while (i < firstSize && j < secondSize) {
        if (tempLeft[i].angle <= tempRight[j].angle) {
            arr[k] = tempLeft[i];
            i+=1;
        } 
        else {
            arr[k] = tempRight[j];
            j+=1;
        }
        k+=1;
    }
    while (i < firstSize) {
        arr[k] = tempLeft[i];
        i+=1;
        k+=1;
    }

    while (j < secondSize) {
        arr[k] = tempRight[j];
        j+=1;
        k+=1;
    }
}
// Function that uses the merge sort algorithm
// Middle of the array is found and the first and second halfs are sorted, then merged (recursive call)
mergeSort(peopleGroup arr[], int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);

        merge(arr, left, middle, right);
    }
}

int main() {
    // Asks and reads in input for the number of groups of people and the angles in degrees of projection
    int N, A;
    scanf("%d %d", &N, &A);

    // Memory allocation - holds both angles
    peopleGroup *groups = (peopleGroup *)malloc(N * 2 * sizeof(peopleGroup));

    // Coordinates and the number of people read in
    // Finds the angle of the coordinates
    // Group data with the angle stored in an array
    // Another group's data with the angle stored (+ 2pi to take care of wrap around angles)
    for (int i = 0; i < N; ++i){
        int x, y, p;
        scanf("%d %d %d", &x, &y, &p);
        double angle = atan2(y, x);
        groups[i] = (peopleGroup){angle, p};
        groups[i + N] = (peopleGroup){angle + TWO_PI, p};
    }

    // Sorts array by angle in ascending order
    // Angles converted to radians
    // Minimum number of people intialized to max val of positive 32 bit integer (lowered w/ data later)
    // Total number of people initialized, as well as variables to traverse array
    mergeSort(groups, 0, N * 2 - 1);
    double angle_radians = A * PI / 180.0;
    int minPeople = 2147483647;
    int totalPeople = 0;
    int i = 0, j = 0;

    // Finding min people in the angle range across the sorted array
    // If current range exceeds the angle the number of people is subtracted
    // If the total people is less than the minimum that is found then the minimum is updated
    while (i < N) {
        if (j < N * 2 && groups[j].angle - groups[i].angle < angle_radians) {
            totalPeople += groups[j].people;
            j+=1;
        }
        else {
            totalPeople -= groups[i].people;
            i+=1;
        }

        if (totalPeople < minPeople) {
            minPeople = totalPeople;
        }
    }
    // Angle gap where there is no people in the projection initiliazed
    double maxAngleNoPpl = 0.0;

    // Largest gap found by iterating through array
    // Max angle is updated if the current gap is larger then the old max val
    for (int i = 1; i < N * 2; i++) {
        double gap = groups[i].angle - groups[i - 1].angle;
        if (gap > maxAngleNoPpl) {
            maxAngleNoPpl = gap;
        }
    }

    // Fmin function used to convert the angle to rdegrees and makes sure it does not surpass 360 degrees by returning the smaller float
    maxAngleNoPpl = fmin(maxAngleNoPpl * 180.0 / PI, 360.0000);

    // Outputs the minimum number of people in the projection 
    // Outputs float point value that specifies the maximum angle that can be used such that no person will be in the projection
    printf("%d\n", minPeople);
    printf("%.4f\n", maxAngleNoPpl);

    // Frees memory
    free(groups);
    
    return 0;

}
















