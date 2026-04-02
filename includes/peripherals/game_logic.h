#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "lcd.h"

// Start the game using LCD, keys, and LEDs
void start_game(lcd_handle_t *lcd, void *key_base, void *led_base);

#endif