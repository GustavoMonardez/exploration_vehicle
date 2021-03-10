#pragma once

#include <LiquidCrystal_I2C.h>
#include "Joystick.h"
#include "Mpu6050.h"

void process_joystick(Joystick& j);
void process_joystick_alt(Joystick& j);
void process_mpu_6050(Mpu6050::Instance& mpu);
void process_display(LiquidCrystal_I2C& lcd, uint8_t& menu_select, int8_t temp);
void send_data();
