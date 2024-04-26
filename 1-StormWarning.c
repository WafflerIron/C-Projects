//Nathan Hicks
//Warns ships in a torus plan of incoming storms

#include <stdio.h>
#include <stdlib.h>

#define NAME_LEN 20 //Max name of boat

//Struct for boat and array lists of boats
typedef struct Boat Boat;
typedef struct ArrayList ArrayList;
struct Boat
{
    int start_x, start_y, delta_x, delta_y, start_time;
    char name[NAME_LEN + 1];
};
struct ArrayList
{
    struct Boat *array;
    int size, capacity;
};

// Given a list and a boat to add to it, add the boat to the end
// of the list
// Expands the list if necessary
void appendToList(ArrayList *list, Boat newBoat);

//Creates storm with set width and heighth at an x and y
//Prints ships caught in that storm
void query(int x, int y, int storm_width, int storm_height,
            int world_width, int world_height,
            ArrayList *rows, ArrayList *columns, int cur_time);

// The main function
int main()
{
    // Read the size of the world
    int height, width;
    scanf("%d %d", &width, &height);

    // Create the array of array lists for x and y world segments
    ArrayList * rows = (ArrayList *)calloc(height, sizeof(ArrayList));
    ArrayList * columns = (ArrayList *)calloc(width, sizeof(ArrayList));

    // Loop through and initialize all the array lists from the two arrays
    for (int i = 0; i < height; i++){
        rows[i].array = (Boat *)calloc(1, sizeof(Boat));
        rows[i].capacity = 1;
        rows[i].size = 0;
    }
    for (int i = 0; i < width; i++){
        columns[i].array = (Boat *)calloc(1, sizeof(Boat));
        columns[i].capacity = 1;
        columns[i].size = 0;
    }

    // Initialize the current time
    int time = 0;
    int command = 0; //Users choice of command

    //Loops until exit command
    while (command != 4){
        scanf("%d", &command);

        //Command 1 adds boat at x and y in a direction with a name
        if (command == 1){
            char d;
            char n[NAME_LEN + 1];
            Boat newBoat;

            scanf("%d %d %c %s", &newBoat.start_x, &newBoat.start_y, &d, newBoat.name);

            //Aligns inputs with array indexing
            newBoat.start_x--;
            newBoat.start_y--;
            newBoat.start_time = time;

            //Determines direction
            switch (d){
            case 'L':
                newBoat.delta_x = -1;
                newBoat.delta_y = 0;
                break;
            case 'R':
                newBoat.delta_x = 1;
                newBoat.delta_y = 0;
                break;
            case 'D':
                newBoat.delta_y = -1;
                newBoat.delta_x = 0;
                break;
            case 'U':
                newBoat.delta_y = 1;
                newBoat.delta_x = 0;
                break;
            }
            //Decides what array list to append to based on direction
            if (newBoat.delta_x != 0)
                appendToList(rows, newBoat);
            else
                appendToList(columns, newBoat);

        //Command 2 advances time
        } else if (command == 2){
            int deltaTime;

            scanf("%d", &deltaTime);
            time += deltaTime;

        //Command 3 Creates storm
        } else if (command == 3){
            int x, y, w, h;

            scanf("%d %d %d %d", &x, &y, &w, &h);
            x--;
            y--;

            query(x, y, w - 1, h - 1, width - 1, height - 1, rows, columns, time);
        } 
    }

    //Frees memory and exits
    for (int i = 0; i < height; i++){
        free(rows[i].array);
    }
    for (int i = 0; i < width; i++){
        free(columns[i].array);
    }
    free(rows);
    free(columns);

    return 0;
}

// Given a list and a boat to add to it, add the boat to the end
// of the list
// Expands the list if necessary
void appendToList(ArrayList *list, Boat newBoat){
    int index; //Either x or y index
    
    if (newBoat.delta_x != 0)
        index = newBoat.start_y;
    else
        index = newBoat.start_x;
    
    //Doubles size of array list if full
    if (list[index].size == list[index].capacity){
        list[index].capacity *= 2;
        list[index].array = (Boat *)realloc(list[index].array, sizeof(Boat) * list[index].capacity);
    }

    //Adds new boat to array list and increments size
    list[index].array[list[index].size] = newBoat;
     
    list[index].size++;
}

//Creates storm with set width and heighth at an x and y
//Prints ships caught in that storm
void query(int x, int y, int storm_width, int storm_height,
            int world_width, int world_height,
            ArrayList *rows, ArrayList *columns, int cur_time){

    int wrapX = 0, wrapY = 0; //How far off the map a storm goes
    int total = 0; //Total number of ships in storm

    //Determines how much of storm needs to wrap around
    if (x + storm_width > world_width){
        wrapX = (x + storm_width) - world_width;
    }
    if (y + storm_height > world_height){
        wrapY = (y + storm_height) - world_height;
    }
    //Saves x and y location of storm for later
    int staticX = x;
    int staticY = y;
    
    //Loops once to count ships in storm and loops twice to print them
    for (int h = 0; h < 2; h++)
    {   
        //Breaks, doesnt need to loop twice if no ships in storm
        if (h == 1 && total == 0)
            break;

        //Loops to count ships in storm for each direction storm wraps around plane
        for (int k = 0; k < 4; k++)
        {
            x = staticX;
            y = staticY;
            //Adjusts dimensions and location of storm based on wrapping
            if (k == 1 && wrapX != 0)
            {
                x = 0;
                storm_width = wrapX;
            }
            if (k == 1 && wrapY != 0)
            {
                y = 0;
                storm_height = wrapY;
            }
            if (k == 1 && wrapX == 0 && wrapY == 0)
                break;

            if (k == 2)
            {
                y = staticY;
                x = 0;
                storm_width = wrapX;
            }
            if (k == 3)
            {
                x = staticX;
                y = 0;
                storm_height = wrapY;
            }
            //Loops through columns affected by storm
            for (int i = x; i <= (x + storm_width) - wrapX; i++)
            {
                //Loops through ships in column
                for (int j = 0; j < columns[i].size; j++)
                {
                    //Finds postion of boat and time and sees if its in storm
                    int shipPos = 0;
                    shipPos = columns[i].array[j].start_y +
                                  (columns[i].array[j].delta_y * (cur_time - columns[i].array[j].start_time));
                    
                    if (shipPos > world_height)
                        shipPos = (shipPos % world_height) - 1;
                    else if (shipPos < 0)
                        shipPos = (shipPos % world_height) + world_height + 1;
                    
                    if (y <= shipPos && shipPos <= (y + storm_height) - wrapY)
                    {
                        //Prints on loop's second iteration
                        if (h == 1)
                            printf("%s\n", columns[i].array[j].name);
                        total++;
                    }
                }
            }
            //Loops through rows caught in storm
            for (int i = y; i <= (y + storm_height) - wrapY; i++)
            {
                //Loops through boats in row
                for (int j = 0; j < rows[i].size; j++)
                {
                    //Finds postion of boat and time and sees if its in storm
                    int shipPos = rows[i].array[j].start_x +
                                  (rows[i].array[j].delta_x * (cur_time - rows[i].array[j].start_time));

                    if (shipPos > world_width)
                        shipPos = (shipPos % world_width) - 1;
                    else if (shipPos < 0)
                        shipPos = (shipPos % world_width) + world_width + 1;

                    if (x <= shipPos && shipPos <= (x + storm_width) - wrapX)
                    {
                        //Prints on loop's second iteration
                        if (h == 1)
                            printf("%s\n", rows[i].array[j].name);
                        total++;
                    }
                }
            }
        }
        //Prints total on first iteration of loop
        if (h == 0)
        printf("%d\n", total);
    }  
}