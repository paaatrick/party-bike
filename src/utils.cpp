#include "utils.h"

uint8_t mapTopPath(uint8_t i) 
{
    uint8_t length_1 = DOWN_TUBE_L - 1;
    uint8_t length_2 = TOP_TUBE_R - SEAT_STAY_L - 1;
    uint8_t total_length = length_1 + length_2;
    uint8_t breakpoint = map(length_1, 0, total_length, 0, 255);
    if (i < breakpoint + 1) {
        return DOWN_TUBE_L - 1 - map(i, 0, breakpoint + 1, 0, length_1);
    } else {
        return TOP_TUBE_R - 1 - map(i, breakpoint, 255, 0, length_2);
    }
}

uint8_t mapBottomPath(uint8_t i)
{
    return map8(i, DOWN_TUBE_L, SEAT_STAY_L - 1);
}