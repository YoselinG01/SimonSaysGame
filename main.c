#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include "address_map_arm.h"

int main() {
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    volatile uint32_t *lw_bridge = (uint32_t*) mmap(NULL, LW_BRIDGE_SPAN,
        PROT_READ | PROT_WRITE, MAP_SHARED, fd, LW_BRIDGE_BASE);
    volatile uint32_t *fpga_buttons = lw_bridge + (KEY_BASE / 4);
    volatile uint32_t *fpga_led = lw_bridge + (LEDR_BASE / 4);

    while(1) {
        uint32_t b = ~(*fpga_buttons) & 0xF;
        *fpga_led = b;  // light up LEDs according to pressed buttons
        usleep(10000);
    }
}