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
#include "RotaryEncoder.h"
#include "Menus.h"
#include <Wire.h>               // mpu-6050

// helper functions prototypes
void read_mpu_6050_data();
int16_t get_calibrated_x_acc();
int16_t get_calibrated_y_acc();
void draw_menu_page(LiquidCrystal_I2C& lcd, char menu[][16],
                         int8_t start_item,
                         int8_t selector_id,
                         int8_t selector_row,
                         int8_t custom_char_1_id = -1,
                         int8_t custom_char_2_id = -1,
                         int8_t custom_char_3_id = -1,
                         int8_t custom_char_4_id = -1,
                         int8_t custom_char_5_id = -1,
                         int8_t custom_char_6_id = -1);
                         
void draw_main_menu_page(LiquidCrystal_I2C& lcd, 
                         char menu_items[max_menu_items][max_chars_per_row], 
                         uint8_t sym_id,
                         uint8_t sym_pos, 
                         uint8_t item_row,
                         uint8_t item_col = 2);

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

// display unit variables
uint8_t selected_menu = 0;
uint8_t selected_submenu = 0;

enum Symbols : uint8_t {
    SELECT_ARROW,
    BACK_ARROW,
    THERMOMETER,
    BATTERY,
    DOT,
    PERCENT,
    SUN,
    BLANK  
};

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
* @fn                - process_display
*
* @brief             - process input data from display unit
*
* @param[in]         - Display object
* @param[in]         - menu selection to be updated
* @param[in]         - current internal temperature
* @param[in]         - bool that indicates if first time running
* 
* @return            - none
*
* @Note              - none
*********************************************************************/
bool first_time_submenu = true;
bool first_time_menu = true;

void process_display(LiquidCrystal_I2C& lcd, uint8_t& menu_select, int8_t temp, char data_in[32]) {
    //normalize values
    virtual_pos = (virtual_pos < min_menu_val) ? min_menu_val : virtual_pos;
    virtual_pos = (virtual_pos > max_menu_val) ? max_menu_val : virtual_pos;
    // if (main_menu_item) {
    // if user has turn knob on rot enc, or it's the first time
    // booting up
    if (virtual_pos != last_pos || first_time_menu) {
        first_time_menu = false;
        char curr_page[2][16];
        /********************************* page 1 *********************************/
        if (virtual_pos == 0) {
            sprintf(curr_page[0], "TX:   %dC   %d", temp, 86);
            sprintf(curr_page[1], "VEH:  %dC   %d", data_in[0], data_in[1]);        
            draw_menu_page(lcd, curr_page, 0, Symbols::DOT, 0,
                Symbols::THERMOMETER, Symbols::BATTERY, Symbols::PERCENT,
                Symbols::THERMOMETER, Symbols::BATTERY, Symbols::PERCENT);
                
        } else if (virtual_pos == 1) {
            sprintf(curr_page[0], "TX:   %dC   %d", temp, 86);
            sprintf(curr_page[1], "VEH:  %dC   %d", data_in[0], data_in[1]);
            draw_menu_page(lcd, curr_page, 0, Symbols::DOT, 1,
                Symbols::THERMOMETER, Symbols::BATTERY, Symbols::PERCENT,
                Symbols::THERMOMETER, Symbols::BATTERY, Symbols::PERCENT);
                
        } 
        /********************************* page 2 *********************************/
        else if (virtual_pos == 2) {
            sprintf(curr_page[0], "VEH:  %dC   %d", data_in[2], data_in[3]);
            sprintf(curr_page[1], "VEH:  %dC   %d", data_in[4], data_in[5]);
            draw_menu_page(lcd, curr_page, 0, Symbols::DOT, 0,
                Symbols::THERMOMETER, Symbols::BATTERY, Symbols::PERCENT,
                Symbols::THERMOMETER, Symbols::BATTERY, Symbols::PERCENT);
                
        } else if (virtual_pos == 3) {
            sprintf(curr_page[0], "VEH:  %dC   %d", data_in[2], data_in[3]);
            sprintf(curr_page[1], "VEH:  %dC   %d", data_in[4], data_in[5]);
            draw_menu_page(lcd, curr_page, 0, Symbols::DOT, 1,
                Symbols::THERMOMETER, Symbols::BATTERY, Symbols::PERCENT,
                Symbols::THERMOMETER, Symbols::BATTERY, Symbols::PERCENT);
                
        } 
        /********************************* page 3 *********************************/
        else if (virtual_pos == 4) {
            sprintf(curr_page[0], "COMMANDS");
            sprintf(curr_page[1], "");
            draw_menu_page(lcd, curr_page, 0, Symbols::SELECT_ARROW, 0);                
        }
        
        last_pos = virtual_pos ;
    }
}

void process_display(LiquidCrystal_I2C& lcd, uint8_t& menu_select, int8_t temp, bool& init_boot) {
    // header buffer
    char header[16];
    char header_veh_data_page[2][16];
    
    // update temp on initial boot up
    if (init_boot) {
        // header
        sprintf(header,"EXP. VEHICLE %dC", temp);
    
        strcpy(header_veh_data_page[0], header);
        strcpy(header_veh_data_page[1], "DISP VEH DATA");
        
        draw_main_menu_page(lcd, header_veh_data_page, 0, 1, 0, 0);
        
        // save curr menu we are on
        selected_menu = static_cast<uint8_t>(Menus::VEHICLE_DATA);
        init_boot = false;
    }

    //normalize values
    virtual_pos = (virtual_pos < min_menu_val) ? min_menu_val : virtual_pos;
    virtual_pos = (virtual_pos > max_menu_val) ? max_menu_val : virtual_pos;
    
    // main menu items
    if (main_menu_item) {
        // only update menus if a change has been detected
        if (virtual_pos != last_pos || first_time_menu) {
            first_time_menu = false;
            first_time_submenu = true;
            // DISP VEH DATA
            if (virtual_pos == static_cast<int>(Menus::VEHICLE_DATA)) {
                // header
                sprintf(header,"EXP. VEHICLE %dC", temp);
                
                strcpy(header_veh_data_page[0], header);
                strcpy(header_veh_data_page[1], "DISP VEH DATA");
                
                draw_main_menu_page(lcd, header_veh_data_page, 0, 1, 0, 0);

                // save curr menu we are on
                selected_menu = static_cast<uint8_t>(Menus::VEHICLE_DATA);
            }
            // OP MODE
            else if (virtual_pos == static_cast<int>(Menus::OPERATION_MODE)) {
                draw_main_menu_page(lcd, main_menus, 0, 0, 0, 2);
                // save curr menu we are on
                selected_menu = static_cast<uint8_t>(Menus::OPERATION_MODE);
            }
            // VEH LIGHTS
            else if (virtual_pos == static_cast<int>(Menus::LIGHTS)) {
                draw_main_menu_page(lcd, main_menus, 0, 1, 0, 2);
                // save curr menu we are on
                selected_menu = static_cast<uint8_t>(Menus::LIGHTS);
            }
            // RET HOME
            else if (virtual_pos == static_cast<int>(Menus::RETURN_HOME)) {
                draw_main_menu_page(lcd, main_menus, 0, 0, 2, 2);
                // save curr menu we are on
                selected_menu = static_cast<uint8_t>(Menus::RETURN_HOME);
            }
            last_pos = virtual_pos ;
        }
    }
    // submenus
    else {
        // resets virtual_pos to 0
        virtual_pos = virtual_pos - selected_menu;

        // curr submenu
        selected_submenu = selected_menu;
        
        if (virtual_pos != last_pos || first_time_submenu) {
            first_time_menu = true;
            first_time_submenu = false;
                        
            // vehicle data submenu
            if (selected_submenu == 0) {
                // TODO fix hardcoded value | update upper boundary for current submenu
                virtual_pos = (virtual_pos > 2) ? 2 : virtual_pos;
                if (virtual_pos == static_cast<int>(VehicleData::DISP_TX_DATA)) {
                    draw_main_menu_page(lcd, veh_tx_submenu, 0, 0, 0, 2);
                } else if (virtual_pos == static_cast<int>(VehicleData::DISP_VEH_DATA)) {
                    draw_main_menu_page(lcd, veh_tx_submenu, 0, 1, 0, 2);
                    selected_submenu = 10;
                } else if (virtual_pos == static_cast<int>(VehicleData::CANCEL)) {
                    draw_main_menu_page(lcd, veh_tx_submenu, 1, 0, 2, 2);
                }
            }
// this will get messy with the virtual pos value
// a submenu of a submenu?...nah..there's a better way      
// maybe a third category: main_menu, submenu, and display_page
//            else if (selected_submenu == 10) {
//                char veh_data[3][16] = {
//                    {"T: 30C | H: 76"},
//                    {"W: 3IN | L: DARK"},
//                    {"GO BACK"}
//                };
//                draw_main_menu_page(lcd, veh_data, 0, 0, 0, 2);
//            }
            last_pos = virtual_pos ;
        }
    }

    // option selected
    if ((!digitalRead(re_sw_pin))) {
        virtual_pos = selected_menu;
        if (main_menu_item) {
            main_menu_item = false;
        } else {
            main_menu_item = true;
        }
        while (!digitalRead(re_sw_pin)) {
          delay(10);
        }
    }
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

void draw_menu_page(LiquidCrystal_I2C& lcd, char menu[][16],
                         int8_t start_item,
                         int8_t selector_id,
                         int8_t selector_row,
                         int8_t custom_char_1_id = -1,
                         int8_t custom_char_2_id = -1,
                         int8_t custom_char_3_id = -1,
                         int8_t custom_char_4_id = -1,
                         int8_t custom_char_5_id = -1,
                         int8_t custom_char_6_id = -1) {
    // clear the screen
    lcd.clear();

    // row indicator/select arrow
    lcd.setCursor(0, selector_row);
    lcd.write(selector_id);
    
    // item 1
    lcd.setCursor(1, 0);
    lcd.print(menu[start_item]);

    // custom character 1 item 1
    if (custom_char_1_id != -1) {
        lcd.setCursor(6, 0);
        lcd.write(custom_char_1_id);
    }
    
    // custom character 2 item 1
    if (custom_char_2_id != -1) {
        lcd.setCursor(12, 0);
        lcd.write(custom_char_2_id);
    }
    
    // custom character 3 item 1
    if (custom_char_3_id != -1) {
        lcd.setCursor(15, 0);
        lcd.write(custom_char_3_id);
    }
    
    // item 2
    if (start_item+1 < max_menu_items) {
        lcd.setCursor(1, 1);
        lcd.print(menu[start_item+1]);
    }      
          
    // custom character 1 item 2
    if (custom_char_4_id != -1) {
        lcd.setCursor(6, 1);
        lcd.write(custom_char_4_id);
    }
    
    // custom character 2 item 2
    if (custom_char_5_id != -1) {
        lcd.setCursor(12, 1);
        lcd.write(custom_char_5_id);
    }
    Serial.print("custom_char_6_id: "); Serial.println(custom_char_6_id);
    // custom character 3 item 2
    if (custom_char_6_id != -1) {
        lcd.setCursor(15, 1);
        lcd.write(custom_char_6_id);   
    }
}
void draw_main_menu_page(LiquidCrystal_I2C& lcd, 
                         char menu_items[max_menu_items][max_chars_per_row], 
                         uint8_t sym_id,
                         uint8_t sym_pos, 
                         uint8_t item_row,
                         uint8_t item_col = 2) {
    // clear the screen
    lcd.clear();
    
    // symbol/custom char
    lcd.setCursor(0, sym_pos);
    lcd.write(sym_id);
    
    // item 1
    lcd.setCursor(item_col, 0);
    lcd.print(menu_items[item_row]);
    
    // item 2
    if (item_row+1 < max_menu_items) {
        lcd.setCursor(item_col, 1);
        lcd.print(menu_items[item_row+1]);
    }
}
