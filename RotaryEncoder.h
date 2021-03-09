#pragma once

#include <stdint.h>

// rotary encoder pins
const uint8_t re_clk_pin    = 2;
const uint8_t re_dt_pin     = 3;
const uint8_t re_sw_pin     = 4;

// rotary encoder values
extern volatile int virtual_pos;
extern int last_pos;
extern int re_sw_state;

// utility functions
void config_rot_encoder();
void rot_encoder_isr();

// testing only
void process_rot_encoder_isr();
