#define _POSIX_C_SOURCE 199309L  // MUST be before any #include <time.h>
#include "terasic_os_includes.h"
#include "terasic_lib.h"
#include <time.h>
#include <stdint.h>

// link with -lrt

long get_tick_count(void)
{
    struct timespec now;

    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
        return -1; // error
    }

    // Return microseconds
    return now.tv_sec * 1000000L + now.tv_nsec / 1000;
}
