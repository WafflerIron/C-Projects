#include <stdio.h>
#include <string.h>

//Macro constants to limit input
#define MAX_NUMBER_LINES 100
#define MAX_SHIP_NUMBER 100
#define MAX_NAME_LENGTH 20
#define MAX_CUSTOMERS 100000

//Function Prototypes
void nameInputs(int currentLine, char input[MAX_NAME_LENGTH * MAX_SHIP_NUMBER + 99 + 2], char names[MAX_NUMBER_LINES][MAX_SHIP_NUMBER + 1][MAX_NAME_LENGTH + 1]); //Names are seperated into their own arrays
void weekOutput(int numberOfLines, int weekNumber, char names[MAX_NUMBER_LINES][MAX_SHIP_NUMBER + 1][MAX_NAME_LENGTH + 1]); //Prints first ship on each line for specified week

int main()
{
    //Main array that stores all the information about the boat lines
    char names[MAX_NUMBER_LINES][MAX_SHIP_NUMBER + 1][MAX_NAME_LENGTH + 1];

    int numberOfLines = 0; //Number of boat lines
    int numberOfCustomers = 0; //Number of customers requesting line orders
    int weekNumber = 0; //Week requested for line order
    
    char shipName[MAX_NAME_LENGTH * MAX_SHIP_NUMBER + 2]; //Ship names seperated by a space
    char trash[3]; //Catches newline character

    //Loop ensures value is between speciied min and max
    while(!(numberOfLines >= 1 && numberOfLines <= MAX_NUMBER_LINES)){
        //User inputs number of boat lines and fgets catches newline
        scanf(" %d", &numberOfLines);
        fgets(trash, 10, stdin);
    }

    //Loops for user to input boat names belonging to each line
    for (int i = 0; i < numberOfLines; i++){
        //User inputs boat names and they are assigned to the main array
        fgets(shipName, MAX_NAME_LENGTH * MAX_SHIP_NUMBER + 99 + 2, stdin);
        nameInputs(i, shipName, names);   
    }

    //Loop ensures value is between speciied min and max
    while(!(numberOfCustomers >= 1 && numberOfCustomers <= MAX_CUSTOMERS)){
        //User inputs number of customers and fgets catches newline
        scanf(" %d", &numberOfCustomers);
        fgets(trash, 10, stdin);
    }

    //Runs for each customer
    for (int i = 0; i < numberOfCustomers; i++){
        //Loop ensures value is between speciied min and max
        while(!(weekNumber >= 1 && weekNumber <= MAX_CUSTOMERS)){
            //User inputs week to be checked and fgets catches newline
            scanf(" %d", &weekNumber);
            fgets(trash, 10, stdin);
        }

        //Prints info about week entered and then resets week to 0 for input validation
        weekOutput(numberOfLines, weekNumber, names);
        weekNumber = 0;
    }

    return 0;
}

//Function Definitions

//Names are seperated into their own arrays
void nameInputs(int currentLine, char input[MAX_NAME_LENGTH * MAX_SHIP_NUMBER + 99 + 2], char names[MAX_NUMBER_LINES][MAX_SHIP_NUMBER + 1][MAX_NAME_LENGTH + 1]){
    //Current ship and specific letter being transfered to main array
    int shipNumber = 0;
    int currentLetter = 0;
    int length = strlen(input);

    //Loops through input seperating names by spacing and breaking when newline is reached
    for(int i = 0; i < length; i++){
        //printf("%c\n", input[i]);
        if(input[i] == '\n'){
            names[currentLine][shipNumber + 1][0] = '\0'; //Used for counting number of ships in line 
            break;
        }
        else if(input[i] == ' '){
            shipNumber++;
            currentLetter = 0;
        }
        else{
            names[currentLine][shipNumber][currentLetter] = input[i];
            currentLetter++;
        }
    }
}

//Prints first ship on each line for specified week
void weekOutput(int numberOfLines, int weekNumber, char names[MAX_NUMBER_LINES][MAX_SHIP_NUMBER + 1][MAX_NAME_LENGTH + 1]){
    int numberOfShips; //Number of ships in a line

    weekNumber--; //Aligns weekNumber with array indices

    //Loops through each line printing first ship based on week
    for(int i = 0; i < numberOfLines; i++){
        numberOfShips = 0;

        //Counts number of ships in line
        while(names[i][numberOfShips][0] != '\0')
            numberOfShips++;

        //Prints first ship on the line for current cycle
        if (weekNumber >= numberOfShips)
            printf("%s ", names[i][weekNumber % numberOfShips]);
        else
            printf("%s ", names[i][weekNumber]);
    }

    printf("\n");
}