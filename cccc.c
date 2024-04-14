#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to calculate factorial
int factorial(int n) {
    if (n == 0) return 1;
    return n * factorial(n - 1);
}

// Function to check if a number is even or odd
void checkEvenOdd(int n) {
    if (n % 2 == 0)
        printf("%d is even.\n", n);
    else
        printf("%d is odd.\n", n);
}

// Function to print the multiplication table
void printTable(int n) {
    for (int i = 1; i <= 10; ++i) {
        printf("%d * %d = %d\n", n, i, n * i);
    }
}

int main() {
    int number;
    printf("Enter a number: ");
    scanf("%d", &number);

    pid_t p1 = fork();

    if (p1 == 0) {
        // This is P1
        printf("Factorial of %d is %d\n", number, factorial(number));
    } else {
        wait(NULL); // Wait for P1 to finish

        pid_t p2 = fork();

        if (p2 == 0) {
            // This is P2
            checkEvenOdd(number);
        } else {
            wait(NULL); // Wait for P2 to finish

            pid_t p3 = fork();

            if (p3 == 0) {
                // This is P3
                printTable(number);
            } else {
                wait(NULL); // Wait for P3 to finish
            }
        }
    }

    return 0;
}
