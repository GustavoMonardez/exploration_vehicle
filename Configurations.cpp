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

// mpu-6050 local variables
int16_t acc_buffer = 180;
struct MpuRawData {
    int16_t x_acc;
    int16_t y_acc;
    int16_t z_acc;
    int16_t temp;
    int16_t x_gyro;
    int16_t y_gyro;
    int16_t z_gyro;
};

// helper functions prototypes
void read_mpu_6050_raw(Mpu6050& mpu, MpuRawData& data);
void init_mpu_6050(Mpu6050& mpu);
void calibrate_mpu_6050(Mpu6050& mpu);


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
* @param[in]         - mpu handle
* @param[in]         - address to the mpu module
* @param[in]         - address to power management 1 register
* @param[in]         - start address of data register
*
* @return            - none
*
* @Note				 - none
*********************************************************************/
void config_mpu_6050(Mpu6050& mpu,
    const uint8_t mpu_addr, 
    const uint8_t pwr_mgmt_reg, 
    const uint8_t data_addr) {

    // link addresses to mpu handle structure
    mpu.device_addr(mpu_addr);
    mpu.pwr_mgmt_reg_addr(pwr_mgmt_reg);
    mpu.start_data_addr(data_addr);
    
	// initialize
	init_mpu_6050(mpu);

	// calibrate
	calibrate_mpu_6050(mpu);

	Serial.println("    MPU-6050 config complete!");
}

// helper functions 
void read_mpu_6050_raw(Mpu6050& mpu, MpuRawData& data) {
    Wire.beginTransmission(mpu.device_addr());
    Wire.write(mpu.start_data_addr());  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(mpu.device_addr(),(uint8_t*)14,(uint8_t*)true);  // request a total of 14 registers
    data.x_acc  = Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    data.y_acc  = Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    data.z_acc  = Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    data.temp   = Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    data.x_gyro = Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    data.y_gyro = Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    data.z_gyro = Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

void init_mpu_6050(Mpu6050& mpu) {
    Wire.begin();
    Wire.beginTransmission(mpu.device_addr());
    Wire.write(mpu.pwr_mgmt_reg_addr());  // PWR_MGMT_1 register
    Wire.write(0);     // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
}

void calibrate_mpu_6050(Mpu6050& mpu) {
    // used to calculate oscillating range
    MpuRawData mpu_raw_data;
    for (uint8_t i = 0; i < 255; ++i) {
        // read data
        read_mpu_6050_raw(mpu, mpu_raw_data);

        // left / right
        mpu.min_x_acc(min(mpu.min_x_acc(), mpu_raw_data.x_acc));
        mpu.max_x_acc(max(mpu.max_x_acc(), mpu_raw_data.x_acc));

        // forward / backward
        mpu.min_y_acc(min(mpu.min_y_acc(), mpu_raw_data.y_acc));
        mpu.max_y_acc(max(mpu.max_y_acc(), mpu_raw_data.y_acc));
    }
    // once the range is established, we add a small buffer
    // to help minimize false readings
    mpu.min_x_acc(mpu.min_x_acc() - acc_buffer);
    mpu.max_x_acc(mpu.max_x_acc() + acc_buffer);
    
    mpu.min_y_acc(mpu.min_y_acc() - acc_buffer);
    mpu.max_y_acc(mpu.max_y_acc() + acc_buffer);
}
