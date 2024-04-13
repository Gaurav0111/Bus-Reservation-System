#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 10
#define MAX_BUSES 10
#define MAX_PASSENGERS 500

struct Bus {
    int busNumber;
    char source[20];
    char destination[30];
    int totalSeats;
    int availableSeats;
    float fare;
};

struct Passenger {
    char name[50];
    int age;
    char email[30];
    int seatNumber;
    int busNumber;
};

struct User {
    char username[20];
    char password[20];
};

// Function prototypes
void displayMainMenu();
void displayUserMenu();
int loginUser(struct User users[], int numUsers, char username[], char password[]);
void bookTicket(struct Bus buses[], int numBuses, struct Passenger passengers[], int* numPassengers, int userId);
void cancelTicket(struct Bus buses[], int numBuses, struct Passenger passengers[], int* numPassengers, int userId);
void checkBusStatus(struct Bus buses[], int numBuses, int userId);

// Main function
int main() {
    // Initialize users and buses
    struct User users[MAX_USERS] = {{"dsaproject", "caf613"}};
    struct Bus buses[MAX_BUSES] = {{205, "DELHI", "VRINDAVAN", 50, 20, 100.0},
                                    {301, "RISHIKESH", "DEHRADUN", 30, 10, 150.0},
                                    {519, "PUNJAB", "SHIMLA", 40, 20, 200.0}};
    int numUsers = 1;
    int numBuses = 3;

    // Initialize passengers
    struct Passenger passengers[MAX_PASSENGERS];
    int numPassengers = 0;

    int loggedInUserId = -1;

    // Main loop
    while (1) {
        if (loggedInUserId == -1) {
            displayMainMenu();
            int choice;
            scanf("%d", &choice);

            if (choice == 1) {
                char username[20];
                char password[20]; // Increased password buffer size

                printf("Enter Username: ");
                scanf("%s", username);
                printf("Enter Password: ");
                scanf("%s", password);

                loggedInUserId = loginUser(users, numUsers, username, password);
                if (loggedInUserId == -1) {
                    printf("Login failed. Please check your username and password.\n");
                } else {
                    printf("Login successful. Welcome, %s\n", username);
                }
            } else if (choice == 2) {
                printf("Exiting the program.\n");
                break;
            } else {
                printf("Invalid choice. Please try again.\n");
            }
        } else {
            displayUserMenu();
            int userChoice;
            scanf("%d", &userChoice);

            switch (userChoice) {
                case 1:
                    bookTicket(buses, numBuses, passengers, &numPassengers, loggedInUserId);
                    break;
                case 2:
                    cancelTicket(buses, numBuses, passengers, &numPassengers, loggedInUserId);
                    break;
                case 3:
                    checkBusStatus(buses, numBuses, loggedInUserId);
                    break;
                case 4:
                    printf("Logging out.\n");
                    loggedInUserId = -1;
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        }
    }

    return 0;
}

void displayMainMenu() {
    printf("\n*****Bus Reservation System*****\n");
    printf("1. Login\n");
    printf("2. Exit\n");
    printf("Enter your choice: ");
}

void displayUserMenu() {
    printf("\n.... User Menu....\n");
    printf("1. Book a Ticket\n");
    printf("2. Cancel a Ticket\n");
    printf("3. Check Bus Status\n");
    printf("4. Logout\n");
    printf("Enter your choice: ");
}

int loginUser(struct User users[], int numUsers, char username[], char password[]) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return i;
        }
    }
    return -1;
}

void bookTicket(struct Bus buses[], int numBuses, struct Passenger passengers[], int* numPassengers, int userId) {
    printf("\n Enter Bus Number: ");
    int busNumber;
    scanf("%d", &busNumber);

    int busIndex = -1;
    for (int i = 0; i < numBuses; i++) {
        if (buses[i].busNumber == busNumber) {
            busIndex = i;
            break;
        }
    }

    if (busIndex == -1) {
        printf("Bus with Bus Number %d not found.\n", busNumber);
    } else if (buses[busIndex].availableSeats == 0) {
        printf("Sorry, the bus is fully booked.\n");
    } else {
        printf("Enter Passenger Name: ");
        scanf("%s", passengers[*numPassengers].name);

        printf("Enter Passenger Age: ");
        scanf("%d", &passengers[*numPassengers].age);

        printf("Enter Passenger Email: ");
        scanf("%s", passengers[*numPassengers].email);

        printf("Enter Passenger Phone Number: ");
        char phoneNumber[11];
        scanf("%s", phoneNumber);
        // Validate phone number
        if (strlen(phoneNumber) != 10 || !isNumeric(phoneNumber)) {
            printf("Invalid phone number format. Please enter a 10-digit integer.\n");
            return;
        }
        passengers[*numPassengers].seatNumber = buses[busIndex].totalSeats - buses[busIndex].availableSeats + 1;
        passengers[*numPassengers].busNumber = busNumber;

        buses[busIndex].availableSeats--;

        printf("Ticket booked successfully!\n");
        (*numPassengers)++;
    }
}

// Function to check if a string consists of only numeric characters
int isNumeric(const char *str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}


void cancelTicket(struct Bus buses[], int numBuses, struct Passenger passengers[], int* numPassengers, int userId) {
    printf("\nEnter Passenger Name: ");
    char name[50];
    scanf("%s", name);

    int found = 0;
    for (int i = 0; i < *numPassengers; i++) {
        if (strcmp(passengers[i].name, name) == 0 && passengers[i].busNumber == buses[userId].busNumber) {
            int busIndex = -1;
            for (int j = 0; j < numBuses; j++) {
                if (buses[j].busNumber == passengers[i].busNumber) {
                    busIndex = j;
                    break;
                }
            }
            buses[busIndex].availableSeats++;
            for (int j = i; j < (*numPassengers) - 1; j++) {
                passengers[j] = passengers[j + 1];
            }
            (*numPassengers)--;
            found = 1;
            printf("Ticket canceled successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("Passenger with name %s not found on this bus.\n", name);
    }
}

void checkBusStatus(struct Bus buses[], int numBuses) {
    printf("\nBus Status:\n");
    for (int i = 0; i < numBuses; i++) {
        printf("Bus Number: %d\n", buses[i].busNumber);
        printf("Total Seats: %d\n", buses[i].totalSeats);
        printf("Source: %s\n", buses[i].source);
        printf("Destination: %s\n", buses[i].destination);
        printf("Available Seats: %d\n", buses[i].availableSeats);
        printf("Fare: %.2f\n", buses[i].fare);
        printf("-------------------------------------------\n");
    }
}

