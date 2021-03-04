#pragma once

#include <stdint.h>

struct Joystick {
	uint8_t left;
	uint8_t right;
	uint8_t down;
	uint8_t up;

    uint8_t vrx_pin;
    uint8_t vry_pin;
    uint8_t sw_pin;
};
