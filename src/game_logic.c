#include "../includes/peripherals/game_logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../lib/address_map_arm.h"


#define NUM_LEDS 4

void start_game(lcd_handle_t *lcd, void *key_base, void *led_base)
{
    if (!lcd || !lcd->initialized || !key_base || !led_base) return;

    volatile unsigned int *fpga_keys = (volatile unsigned int *)key_base;
    volatile unsigned int *fpga_leds = (volatile unsigned int *)led_base;

    srand(time(NULL));

    uint8_t sequence[NUM_LEDS];
    uint8_t user_input[NUM_LEDS];

    char buf[64];

    // Title
    lcd_clear(lcd);
    lcd_write_text(lcd, 0, 0, "SIMON SAYS GAME");
    sleep(2);

    // Generate sequence
    for (int i = 0; i < NUM_LEDS; i++) {
        sequence[i] = rand() % NUM_LEDS;
    }

    lcd_clear(lcd);
    lcd_write_text(lcd, 0, 20, "Watch LEDs");
    sleep(1);

    // Show sequence

    lcd_clear(lcd);

    for (int i = 0; i < NUM_LEDS; i++) {
        
        *fpga_leds = (1 << sequence[i]);
        usleep(500000);
        *fpga_leds = 0;
        usleep(200000);
    }

    lcd_clear(lcd);
    lcd_write_text(lcd, 0, 20, "Repeat sequence");
    sleep(1);

    // Input phase
    for (int step = 0; step < NUM_LEDS; step++) {

        unsigned int prev = (~(*fpga_keys)) & 0xF;
        unsigned int curr;

        while (1) {
            curr = (~(*fpga_keys)) & 0xF;
            unsigned int new_press = curr & ~prev;

            if (new_press) {
                int button = -1;

                if (new_press & 0x1) button = 0;
                else if (new_press & 0x2) button = 1;
                else if (new_press & 0x4) button = 2;
                else if (new_press & 0x8) button = 3;

                if (button == -1) continue;

                user_input[step] = button;

                *fpga_leds = (1 << button);
                usleep(200000);
                *fpga_leds = 0;

                break;
            }

            prev = curr;
            usleep(1000);
        }
    }


    for (int i = 0; i < NUM_LEDS; i++) {
        if (user_input[i] != sequence[i]) {
            lcd_clear(lcd);
            lcd_write_text(lcd, 0, 40, "YOU LOSE!");
            lcd_write_text(lcd, 0, 50, "Try again!");
            sleep(2);
            *fpga_leds = 0;
            return;
        }
    }

    lcd_clear(lcd);
    lcd_write_text(lcd, 0, 40, "YOU WIN!");
    sleep(2);
    *fpga_leds = 0;
}