/**
 * @file Globals.h
 *
 * @brief Global variables and constants
 *
 * @author Gustavo Monardez
 *
 */

#pragma once

#include <LiquidCrystal_I2C.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <Wire.h>
#include <SPI.h>
#include "Joystick.h"
#include "Mpu6050.h"
#include "DataPackage.h"


namespace Globals {
	// transmitter
	RF24 transmitter(10, 9); // CE, CSN;
	const uint64_t transmitter_address = 0x0000000001;
	
	// joystick 1 pins
	const uint8_t j1_vrx_pin	= A0;
	const uint8_t j1_vry_pin	= A1;
	const uint8_t j1_sw_pin		= 5;

	// joystick 2 pins
	const uint8_t j2_vrx_pin	= A2;
	const uint8_t j2_vry_pin	= A3;
	const uint8_t j2_sw_pin		= 6;

	// display unit
	LiquidCrystal_I2C lcd(0x27, 16, 2);

	// mpu-6050
	const uint8_t mpu_addr		    = 0x68;
    const uint8_t pwr_mgmt_1        = 0x6B;
    const uint8_t start_data_addr   = 0x3B;

	// outgoing data
	DataPackage data_pkg;
}
