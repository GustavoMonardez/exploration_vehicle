#pragma once
/*
* File:		Globals.h
* Project:	Exploration Vehicle
* Author:	Gustavo Monardez
* Date: 	02/26/2021
*/

#include <LiquidCrystal_I2C.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <Wire.h>
#include <SPI.h>


namespace Globals {
	// transmitter
	RF24 transmitter(10, 9); // CE, CSN;
	const char transmitter_address[6] = { "00001" };
	
	// joystick 1 pins
	const uint8_t j1_vrx_pin	= A0;
	const uint8_t j1_vry_pin	= A1;
	const uint8_t j1_sw_pin		= 5;

	// joystick 2 pins
	const uint8_t j2_vrx_pin	= A2;
	const uint8_t j2_vry_pin	= A3;
	const uint8_t j2_sw_pin		= 6;

	// rotary encoder pins
	const uint8_t re_clk_pin	= 2;
	const uint8_t re_dt_pin		= 3;
	const uint8_t re_sw_pin		= 4;

	// display unit
	LiquidCrystal_I2C lcd(0x27, 16, 2);

	// mpu-6050
	const uint8_t mpu_addr		= 0x68;

	// outgoing data
	struct DataPackage {
		// j1
		// j2
		// mpu-6050
		// menu select
	};
	DataPackage data_pkg;
}
