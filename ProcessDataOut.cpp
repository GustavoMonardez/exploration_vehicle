/**
 * @file ProcessDataOut.cpp
 *
 * @brief Read and Send Data functions implementation
 *
 * @author Gustavo Monardez
 *
 */
#include "ProcessDataOut.h"     // func prototypes
#include "wiring_private.h"     // analog read
#include <Wire.h>               // mpu-6050

// helper functions prototypes
void read_mpu_6050_data(Mpu6050& mpu);

// mpu-6050 raw data variables
int16_t raw_x_acc;
int16_t raw_y_acc;
int16_t raw_z_acc;
int16_t raw_temp;
int16_t raw_x_gyro;
int16_t raw_y_gyro;
int16_t raw_z_gyro;

/*********************************************************************
* @fn                - process_joystick
*
* @brief             - process input data from joystick (x,y axis)
*
* @param[in]         - Joystick object
* 
* @return            - none
*
* @Note              - version that processes vrx values
*                      as left/right and vry values as down/up
*********************************************************************/
void process_joystick(Joystick& j) {
    // read vx input data
    int vx_val = analogRead(j.vrx_pin);
    
    // read vy input data
    int vy_val = analogRead(j.vry_pin);
    
    // left
    if (vx_val <= 500) {
        j.left = map(vx_val, 500, 0, 0, 255);
        j.right = 0;
    }
    // right
    else if (vx_val >= 510){
        j.right = map(vx_val, 510, 1023, 0, 255);
        j.left = 0;
    }
    // up / forward
    if (vy_val <= 500) {
        j.up = map(vy_val, 500, 0, 0, 255);
        j.down = 0;
    }
    // down / reverse
    else if (vy_val >= 510){
        j.down = map(vy_val, 510, 1023, 0, 255);
        j.up = 0;
    }
    
    // when the value of  vrx and vry is between 501 and 509
    // it means that the joystick is in its resting position
    // so we need to make sure all values are set to 0
    if (vx_val >= 501 && vx_val <= 509) {
        j.left = 0;
        j.right = 0;
    }
    
    if (vy_val >= 501 && vy_val <= 509) {
        j.down = 0;
        j.up = 0;
    }
}

/*********************************************************************
* @fn                - process_joystick_alt
*
* @brief             - process input data from joystick (x,y axis)
*
* @param[in]         - Joystick object
* 
* @return            - none
*
* @Note              - alternate version that processes vrx values
*                      as down/up and vry values as left/right
*********************************************************************/
void process_joystick_alt(Joystick& j) {
    // read vx input data
    int vx_val = analogRead(j.vrx_pin);
    
    // read vy input data
    int vy_val = analogRead(j.vry_pin);
    
    // down / reverse
    if (vx_val <= 500) {
        j.down = map(vx_val, 500, 0, 0, 255);
        j.up = 0;
    }
    // up / forward
    else if (vx_val >= 510){
        j.up = map(vx_val, 510, 1023, 0, 255);
        j.down = 0;
    }
    // left
    if (vy_val <= 500) {
        j.left = map(vy_val, 500, 0, 0, 255);
        j.right = 0;
    }
    // right
    else if (vy_val >= 510){
        j.right = map(vy_val, 510, 1023, 0, 255);
        j.left = 0;
    }

    // when the value of  vx and vy is between 501 and 509
    // it means that the joystick is in its resting position
    // so we need to make sure all values are set to 0
    if (vx_val >= 501 && vx_val <= 509) {
        j.down = 0;
        j.up = 0;
    }

    if (vy_val >= 501 && vy_val <= 509) {
        j.left = 0;
        j.right = 0;
    }
}

/*********************************************************************
* @fn                - process_mpu_6050
*
* @brief             - process input data from mpu-6050 (x,y axis)
*
* @param[in]         - Joystick object
* 
* @return            - none
*
* @Note              - alternate version that processes vrx values
*                      as down/up and vry values as left/right
*********************************************************************/
void process_mpu_6050(Mpu6050& mpu) {
    // read raw data from device
    read_mpu_6050_data(mpu);

    //mpu.
}

/*********************************************************************
* @fn                - send_data
*
* @brief             - send data from all modules via NRF24
*
* @param[in]         - none
* 
* @return            - none
*
* @Note              - none
*********************************************************************/
void send_data() {
    
}

// helper functions prototypes
void read_mpu_6050_data(Mpu6050& mpu) {
    Wire.beginTransmission(mpu.device_addr());
    Wire.write(mpu.start_data_addr());  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(mpu.device_addr(),(uint8_t*)14,(uint8_t*)true);  // request a total of 14 registers
    raw_x_acc  = Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    raw_y_acc  = Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    raw_z_acc  = Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    raw_temp   = Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    raw_x_gyro = Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    raw_y_gyro = Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    raw_z_gyro = Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}
