#pragma once

#include <LiquidCrystal_I2C.h>
#include <RF24.h>
#include <nRF24L01.h>
#include "Joystick.h"
#include "Mpu6050.h"

void config_radio(RF24& radio, const uint64_t address);

void config_joystick(Joystick& j,
    const uint8_t vrx_pin, const uint8_t vrx_mode,
	const uint8_t vry_pin, const uint8_t vry_mode,
	const uint8_t sw_pin, const uint8_t sw_mode);

void config_display(LiquidCrystal_I2C& lcd);

void config_mpu_6050(
    const uint8_t mpu_addr, 
    const uint8_t pwr_mgmt_reg, 
    const uint8_t data_addr);
