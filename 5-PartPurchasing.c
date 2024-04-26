//Nathan Hicks
//Unveils and orders parts to fix broken ship

#include <stdio.h>
#include <stdlib.h>

int total = 0; //Total money spent

typedef struct Part Part;
typedef struct Component Component;
typedef struct Queue Queue;

struct Part {
    int count; // The number of this part available
    int extra_part; // The additional part that comes when purchasing
    int price; // The price of purchasing the part
};
struct Component {
    int status; // Is the component known to the repair crew
    int part_req; // The part required to fix
    int num_comp_rev; // Number of components revealed when fixed
    int * comp_rev; // Array of revealed components when fixed
};
struct Queue {
    int component;
    Queue * next;
};

//Buys part and adds to list, also increases total
void buyPart(Part *partsList, int part);
//Inserts tail to linked list
Queue * enqueue(Queue * q, int component);
//Removes head of linked list
Queue * dequeue(Queue * q);
//Creates a queue of all components related to each other
Queue * fixLine(Queue * q, Component * componentList, Part * partsList, int comp);
//Purchases and fixes all components in correct order
Queue * purchaseAndRemove(Queue * q, Component * componentList, Part * partsList);

int main()
{
    int partNumber, componentNumber; //Number of parts and components inputted

    scanf(" %d", &partNumber);

    Part partsList[partNumber]; //List of parts inventory

    //Initializes parts list based on input
    for (int i = 0; i < partNumber; i++){
        int price, extra_part;

        scanf(" %d %d", &price, &extra_part);
        extra_part -= 1;

        partsList[i].count = 0;
        partsList[i].price = price;
        partsList[i].extra_part = extra_part;
    }

    scanf(" %d", &componentNumber);

    Component componentList[componentNumber]; //List of component details

    //Ininitalizes component list and relations to other components
    //from input
    for (int i = 0; i < componentNumber; i++){
        int partReq, numRev, partRev;
        scanf(" %d %d", &partReq, &numRev);

        componentList[i].status = 0;
        componentList[i].part_req = partReq - 1;
        componentList[i].num_comp_rev = numRev;
        componentList[i].comp_rev = (int *) malloc(sizeof(int) * numRev);


        for (int j = 0; j < numRev; j++){
            scanf(" %d", &partRev);
            componentList[i].comp_rev[j] = partRev - 1;
        }
    }

    Queue *q = NULL; //creates the Queue

    //Loops through and adds components unrelated to each other to queue
    for (int i = 0; i < componentNumber; i++){
        if (componentList[i].status == 0) {
            q = fixLine(q, componentList, partsList, i);
        }
    }

    //Loops until queue is empty and all components are fixed
    while (q != NULL){
        q = purchaseAndRemove(q, componentList, partsList);
    }

    printf("%d\n", total); //prints total

    //Frees dynamic array of component relationships
    for (int i = 0; i < componentNumber; i++)
        free(componentList[i].comp_rev);

    //Exits successful with one new boat ready to go
    return 0;
}

void buyPart(Part *partsList, int part){
    partsList[part].count += 1;
    partsList[partsList[part].extra_part].count += 1;
    total += partsList[part].price;
}

Queue * enqueue(Queue * q, int component) {
    //Dynamically creates newTail and initializes components
    Queue * newTail = (Queue *) malloc(sizeof(Queue));
    newTail->component = component;
    newTail->next = newTail;

    // Empty list case
    if (q == NULL) {
        return newTail;
    }
    // At least 1 node case
    // The resulting tail's next should point to old head
    newTail->next = q->next;

    // The new tail should follow after the old tail
    q->next = newTail;
    // Return the resulting tail
    return newTail;
}

Queue * dequeue(Queue * q) {
    // Empty list case
    if (q == NULL) {
        return NULL;
    }
    // Single node list case
    if (q->next == q) {
        free(q);
        return NULL;
    }
    // More than 1 node case
    // Get the new head
    Queue * newHead = q->next->next;
    // Free the oldhead
    free(q->next);
    // Link the tail to the head
    q->next = newHead;
    // Return the tail
    return q;

}

Queue * fixLine(Queue * q, Component * componentList, Part * partsList, int comp) {
    //Adds component to queue if it hasn't been found yet
    if (componentList[comp].status == 0){
        q = enqueue(q, comp);
    
    //Component has been found before case
    } else {
        return q;
    }

    componentList[comp].status = 1; //Sets component to found

    //Component reveals no further components case
    if (componentList[comp].num_comp_rev == 0){
        return q;
    }
    
    //Recurses for all components revealed when parent component fixed
    for (int i = 0; i < componentList[comp].num_comp_rev; i++){
        q = fixLine(q, componentList, partsList, componentList[comp].comp_rev[i]);
    }

    //Returns queue
    return q;
}

Queue * purchaseAndRemove(Queue * q, Component * componentList, Part * partsList){
    //Buys parts if not in inventory
    if (partsList[componentList[q->next->component].part_req].count == 0)
        buyPart(partsList, componentList[q->next->component].part_req);
    
    //Uses part to fix component
    partsList[componentList[q->next->component].part_req].count -= 1;

    //Checks component off broken list
    return dequeue(q);
}

