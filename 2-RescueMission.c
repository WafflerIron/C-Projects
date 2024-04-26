//Nathan Hicks
//Uses binary search to locate a boat based on where it has and hasn't been

//Headers
#include<stdio.h>

//Function Protypes

//Calculates mid point of possible boat positions
//Tells satalite to take a picture at midpoint
int midPoint(int colEnd, 
             int rowEnd,
             int boats[rowEnd][colEnd], int time);

//Rules out possible boat possitions based on picture having wake, no wake, or boat
//Cuts the array of possible boat possitions about in half
int cut(int colEnd, int rowEnd,
        int boatArr[rowEnd][colEnd], int time, int midPoint);

//Main
int main()
{
    int maxSpeed, maxPos; //Maximum starting speed and position
    int time = 1; //Time of first photo taken
    char trash[2]; //Gets rid of newline

    //Reads in max speed and pos and trashes newline
    scanf("%d %d", &maxSpeed, &maxPos); 
    fgets(trash, 2, stdin);

    //Increases by one to line up with array
    maxSpeed++;
    maxPos++;

    //Creates 2D array of possible boat positions
    //Sets every boat to 1 for possible
    int possibleBoats[maxSpeed][maxPos];
    for (int i = 0; i < maxSpeed; i++)
        for (int j = 0; j < maxPos; j++)
            possibleBoats[i][j] = 1;

    //Loops until rescue team is sent
    //Calculates and guesses midpoint
    //Then cuts array in half or sends rescue team
    while (cut(maxPos, maxSpeed, possibleBoats, time, 
           midPoint(maxPos, maxSpeed, possibleBoats, time))) {

        time++; //Another hour goes by if rescue team not sent
    }

    //Exits successfully
    return 0;
}

//Function Definitions

//Calculates mid point of possible boat positions
//Tells satalite to take a picture at midpoint
int midPoint(int colEnd, 
             int rowEnd,
             int boats[rowEnd][colEnd], int time){
    
    int total = 0; //Sum of the boats possible positions
    int count = 0; //How many boats are still possible

    //Counts and sums the possible boat positions
    for (int i = 0; i < rowEnd; i++){
        for (int j = 0; j < colEnd; j++){
            if (boats[i][j] == 1){
                count++;
                total += (time * i) + j;
            }
        }
    }

    //Guesses one of the possible positions if only 2 ships left
    if (count <= 2)
    {
        for (int i = 0; i < rowEnd+1; i++){
            for (int j = 0; j < colEnd+1; j++){
                if (boats[i][j] == 1){
                    printf("? %d\n", (time * i) + j);
                    fflush(stdout);
                    return (time * i) + j; //Returns for cut function later
                }

            }
        }
    }
    //Guesses midpoint by dividing total by count
    printf("? %d\n", total / count);
    fflush(stdout);
    return total / count; //Returned for cut function later
}

//Rules out possible boat possitions based on picture having wake, no wake, or boat
//Cuts the array of possible boat possitions about in half
int cut(int colEnd, int rowEnd,
        int boatArr[rowEnd][colEnd], int time, int midPoint){
    
    //Reads in the results from satelite photo
    char wake[10];
    fgets(wake, 10, stdin);
    
    //If there is wake in photo, cut off midpoint and lower in array
    if (wake[0] == 'W'){
        for (int i = 0; i < rowEnd; i++){
            for (int j = 0; j < colEnd; j++){
                if (boatArr[i][j] == 0){
                    continue;
                }else if (((i * time) + j) <= midPoint){
                    boatArr[i][j] = 0;
                }
            }
        }
    }

    //If there is no wake, cut off midpoint and higher in array
    else if (wake[0] == 'N'){
        for (int i = 0; i < rowEnd; i++){
            for (int j = 0; j < colEnd; j++){
                if (boatArr[i][j] == 0){
                    continue;
                }else if (((i * time) + j) >= midPoint){
                    boatArr[i][j] = 0;
                }
            }
        }
    
    //If boat is found, send rescue team to it's future position
    } else if (wake[0] == 'B'){
        for (int i = 0; i < rowEnd; i++){
            for (int j = 0; j < colEnd; j++){
                if (boatArr[i][j] == 0){
                    continue;
                }else{
                    printf("! %d\n", ((i * (time+1)) + j));
                    fflush(stdout);
                    return 0; //Returns 0, breaking while loop in main
                              //and allowing program to exit
                }
            }
        }
    }
    
    return 1; //Keeps program running, while loop stays true
}