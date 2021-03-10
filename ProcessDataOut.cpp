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
void read_mpu_6050_data();
int16_t get_calibrated_x_acc();
int16_t get_calibrated_y_acc();

// mpu-6050 raw data variables
int16_t raw_x_acc;
int16_t raw_y_acc;
int16_t raw_z_acc;
int16_t raw_temp;
int16_t raw_x_gyro;
int16_t raw_y_gyro;
int16_t raw_z_gyro;

// mpu-6050 calibrated data variables
int16_t calib_x_acc;
int16_t calib_y_acc;
int16_t calib_z_acc;
int16_t calib_temp;
int16_t calib_x_gyro;
int16_t calib_y_gyro;
int16_t calib_z_gyro;

// mpu-6050 pwm variables
const int16_t pwm_x_val = 255;
const int16_t pwm_y_val = 255;

 
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
void process_mpu_6050(Mpu6050::Instance& mpu) {
    // read raw data from device
    read_mpu_6050_data();

    // get left/right calibrated data
    calib_x_acc = get_calibrated_x_acc();

    // get down/up calibrated data
    calib_y_acc = get_calibrated_y_acc();
    
    // save left/right calibrated data on mpu handle
    if (calib_x_acc < 0) { 
        // convert to positive num before assigning
        mpu.left(calib_x_acc * -1);
        mpu.right(0);
    } else {
        mpu.right(calib_x_acc);
        mpu.left(0);
    }
    
    // save down/up calibrated data on mpu handle
    if (calib_y_acc < 0) {
        // convert to positive num before assigning
        mpu.down(calib_y_acc  * -1);
        mpu.up(0);
    } else {
        mpu.up(calib_y_acc);
        mpu.down(0);
    }

    /* The temperature in degrees C for a given register value may be 
    * computed as:
    * Temperature in degrees 
    * C = (TEMP_OUT Register Value as a signed quantity)/340 + 36.53
    * Page 30 of MPU-6000-Register-Map1.pdf
    */
    mpu.temp(raw_temp/340.00+36.53);
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

// helper functions
void read_mpu_6050_data() {
    Wire.beginTransmission(Mpu6050::device_addr());
    Wire.write(Mpu6050::start_data_addr());  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(Mpu6050::device_addr(),(uint8_t*)14,(uint8_t*)true);  // request a total of 14 registers
    raw_x_acc  = Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    raw_y_acc  = Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    raw_z_acc  = Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    raw_temp   = Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    raw_x_gyro = Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    raw_y_gyro = Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    raw_z_gyro = Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

int16_t get_calibrated_x_acc() {
    // ignore oscillating values within the min_x_acc and max_x_acc range
    // as this is a range of values that will be present, even when the 
    // mpu is not moving
    if (raw_x_acc >= Mpu6050::min_x_acc() && raw_x_acc <= Mpu6050::max_x_acc()) return 0;

    // if the min and max values are outside of the pre-determined
    // range, return the the max pwm values
    if (raw_x_acc < Mpu6050::lower_boundary()) return pwm_x_val * -1;
    if (raw_x_acc > Mpu6050::upper_boundary()) return pwm_x_val;

    // if the raw value is less than the min_x_acc
    // the mpu has been tilted to the left, in this case
    // represented as a negative value ranging from -1 to -255
    int8_t sign = (raw_x_acc < Mpu6050::min_x_acc()) ? -1 : 1;

    /* map input data to values according to the following:
    *     left/down: -1 to -255
    *     down/up:    1 to  255
    *     neutral:    0
    */     
    return map(raw_x_acc, Mpu6050::min_x_acc(), (sign*Mpu6050::upper_boundary()), 0, (sign*pwm_x_val)); 
}

int16_t get_calibrated_y_acc() {
    // ignore oscillating values within the min_y_acc and max_y_acc range
    // as this is a range of values that will be present, even when the 
    // mpu is not moving
    if (raw_y_acc >= Mpu6050::min_y_acc() && raw_y_acc <= Mpu6050::max_y_acc()) return 0;

    // if the min and max values are outside of the pre-determined
    // range, return the the max down/up pwm values
    if (raw_y_acc < Mpu6050::lower_boundary()) return pwm_y_val * -1;
    if (raw_y_acc > Mpu6050::upper_boundary()) return pwm_y_val;

    // if the raw value is less than the min_x_acc
    // the mpu has been tilted to the left, in this case
    // represented as a negative value ranging from -1 to -255
    int8_t sign = (raw_y_acc < Mpu6050::min_y_acc()) ? -1 : 1;

    /* map input data to values according to the following:
    *     left/down: -1 to -255
    *     down/up:    1 to  255
    *     neutral:    0
    */     
    return map(raw_y_acc, Mpu6050::min_y_acc(), (sign*Mpu6050::upper_boundary()), 0, (sign*pwm_y_val)); 
}
