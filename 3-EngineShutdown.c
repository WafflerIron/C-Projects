//Nathan Hicks
//Assignment 3 Engine Shutdown
//Permutes all possible actions to raise and lower engine temperature
//Then outputs best order to decrease the temperature the most

// Includes
#include <stdio.h>

//Max number of actions that can be taken
#define MAX_ACTIONS 11

//Global Variables
int iniActions[MAX_ACTIONS], bestActions[MAX_ACTIONS]; //Initial actions and best order of those actions
int nextActions[MAX_ACTIONS][MAX_ACTIONS]; //Array of next possible actions
int currentTemp, bestTemp = 100000 * 11 + 1; //Current temp and best temperature found
int actions; //Number of actions

// The brute force permutation generator
void permute(int * used, 
             int * perm, 
             int index // alternatively use value
             ) 
{
    // Check if the index is invalid
    if (actions == index) {
        currentTemp = 0; //Set current temperature to 0
        for (int i = 0; i < actions; i++) {

            //Add the first action taken to current temp
            currentTemp += iniActions[perm[i]];

            //Adds subsequent possible actions that can be taken based on order
            for (int j = i + 1; j < actions; j++)
                currentTemp += nextActions[perm[j]][perm[i]];
            
        }  

        //If a new best temperature is found, saves it and the order
        //used to obtain it
        if(currentTemp < bestTemp){
            bestTemp = currentTemp;
            for (int i = 0; i < actions; i++)
                bestActions[i] = perm[i];

        }
        return; // prevent calling recursively
    }
    
    // Alternatively loop over index
    for (int curValue = 0; 
         curValue < actions; 
         curValue++)
    {
        // Check if the value is not allowed
        if (used[curValue]) continue; // skip
        
        // Use the value
        used[curValue] = 1;
        perm[index] = curValue;
        // Recurse
        permute(used, perm, index + 1);
    
        // Unuse the value
        used[curValue] = 0;
    } 
}

// The main function
int main() {

    //User inputs number of actions to be taken
    scanf("%d", &actions);

    int used[actions]; // 0 or 1 representing which values have been used
    int perm[actions]; // The array to hold the values in the permutation

    //Assigns initial actions temperature change
    for (int i = 0; i < actions; i++)
        scanf(" %d", &iniActions[i]);

    //Assigns subsequent actions temperature change based on order
    for (int i = 0; i < actions; i++)
        for (int j = 0; j < actions; j++)
            scanf(" %d", &nextActions[i][j]);

    //Zeroes out the diagonal since it is unused
    for (int i = 0; i < actions; i++)
        nextActions[i][i] = 0;
    
    // Initialize the array
    for (int i = 0; i < actions; i++) {
        used[i] = 0;
    }
    
    // Recurse through all the permutations
    permute(used, perm, 0);
    
    //Print best order found to hopefully put out the fire
    for (int i = 0; i < actions; i++)
        printf("%d ", bestActions[i] + 1);
    printf("\n");

    //Ship is saved! Exit program
    return 0;
}