#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include "address_map_arm.h"

#define NUM_LEDS 4   // number of LEDs/buttons

int main() {
    // Open /dev/mem to access FPGA
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if(fd == -1) { 
        perror("open"); 
        return -1; 
    }

    // Map the LW bridge (for LEDs and KEYs)
    volatile uint32_t *lw_bridge = (uint32_t*) mmap(NULL, LW_BRIDGE_SPAN,
        PROT_READ | PROT_WRITE, MAP_SHARED, fd, LW_BRIDGE_BASE);
    if(lw_bridge == MAP_FAILED) {
        perror("mmap"); 
        close(fd);
        return -1;
    }

    // Pointers to LEDs and buttons 
    volatile uint32_t *fpga_led     = lw_bridge + (LEDR_BASE / 4);
    volatile uint32_t *fpga_buttons = lw_bridge + (KEY_BASE  / 4);

    srand(time(NULL));

    uint8_t sequence[NUM_LEDS];
    uint8_t user_input[NUM_LEDS];   

    // Generate random LED sequence
    for(int i = 0; i < NUM_LEDS; i++)
        sequence[i] = rand() % NUM_LEDS;

    // Flash the sequence on LEDs
    printf("Flashing sequence:\n");
    for(int i = 0; i < NUM_LEDS; i++) {
        *fpga_led = (1 << sequence[i]);   // LED on
        usleep(500000);                   // 0.5s delay
        *fpga_led = 0;                    // LED off
        usleep(200000);                   // 0.2s delay
    }

    printf("\nRepeat the sequence!\n");

    // User input loop
    for (int step = 0; step < NUM_LEDS; step++) {

    //printf("Waiting for input %d of %d...\n", step+1, NUM_LEDS);

    uint32_t prev = (~(*fpga_buttons)) & 0xF;
    uint32_t curr;

    while (1) {
        curr = (~(*fpga_buttons)) & 0xF;
        uint32_t new_press = curr & ~prev;

        if (new_press != 0) {
            int button = -1;

            if (new_press & 0x1) button = 0;
            if (new_press & 0x2) button = 1;
            if (new_press & 0x4) button = 2;
            if (new_press & 0x8) button = 3;

            printf("You pressed: %d\n", button);
            user_input[step] = button;

            
            *fpga_led = (1 << button);
            usleep(200000);   // LED stays on briefly
            *fpga_led = 0;

            break;
        }

        prev = curr;
        usleep(1000);
    }
}

    // Compare user input
    for (int i = 0; i < NUM_LEDS; i++) {
        if (user_input[i] != sequence[i]) {
            printf("You lose.\n");
            *fpga_led = 0;
            munmap((void*)lw_bridge, LW_BRIDGE_SPAN);
            close(fd);
            return 0;
        }
    }

    printf("\nYOU WIN!\n");

    *fpga_led = 0;
    munmap((void*)lw_bridge, LW_BRIDGE_SPAN);
    close(fd);

    return 0;
}