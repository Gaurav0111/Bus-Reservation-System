import java.util.Scanner;

class Bus {
    int busNumber;
    String source;
    String destination;
    int totalSeats;
    int availableSeats;
    float fare;

    public Bus(int busNumber, String source, String destination, int totalSeats, int availableSeats, float fare) {
        this.busNumber = busNumber;
        this.source = source;
        this.destination = destination;
        this.totalSeats = totalSeats;
        this.availableSeats = availableSeats;
        this.fare = fare;
    }
}


class Passenger {
    String name;
    int age;
    String email;
    String phoneNumber; // Added phone number field
    int seatNumber;
    int busNumber;
}

class User {
    String username;
    String password; // Increased password buffer size
}

public class Main {
    private static final int MAX_USERS = 10;
    private static final int MAX_BUSES = 10;
    private static final int MAX_PASSENGERS = 500;

    public static void main(String[] args) {
        User[] users = { new User() }; // Initialize users
        users[0].username = "admin";
        users[0].password = "admin";

        Bus[] buses = {
            new Bus(205, "DELHI", "VRINDAVAN", 50, 20, 100.0f),
            new Bus(301, "RISHIKESH", "DEHRADUN", 30, 10, 150.0f),
            new Bus(519, "PUNJAB", "SHIMLA", 40, 20, 200.0f)
        }; // Initialize buses

        int numUsers = 1;
        int numBuses = 3;

        Passenger[] passengers = new Passenger[MAX_PASSENGERS]; // Initialize passengers
        int numPassengers = 0;

        int loggedInUserId = -1;

        Scanner scanner = new Scanner(System.in);

        while (true) {
            if (loggedInUserId == -1) {
                displayMainMenu();
                int choice = scanner.nextInt();
                scanner.nextLine(); // Consume newline

                if (choice == 1) {
                    System.out.print("Enter Username: ");
                    String username = scanner.nextLine();
                    System.out.print("Enter Password: ");
                    String password = scanner.nextLine();

                    loggedInUserId = loginUser(users, numUsers, username, password);
                    if (loggedInUserId == -1) {
                        System.out.println("Login failed. Please check your username and password.");
                    } else {
                        System.out.printf("Login successful. Welcome, %s%n", username);
                    }
                } else if (choice == 2) {
                    System.out.println("Exiting the program.");
                    break;
                } else {
                    System.out.println("Invalid choice. Please try again.");
                }
            } else {
                displayUserMenu();
                int userChoice = scanner.nextInt();
                scanner.nextLine(); // Consume newline

                switch (userChoice) {
                    case 1:
                        bookTicket(buses, numBuses, passengers, numPassengers, loggedInUserId, scanner);
                        break;
                    case 2:
                        cancelTicket(buses, numBuses, passengers, numPassengers, loggedInUserId, scanner);
                        break;
                    case 3:
                        checkBusStatus(buses, numBuses);
                        break;
                    case 4:
                        System.out.println("Logging out.");
                        loggedInUserId = -1;
                        break;
                    default:
                        System.out.println("Invalid choice. Please try again.");
                }
            }
        }

        scanner.close();
    }

    static void displayMainMenu() {
        System.out.println("\n*****Bus Reservation System*****");
        System.out.println("1. Login");
        System.out.println("2. Exit");
        System.out.print("Enter your choice: ");
    }

    static void displayUserMenu() {
        System.out.println("\n.... User Menu....");
        System.out.println("1. Book a Ticket");
        System.out.println("2. Cancel a Ticket");
        System.out.println("3. Check Bus Status");
        System.out.println("4. Logout");
        System.out.print("Enter your choice: ");
    }

    static int loginUser(User[] users, int numUsers, String username, String password) {
        for (int i = 0; i < numUsers; i++) {
            if (users[i].username.equals(username) && users[i].password.equals(password)) {
                return i;
            }
        }
        return -1;
    }

    static void bookTicket(Bus[] buses, int numBuses, Passenger[] passengers, int numPassengers, int userId, Scanner scanner) {
        System.out.print("\nEnter Bus Number: ");
        int busNumber = scanner.nextInt();
        scanner.nextLine(); // Consume newline

        int busIndex = -1;
        for (int i = 0; i < numBuses; i++) {
            if (buses[i].busNumber == busNumber) {
                busIndex = i;
                break;
            }
        }

        if (busIndex == -1) {
            System.out.printf("Bus with Bus Number %d not found.%n", busNumber);
        } else if (buses[busIndex].availableSeats == 0) {
            System.out.println("Sorry, the bus is fully booked.");
        } else {
            Passenger passenger = new Passenger();

            System.out.print("Enter Passenger Name: ");
            passenger.name = scanner.nextLine();
            System.out.print("Enter Passenger Age: ");
            passenger.age = scanner.nextInt();
            scanner.nextLine(); // Consume newline
            System.out.print("Enter Passenger Email: ");
            passenger.email = scanner.nextLine();
            System.out.print("Enter Passenger Phone Number: ");
            passenger.phoneNumber = scanner.nextLine();

            // Validate phone number
            if (passenger.phoneNumber.length() != 10 || !isNumeric(passenger.phoneNumber)) {
                System.out.println("Invalid phone number format. Please enter a 10-digit integer.");
                return;
            }

            passenger.seatNumber = buses[busIndex].totalSeats - buses[busIndex].availableSeats + 1;
            passenger.busNumber = busNumber;

            buses[busIndex].availableSeats--;

            passengers[numPassengers] = passenger;

            System.out.println("Ticket booked successfully!");
            numPassengers++;
        }
    }

    static void cancelTicket(Bus[] buses, int numBuses, Passenger[] passengers, int numPassengers, int userId, Scanner scanner) {
        System.out.print("\nEnter Passenger Name: ");
        String name = scanner.nextLine();

        boolean found = false;
        for (int i = 0; i < numPassengers; i++) {
            if (passengers[i].name.equals(name) && passengers[i].busNumber == buses[userId].busNumber) {
                int busIndex = -1;
                for (int j = 0; j < numBuses; j++) {
                    if (buses[j].busNumber == passengers[i].busNumber) {
                        busIndex = j;
                        break;
                    }
                }
                buses[busIndex].availableSeats++;
                for (int j = i; j < numPassengers - 1; j++) {
                    passengers[j] = passengers[j + 1];
                }
                numPassengers--;
                found = true;
                System.out.println("Ticket canceled successfully!");
                break;
            }
        }
        if (!found) {
            System.out.printf("Passenger with name %s not found on this bus.%n", name);
        }
    }

    static void checkBusStatus(Bus[] buses, int numBuses) {
        System.out.println("\nBus Status:");
        for (int i = 0; i < numBuses; i++) {
            System.out.println("Bus Number: " + buses[i].busNumber);
            System.out.println("Total Seats: " + buses[i].totalSeats);
            System.out.println("Source: " + buses[i].source);
            System.out.println("Destination: " + buses[i].destination);
            System.out.println("Available Seats: " + buses[i].availableSeats);
            System.out.printf("Fare: %.2f%n", buses[i].fare);
            System.out.println("-------------------------------------------");
        }
    }

    static boolean isNumeric(String str) {
        for (int i = 0; i < str.length(); i++) {
            if (!Character.isDigit(str.charAt(i))) {
                return false;
            }
        }
        return true;
    }
}
