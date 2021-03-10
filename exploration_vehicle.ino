/*
* Exploration Vehicle
* Transmitter Code
* Author: Gustavo Monardez
*
* D13   - SCLK                          D12   - MISO
* A0    - J1_X                          D11   - MOSI
* A1    - J1_Y                          D10   - CE
* A2    - J2_Y                          D9    - CNS
* A3    - J2_Y                          D8    -
* A4    - SDA      (LCD and MPU-6050)   D7    -
* A5    - SCL      (LCD and MPU-6050)   D6    - J2_SW
* A6                                    D5    - J1_SW
* A7                                    D4    - ROT ENC SW
*                                       D3    - ROT ENC DT
*                                       D2    - ROT ENC CLK
*/

#include "Globals.h"
#include "Configurations.h"
#include "ProcessDataOut.h"
#include "RotaryEncoder.h"
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <Wire.h>

using Globals::transmitter;
using Globals::transmitter_address;
using Globals::data_pkg;
// joystick
using Globals::j1_vrx_pin;
using Globals::j1_vry_pin;
using Globals::j1_sw_pin;

// mpu-6050
using Globals::mpu_addr;
using Globals::pwr_mgmt_1;
using Globals::start_data_addr;

void setup() {
    Serial.begin(9600);
    Serial.println("Initialization started...");
    Serial.print("data_pkg: ");Serial.println(sizeof(data_pkg));
    config_radio(transmitter, transmitter_address);
    config_joystick(data_pkg.j1,
                    j1_vrx_pin, INPUT, 
                    j1_vry_pin, INPUT,
                    j1_sw_pin, INPUT_PULLUP);
    config_mpu_6050(mpu_addr, pwr_mgmt_1, start_data_addr);
    config_rot_encoder();
    
    Serial.println("Initialization complete!\n\n");
}

char text[] = "Hello World!";
void loop() {
	//Globals::transmitter.write(&text, sizeof(text));
//    process_joystick_alt(data_pkg.j1);
//    Serial.print("left: ");
//    Serial.println(data_pkg.j1.left);
//    Serial.print("right: ");
//    Serial.println(data_pkg.j1.right);
//    Serial.print("down: ");
//    Serial.println(data_pkg.j1.down);
//    Serial.print("up: ");
//    Serial.println(data_pkg.j1.up);
    process_mpu_6050(data_pkg.mpu);
    Serial.print("mpu left: ");
    Serial.println(data_pkg.mpu.left());
    Serial.print("mpu right: ");
    Serial.println(data_pkg.mpu.right());
    Serial.print("mpu down: ");
    Serial.println(data_pkg.mpu.down());
    Serial.print("mpu up: ");
    Serial.println(data_pkg.mpu.up());
	delay(2000);
//    process_rot_encoder_isr();
}
