/**
 * @file Configurations.cpp
 *
 * @brief Configuration functions implementation
 *
 * @author Gustavo Monardez
 *
 */
#include "Configurations.h"
#include "RotaryEncoder.h"
#include "LcdCustomCharacters.h"
#include "Mpu6050.h"

// helper functions prototypes
void init_mpu_6050(const uint8_t mpu_addr, const uint8_t pwr_mgmt_reg);
void calibrate_mpu_6050(const uint8_t mpu_addr, const uint8_t data_addr);

/*********************************************************************
* @fn                - config_radio
*
* @brief             - initializes and starts radio/transmitter 
*					   communication
*
* @param[in]         - reference to radio object (RF24)
* @param[in]         - 40-bit address of the pipe to open
*
* @return            - none
*
* @Note				 - none
*********************************************************************/
void config_radio(RF24& radio, const uint64_t address) {
	radio.begin();
	radio.openWritingPipe(address);
	radio.setPALevel(RF24_PA_MIN);
	radio.stopListening();
	Serial.println("    Radio config complete!");
}

/*********************************************************************
* @fn                - config_joystick
*
* @brief             - initializes and starts joystick
*
* @param[in]         - joystick handle
* @param[in]         - x axis pin number
* @param[in]         - x axis pin mode
* @param[in]         - y axis pin number
* @param[in]         - y axis pin mode
* @param[in]         - sw pin number
* @param[in]         - sw pin mode
* 
* @return            - none
*
* @Note				 - none
*********************************************************************/
void config_joystick(Joystick& j,
    const uint8_t vrx_pin, const uint8_t vrx_mode,
	const uint8_t vry_pin, const uint8_t vry_mode,
	const uint8_t sw_pin, const uint8_t sw_mode) {

    // link pins to joystick handle structure
    j.vrx_pin = vrx_pin;
    j.vry_pin = vry_pin;
    j.sw_pin = sw_pin;
    
	// config pins
	pinMode(vrx_pin, vrx_mode); 
	pinMode(vry_pin, vry_mode);
	pinMode(sw_pin, sw_mode);

	Serial.println("    Joysticks config complete!");
}

/*********************************************************************
* @fn                - config_rot_encoder
*
* @brief             - initializes and attaches isr to clk pin
*
* @param[in]         - clk pin number
* @param[in]         - clk pin mode
* @param[in]         - data pin number
* @param[in]         - data pin mode
* @param[in]         - sw pin number
* @param[in]         - sw pin mode
*
* @return            - none
*
* @Note				 - none
*********************************************************************/
void config_rot_encoder(const uint8_t clk_pin, const uint8_t clk_mode,
	const uint8_t dt_pin, const uint8_t dt_mode,
	const uint8_t sw_pin, const uint8_t sw_mode) {
	// config pins
	pinMode(clk_pin, clk_mode);
	pinMode(dt_pin, dt_mode);
	pinMode(sw_pin, sw_mode);

	// Attach the routine to service the interrupts
	attachInterrupt(digitalPinToInterrupt(clk_pin), rot_encoder_isr, LOW);

	Serial.println("    Rotary Encoder config complete!");
}

/*********************************************************************
* @fn                - config_display
*
* @brief             - initializes lcd display
*
* @param[in]         - reference to lcd object
*
* @return            - none
*
* @Note				 - none
*********************************************************************/
void config_display(LiquidCrystal_I2C& lcd) {
	lcd.init();
	lcd.backlight();
	lcd.createChar(0, select_arrow);
	lcd.clear();
	lcd.setCursor(0, 1);
	lcd.write(0);
	lcd.setCursor(2, 0);
	/* temp menu,to be updated once it's defined */
	lcd.print("EXP. VEHICLE");
	lcd.setCursor(2, 1);
	lcd.print("LIGHTS ON/OFF");
	/*********************************************/

	Serial.println("    Display config complete!");
}

/*********************************************************************
* @fn                - config_mpu_6050
*
* @brief             - wake up and initial calibration of mpu-6050
*
* @param[in]         - address to the mpu module
* @param[in]         - address to power management 1 register
* @param[in]         - start address of data register
*
* @return            - none
*
* @Note				 - none
*********************************************************************/
void config_mpu_6050(const uint8_t mpu_addr, const uint8_t pwr_mgmt_reg, const uint8_t data_addr) {
	// initialize
	init_mpu_6050(mpu_addr, pwr_mgmt_reg);

	// calibrate
	calibrate_mpu_6050(mpu_addr, data_addr);

	Serial.println("    MPU-6050 config complete!");
}
