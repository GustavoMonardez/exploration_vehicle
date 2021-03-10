#pragma once

#include <stdint.h>

namespace Mpu6050 {
    /********* Mpu-6050 addresses api *********/
    uint8_t device_addr();
    uint8_t pwr_mgmt_reg_addr();
    uint8_t start_data_addr();
    void device_addr(uint8_t val);
    void pwr_mgmt_reg_addr(uint8_t val);
    void start_data_addr(uint8_t val);

    /********* Mpu-6050 boundaries api *********/
    int16_t lower_boundary();
    int16_t upper_boundary();
    void lower_boundary(int16_t val);
    void upper_boundary(int16_t val);
    
    /***** Mpu-6050 oscillating range api *****/
    int16_t min_x_acc();
    int16_t max_x_acc();
    int16_t min_y_acc();
    int16_t max_y_acc();       
    void min_x_acc(int16_t val);
    void max_x_acc(int16_t val);
    void min_y_acc(int16_t val);
    void max_y_acc(int16_t val);

    // hardware addresses
    extern uint8_t _device_addr;
    extern uint8_t _pwr_mgmt_reg_addr;
    extern uint8_t _start_data_addr;

    // max lower/upper values to consider
    extern int16_t _lower_boundary;
    extern int16_t _upper_boundary;

    // even in a static, resting position, x,y,z values 
    // will oscillate, going up and down in a range
    // the below variables will help determine that range in order
    // to provide with a more consistent "zero" position.
    extern int16_t _min_x_acc;
    extern int16_t _max_x_acc;
    extern int16_t _min_y_acc;
    extern int16_t _max_y_acc;
    
    class Instance{
    public:
        // getters
        uint8_t left();
        uint8_t right();
        uint8_t down();
        uint8_t up();
        int8_t temp();
    
        // setters
        void left(uint8_t val);
        void right(uint8_t val);
        void down(uint8_t val);
        void up(uint8_t val); 
        void temp(int8_t val);   
    private:
        uint8_t _left;
        uint8_t _right;
        uint8_t _down;
        uint8_t _up;
        int8_t _temp;
    };
}
