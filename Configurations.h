#pragma once

#include <RF24.h>
#include <nRF24L01.h>

void config_radio(RF24& radio);

void config_joystick(const uint8_t vrx_pin, const uint8_t vrx_mode,
	const uint8_t vry_pin, const uint8_t vry_mode,
	const uint8_t sw_pin, const uint8_t sw_mode);

void config_rot_encoder(const uint8_t clk_pin, const uint8_t clk_mode,
	const uint8_t dt_pin, const uint8_t dt_mode,
	const uint8_t sw_pin, const uint8_t sw_mode);

void config_display(LiquidCrystal_I2C& lcd);

void config_mpu_6050(const uint8_t mpu_addr, const uint8_t pwr_mgmt_reg, const uint8_t start_addr);
