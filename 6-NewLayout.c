//Nathan Hicks

// Counts the number of living quarters and total distance to them
// given a room layout

// Header files
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/////// Struct definitions ///////
// Typedefs to make code cleaner
typedef struct Stack Stack;
typedef struct Node Node;
// Node of the linked list
struct Node
{
    int id; //ID of room
    int children; //0 for no children and 1 for has children
    int depth; //Depth of node
    int parentID; //ID of parent
    Node *next; //Points to next child
};
// Stack
struct Stack
{
    Node *head;
};
/////// Prototypes ///////
///// Linked List /////
Node *createNode(int id);
Node *insertHead(Node *head, int id);
Node *removeHead(Node *head);
///// Stack /////
Stack *createStack();
void push(Stack *stack, int id);
void pop(Stack *stack);
void deleteStack(Stack *stack);
/////// Functions ///////
///// Linked List /////
//Function to return an initialized node
Node *createNode(int id)
{
    Node *res = (Node *)malloc(sizeof(Node));

    res->id = id;

    res->children = 0;
    res->depth = 0;
    res->parentID = -1;

    res->next = NULL;

    return res;
}
// Function to insert a node to the head of a linked list
// Return the resulting head
Node *insertHead(Node *head, int id)
{
    // Create and link the new head
    Node *newHead = createNode(id);
    newHead->next = head;
    
    //If new head isn't the entrance, then update child's parent and depth
    //Add set parent's children equal to true
    if (head != NULL){
        newHead->parentID = head->id;
        head->children = 1;
        newHead->depth = 1 + head->depth;
    }

    // Return the resulting head
    return newHead;
}
// Function to remove the head of a given linked list
// Return the resulting head
Node *removeHead(Node *head)
{
    // Empty list case
    if (head == NULL)
    {
        return NULL;
    }
    // Prevent Use after free by storing the new head pointer
    Node *newHead = head->next;
    // Delete the old head
    free(head);
    // Return the new head
    return newHead;
}
///// Stack /////
// Function to dynamically create a stack implemented using a Linked List
Stack *createStack()
{
    Stack *res = (Stack *)malloc(sizeof(Stack));
    // Make the list empty
    res->head = NULL;
    // Return the resulting stack
    return res;
}
// Function to add an element to the top of the stack
void push(Stack *stack, int id)
{
    stack->head = insertHead(stack->head, id);
}
// Function to remove the top of a stack
void pop(Stack *stack)
{
    stack->head = removeHead(stack->head);
}
// Function to clean up the memory associated with a stack
void deleteStack(Stack *stack)
{
    // Remove the values in the stack
    while (stack->head != NULL)
        pop(stack);
    // Clean up the memory of the stack
    free(stack);
}
///// The main function /////
int main()
{
    int currentRoom; //Current room employee is in
    int quarters = 0; //Number of living quarters
    int distance = 0; //Total distance to each quarter

    scanf(" %d", &currentRoom); //Scans in starting room

    // Create stack and pushes starting room on;
    Stack *stk = createStack();
    

    while (currentRoom != -1) {
        //Checks if employee just entered the boat
        if (stk->head == NULL)
            push(stk, currentRoom);

        //If employee enters new room, push new room on stack
        else if (currentRoom != stk->head->parentID) {
            push(stk, currentRoom);

        //If employee enters previous room, check if current room has children
        //If no children then room must be quarters, add to totals
        } else {
            if (stk->head->children == 0) {
                quarters++;
                distance += stk->head->depth;
            }

            pop(stk);
        }

        scanf(" %d", &currentRoom);
    }
    
    //If the entrance is the only room, count it as a quarter;
    if (stk->head->children == 0) quarters++;

    //Prints totals
    printf("%d %d\n", quarters, distance);

    // Clean up the memory associated with the stack
    deleteStack(stk);
    // Exit
    return 0;
}