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
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <Wire.h>

void setup() {
    Serial.begin(9600);
    Serial.println("Initialization started...");
    
    config_radio(Globals::transmitter, Globals::transmitter_address);
    config_joystick(Globals::j1_vrx_pin, INPUT, 
                    Globals::j1_vry_pin, INPUT,
                    Globals::j1_sw_pin, INPUT_PULLUP);
    
    Serial.println("Initialization complete!\n\n");
}

char text[] = "Hello World!";
void loop() {
	Globals::transmitter.write(&text, sizeof(text));
	delay(1000);
}
