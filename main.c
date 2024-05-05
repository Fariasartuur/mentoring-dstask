#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DESCRIPTION_SIZE 100

typedef struct Task {
    int id;
    char description[MAX_DESCRIPTION_SIZE];
    struct Task* prox;
} TASK;

typedef struct {
    TASK *head;
    TASK *tail;
} Queue;

typedef struct {
    TASK *top;
} Stack;

typedef struct {
    TASK *head;
} List;

Queue pending = {NULL, NULL};
List completed = {NULL};
Stack draft = {NULL};

// CONSTRUCTOR
TASK* createTask(int id, char* description);
TASK* createTaskByScanf();

// LIST
void addToCompletedList(TASK* newTask);
TASK* removeFromCompletedListByItsId(int id);
void seeAllCompletedList();

// QUEUE
void putToPendingQueue(TASK* newTask);
TASK* getFromPendingQueue();
void seeAllPendingQueue();

// STACK
void pushToDraftStack(TASK* newTask);
TASK* popFromDraftStack();
void seeAllDraftStack();

// MENU
void displayMenu();

int main() {
    int choice;
    int id;
    TASK* newTask = NULL;
    TASK* firstTask = NULL;
    TASK* task = NULL;
    TASK* lastTask = NULL;
    printf("################# TASK MANAGER SYSTEM #################");

    do {
        displayMenu();
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // CREATE A TASK, THEN ADD TO PENDING QUEUE
                newTask = createTaskByScanf();
                putToPendingQueue(newTask);
                break;

            case 2:
                // SEE ALL TASKS FROM PENDING QUEUE
                seeAllPendingQueue();
                break;

            case 3:
                // COMPLETE FIRST PENDING TASK
                firstTask = getFromPendingQueue();
                if(firstTask != NULL) {
                    addToCompletedList(firstTask);
                }
                break;

            case 4:
                // SEE ALL TASKS FROM COMPLETED LIST
                seeAllCompletedList();
                break;

            case 5:
                // SET COMPLETED TASK TO DRAFT
                printf("Enter Task ID (number): ");
                scanf("%d", &id);

                task = removeFromCompletedListByItsId(id);
                if(task != NULL) {
                    pushToDraftStack(task);
                }
                break;

            case 6:
                // SEE ALL TASKS FROM DRAFT STACK
                seeAllDraftStack();
                break;

            case 7:
                // SET LAST DRAFT AS PENDING TASK
                lastTask = popFromDraftStack();
                if(lastTask != NULL) {
                    putToPendingQueue(lastTask);
                }
                break;

            case 0:
                printf("Exiting program\n");
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
    } while (choice != 0);

    printf("################# SYSTEM SHUT DOWN #################");
}

int duplicatedId(int id) {
    TASK* temp;

    // Pending Queue
    temp = pending.head;
    while (temp != NULL) {
        if (temp->id == id) return 1;
        temp = temp->prox;
    }

    // Completed List
    temp = completed.head;
    while (temp != NULL) {
        if (temp->id == id) return 1;
        temp = temp->prox;
    }

    // Draft Stack
    temp = draft.top;
    while (temp != NULL) {
        if (temp->id == id) return 1;
        temp = temp->prox;
    }

    return 0;
}

// CONSTRUCTOR
TASK* createTask(int id, char* description) {
    TASK* newTask = (TASK*) malloc(sizeof(TASK));

    newTask->id = id;
    strcpy(newTask->description, description);
    newTask->prox = NULL;

    return newTask;
}

TASK* createTaskByScanf() {
    int id;
    char description[MAX_DESCRIPTION_SIZE];

    printf("Enter Task ID (number): ");
    scanf("%d", &id);

    if(duplicatedId(id)){
        printf("Error: Task ID already exists.\n");
        return NULL;
    }

    getchar();
    printf("Enter Task Description (string): ");

    fgets(description, MAX_DESCRIPTION_SIZE, stdin);
    description[strcspn(description, "\n")] = '\0';

    return createTask(id, description);
}

// LIST
void addToCompletedList(TASK* newTask) {

    if (newTask == NULL) {
        printf("Error: Cannot add NULL task to the completed list.\n");
        return;
    }

    printf("Adding Task to Completed List\n\n");

    if(completed.head == NULL){
        completed.head = newTask;
        newTask->prox = NULL;
    } else{
        newTask->prox = completed.head;
        completed.head = newTask;
    }

}

TASK* removeFromCompletedListByItsId(int id) {
    printf("Removing Task from Completed List\n");

    if(completed.head == NULL){
        printf("Queue is empty\n");
        return NULL; // Return NULL if Queue is empty
    }

    TASK* auxLR = completed.head;
    TASK* previous = NULL;
    
    while(auxLR != NULL){
        if(auxLR->id == id){
            if(previous == NULL){
                completed.head = auxLR->prox;
            } else{
                previous->prox = auxLR->prox;
            }
            auxLR->prox = NULL;
            return auxLR;
        }
        previous = auxLR;
        auxLR = auxLR->prox;
    }

    return NULL;

}

void seeAllCompletedList() {
    TASK* auxC = completed.head;
    printf("Printing All Completed List\n\n");

    while(auxC != NULL){
        printf("Task ID: %d, Description: %s\n", auxC->id, auxC->description);
        auxC = auxC->prox;
    }
}

// QUEUE
void putToPendingQueue(TASK* newTask) {

    if (newTask == NULL) {
        printf("Error: Cannot put NULL task to the pending queue.\n");
        return;
    }

    printf("Putting Task to Pending Queue\n\n");

    if(pending.head == NULL){
        pending.head = newTask;
        pending.tail = newTask;
    } else{
        pending.tail->prox = newTask;
        pending.tail = newTask;
    }
    pending.tail->prox = NULL;
}

TASK* getFromPendingQueue() {
    printf("Getting Task from Pending Queue\n\n");

    if(pending.head == NULL){
        printf("Queue is empty\n");
        return NULL; // Return NULL if Queue is empty
    }

    TASK* auxQG = pending.head;
    pending.head = pending.head->prox;

    auxQG->prox = NULL;
    return auxQG;
}

void seeAllPendingQueue() {
    TASK* auxP = pending.head;
    if(auxP == NULL)
    return;
    printf("Printing All Pending Queue\n\n");

    while(auxP != NULL){
        printf("Task ID: %d, Description: %s\n", auxP->id, auxP->description);
        auxP = auxP->prox;
    }
}

// STACK
void pushToDraftStack(TASK* newTask) {

    if (newTask == NULL) {
        printf("Error: Cannot push NULL task to the draft stack.\n");
        return;
    }

    printf("Pushing Task to Draft Stack\n");

    newTask->prox = draft.top;
    draft.top = newTask;
}

TASK* popFromDraftStack() {
    printf("Popping Task to Draft Stack\n");

    if(draft.top == NULL){
        printf("Queue is empty\n");
        return NULL; // Return NULL if Queue is empty
    }

    TASK* auxPop = draft.top;
    draft.top = draft.top->prox;

    auxPop->prox = NULL;
    return auxPop;
}

void seeAllDraftStack() {
    TASK* auxD = draft.top;
    if(auxD == NULL)
    return;
    printf("Printing All Draft Stack\n\n");

    while(auxD != NULL){
        printf("Task ID: %d, Description: %s\n", auxD->id, auxD->description);
        auxD = auxD->prox;
    }
}

// MENU
void displayMenu() {
    printf("\nMenu:\n");
    printf("1 - Create New Pending Task\n");
    printf("2 - See All Pending Tasks\n");
    printf("3 - Complete First Pending Task\n");
    printf("4 - See All Completed Tasks\n");
    printf("5 - Set Completed Task as Draft by its ID\n");
    printf("6 - See All Draft Tasks\n");
    printf("7 - Set Last Draft Task as Pending Task\n");
    printf("0 - Exit\n");
}