#pragma once

#include "Joystick.h"
#include "Mpu6050.h"

void process_joystick(Joystick& j);
void process_joystick_alt(Joystick& j);
void process_mpu_6050(Mpu6050& mpu);
void send_data();
