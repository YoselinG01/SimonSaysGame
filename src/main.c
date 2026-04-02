#include <stdio.h>
#include <unistd.h>
#include "../includes/peripherals/lcd.h"
#include "../includes/peripherals/game_logic.h"
#include "../lib/address_map_arm.h"
#include "../includes/hal/hal-api.h"

int main() {
    lcd_handle_t lcd = {0};

    // Initialize LCD
    if (lcd_init(&lcd) != 0) {
        lcd_write_text(&lcd, 0, 0, "LCD INIT FAILED");
        return -1;
    }

    // Open HAL mapping for LW bridge peripherals
    hal_map_t hal_map = {0};
    if (hal_open(&hal_map) != 0) {
        lcd_write_text(&lcd, 0, 0, "HAL OPEN FAILED");
        lcd_cleanup(&lcd);
        return -1;
    }

    // Start Simon Says game
    start_game(
        &lcd,
        hal_get_virtual_addr(&hal_map, KEY_BASE),
        hal_get_virtual_addr(&hal_map, LEDR_BASE)
    );

    // Close HAL mapping
    hal_close(&hal_map);

    // Cleanup LCD
    //lcd_write_text(&lcd, 0, 0, "Game Complete");
    //sleep(1);

    lcd_cleanup(&lcd);

    return 0;
}