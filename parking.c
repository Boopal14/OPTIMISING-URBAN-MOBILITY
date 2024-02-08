#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SPACES 10
#define HOURLY_RATE 10
#define HALF_HOUR_RATE 5

// Node structure for a linked list representing the parking spaces
struct Node {
    int spaceId;
    char ownerName[50];
    char carNumber[15];
    time_t timeOfParking;
    struct Node* next;
};

// Parking lot data structure
struct ParkingLot {
    struct Node* front;  // Front of the queue
    struct Node* rear;   // Rear of the queue
};

// Function to initialize the parking lot
void initializeParkingLot(struct ParkingLot *lot) {
    lot->front = lot->rear = NULL;  // Initialize an empty queue
}

// Function to check if the parking lot is empty
bool isParkingLotEmpty(struct ParkingLot *lot) {
    return lot->front == NULL;
}

// Function to display the parking lot status
void displayParkingLot(struct ParkingLot *lot) {
    printf("Parking Lot Status:\n");

    if (isParkingLotEmpty(lot)) {
        printf("No cars parked\n");
        return;
    }

    struct Node* current = lot->front;
    while (current != NULL) {
        printf("Space %d: Occupied\n", current->spaceId);
        printf("   Owner Name: %s\n", current->ownerName);
        printf("   Car Number: %s\n", current->carNumber);
        printf("   Time of Parking: %s", ctime(&(current->timeOfParking))); // ctime adds newline

        current = current->next;
    }
}

// Function to park a car
void parkCar(struct ParkingLot *lot) {
    int spaceId = lot->rear ? lot->rear->spaceId + 1 : 1;

    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->spaceId = spaceId;

    printf("Enter Owner Name: ");
    scanf("%s", newNode->ownerName);

    printf("Enter Car Number: ");
    scanf("%s", newNode->carNumber);

    newNode->timeOfParking = time(NULL);  // Get current time

    newNode->next = NULL;

    if (isParkingLotEmpty(lot)) {
        lot->front = lot->rear = newNode;
    } else {
        lot->rear->next = newNode;
        lot->rear = newNode;
    }

    printf("Car parked at Space %d\n", spaceId);
}

// Function to calculate parking duration in seconds
double calculateParkingDuration(struct Node *carNode) {
    time_t currentTime;
    time(&currentTime);
    return difftime(currentTime, carNode->timeOfParking);
}

// Function to generate a bill when leaving the parking lot
void generateBill(struct Node *carNode) {
    time_t currentTime;
    time(&currentTime);

    double parkingDuration = calculateParkingDuration(carNode);

    printf("\n_________________________________\n");
    printf("\nBilling Details:\n");
    printf("Owner Name: %s\n", carNode->ownerName);
    printf("Car Number: %s\n", carNode->carNumber);
    printf("Time of Parking: %s", ctime(&(carNode->timeOfParking)));
    printf("Leaving Time: %s", ctime(&currentTime));

    double parkingFee = (parkingDuration / 3600) * HOURLY_RATE;

    if (parkingFee < 5) {
        parkingFee = 5;  // Minimum charge for less than half an hour
    }

    printf("Parking Duration: %.2f hours\n", parkingDuration / 3600);
    printf("Parking Fee: %.2f INR\n", parkingFee);
    printf("\n_________________________________\n");
}


// Function to make a space available by leaving
void leaveParking(struct ParkingLot *lot) {
    if (isParkingLotEmpty(lot)) {
        printf("No cars to leave\n");
        return;
    }

    struct Node* temp = lot->front;
    lot->front = lot->front->next;

    generateBill(temp);

    free(temp);

    if (lot->front == NULL) {
        lot->rear = NULL;  // Reset rear when the last car leaves
    }
}

// Function to free the allocated memory for the parking lot
void freeParkingLot(struct ParkingLot *lot) {
    while (lot->front != NULL) {
        struct Node* temp = lot->front;
        lot->front = lot->front->next;
        free(temp);
    }
}

int main() {
    struct ParkingLot parkingLot;
    int choice;

    initializeParkingLot(&parkingLot);

    do {
        printf("\n1. Display Parking Lot\n");
        printf("2. Park a Car\n");
        printf("3. Leave Parking\n");
        printf("0. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayParkingLot(&parkingLot);
                break;
            case 2:
                parkCar(&parkingLot);
                break;
            case 3:
                leaveParking(&parkingLot);
                break;
            case 0:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    // Free allocated memory before exiting
    freeParkingLot(&parkingLot);

    return 0;
}
