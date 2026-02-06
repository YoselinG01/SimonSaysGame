#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    // Seed random number generator
    srand(time(NULL));

    printf("=== Simon Says Game ===\n");

    // Simulate 4 random LED steps
    for (int i = 0; i < 4; i++) {
        int led = rand() % 4;  // Random LED 0-3
        printf("Step %d: Light LED %d\n", i + 1, led);
    }

    printf("Game sequence finished.\n");

    return 0;
}
