//Nathan Hicks
//Given a coodinate plane with a ship, harbor, and tugboats
//Prints order tugboats should take ship to harbor

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Struct for all the necassary elements of the tugboat
typedef struct Tugboat Tugboat;
struct Tugboat {
    long long int tugX, tugY, tug_harbor, tug_tug;
    char name [20 + 1];
    int valid;
};

//Sorts the tugboats
void mergeSort(Tugboat * arr, long long int size);

// Dynamically create an array of the two subarrays merged
Tugboat * merge(Tugboat * first, long long int firstSize, 
             Tugboat * second, long long int secondSize);

//Compares the elements of two tugboats
int compare(Tugboat first, Tugboat second);

//Prints valid boats
void printArray(Tugboat * array, long long int size);


int main() {
    //Initial coords for ship and harbor and number of tugboats being input
    long long int shipX, shipY, harborX, harborY, tugBoatTotal;

    //Scans in coords and zeroes out harbor with ship
    scanf("%lld %lld %lld %lld", &shipX, &shipY, &harborX, &harborY);
    harborX -= shipX;
    harborY -= shipY;

    scanf("%lld", &tugBoatTotal);

    Tugboat boats[tugBoatTotal]; //Array of tugboats to be initialized and sorted

    //Initializes valid tugboats
    for (int i = 0; i < tugBoatTotal; i++){
        long long int tugX, tugY, tug_harbor, tug_tug;
        char name[20 + 1];

        scanf("%lld %lld %s", &tugX, &tugY, name);
        
        //Zeroes out tugboat coords and calculates vectors
        tugX -= shipX;
        tugY -= shipY;
        tug_harbor = (tugX * harborX) + (tugY * harborY);
        tug_tug = (tugX * tugX) + (tugY * tugY);
        
        //Does not read in a tugboat if past harbor or behind ship
        if ((tug_harbor <= 0) || 
            (tug_harbor > (harborX * harborX) + (harborY * harborY))){
            
            tugBoatTotal--;
            i--;
        } else {
            //Initializing values inputted
            boats[i].tugX = tugX;
            boats[i].tugY = tugY;
            boats[i].tug_harbor = tug_harbor;
            boats[i].tug_tug = tug_tug;
            boats[i].valid = 1;
            strcpy(boats[i].name, name);
        }
    }
    
    //Sorts array and prints valid boats
    mergeSort(boats, tugBoatTotal);
    printArray(boats, tugBoatTotal);
  
    //Exits successfully with no errors or memory leaks (hopefully)
    return 0;
}

//Merge sorts the arry
void mergeSort(Tugboat * arr, long long int size) {
    //Base case
    if (size <= 1) return;

    // Getting the sizes of the two halves
    long long int sizeLeft = size / 2;
    long long int sizeRight = size - sizeLeft;

    // Get the pointers into the two halves
    Tugboat * leftArr = arr;
    Tugboat * rightArr = &arr[sizeLeft];

    // Sort the left side
    mergeSort(leftArr, sizeLeft);

    // Sort the right side
    mergeSort(rightArr, sizeRight);
    Tugboat * result = merge(leftArr, sizeLeft, 
                         rightArr, sizeRight);
    
    // Copy result into arr
    for (int i = 0; i < size; i++)
        arr[i] = result[i];
    
    //Free used memory
    free(result); 
}

//Compares elements of two tugboats
int compare(Tugboat first, Tugboat second){
    //Compares distance from ship along main vector of two tugboats
    if (first.tug_harbor < second.tug_harbor) return 1;
    else if (first.tug_harbor > second.tug_harbor) return 0;
    //Compares distance from the main vector
    else if (first.tug_tug < second.tug_tug) return 1;
    else if (first.tug_tug > second.tug_tug) return 0;
    //Compares names
    else if (strcmp(first.name, second.name) < 0) return 1;
    else return 0;
    
}

//Dynamically create an array of the two subarrays merged
Tugboat * merge (Tugboat * first, long long int firstSize, 
            Tugboat * second, long long int secondSize) {
    
    long long int total = firstSize + secondSize;
    Tugboat * result = (Tugboat *) calloc(total, sizeof(Tugboat));
    
    long long int firstPtr = 0;
    long long int secondPtr = 0;
    for (int i = 0; i < total; i++) {
        if (firstPtr == firstSize) {
            // Empty first array
            result[i] = second[secondPtr];
            secondPtr++;
        } else if (secondPtr == secondSize) {
            // Empty second array
            result[i] = first[firstPtr];
            firstPtr++;
        } else if (compare(first[firstPtr], second[secondPtr])) {
            // First is smaller
            if (first[firstPtr].tug_harbor == second[secondPtr].tug_harbor)
                second[secondPtr].valid = 0; //if distance from ship equal don't print
                                             //loser of tiebreaker
            result[i] = first[firstPtr];
            firstPtr++;
        } else {
            // Second was equal or smaller
            if (first[firstPtr].tug_harbor == second[secondPtr].tug_harbor)
                first[firstPtr].valid = 0; //if distance from ship equal don't print
                                           //loser of tiebreaker
            result[i] = second[secondPtr];
            secondPtr++;
        }
    }   
    return result;
}

//Prints valid tugboats
void printArray(Tugboat * array, long long int size){

    int counter = 0; //number of valid tugboats

    //Counts how many tugboats will be printed, and prints counter
    for (int i = 0; i < size; i++){
        if (array[i].valid)
            counter++;
    }
    printf("%d\n", counter);

    //Prints all valid tugboats
    for (int i = 0; i < size; i++){
        if (array[i].valid)
            printf("%s\n", array[i].name);
    }
}
