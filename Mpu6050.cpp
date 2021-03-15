/**
 * @file Mpu6050.cpp
 *
 * @brief Mpu6050 api and data class definition
 *
 * @author Gustavo Monardez
 *
 */
#include "Mpu6050.h"

namespace Mpu6050 {
    // hardware addresses
    uint8_t _device_addr = 0;
    uint8_t _pwr_mgmt_reg_addr = 0;
    uint8_t _start_data_addr = 0;

    // max lower/upper values to consider
    int16_t _lower_boundary = -12000;
    int16_t _upper_boundary  =  12000;

    // oscillating range
    int16_t _min_x_acc = 0;
    int16_t _max_x_acc = 0;
    int16_t _min_y_acc = 0;
    int16_t _max_y_acc = 0;

    /********* Mpu-6050 addresses api *********/
    uint8_t device_addr() {
        return _device_addr;
    }
    
    uint8_t pwr_mgmt_reg_addr() {
        return _pwr_mgmt_reg_addr;
    }
    
    uint8_t start_data_addr() {
        return _start_data_addr;
    }

    void device_addr(uint8_t val) {
        _device_addr = val;
    }
    
    void pwr_mgmt_reg_addr(uint8_t val) {
        _pwr_mgmt_reg_addr = val;
    }
    
    void start_data_addr(uint8_t val) {
        _start_data_addr = val;
    }
    
    /********* Mpu-6050 boundaries api *********/
    int16_t lower_boundary() {
        return _lower_boundary;
    }
    
    int16_t upper_boundary() {
        return _upper_boundary;
    }

    void lower_boundary(int16_t val) {
        _lower_boundary = val;
    }
    
    void upper_boundary(int16_t val) {
        _upper_boundary = val;
    }
    
    /********* Mpu-6050 oscillating range api *********/ 
    int16_t min_x_acc() {
        return _min_x_acc;
    }
    
    int16_t max_x_acc() {
        return _max_x_acc;
    }
    
    int16_t min_y_acc() {
        return _min_y_acc;
    }
    
    int16_t max_y_acc() {
        return _max_y_acc;
    }    
    
    void min_x_acc(int16_t val) {
        _min_x_acc = val;
    }
    
    void max_x_acc(int16_t val) {
        _max_x_acc = val;
    }
    void min_y_acc(int16_t val) {
        _min_y_acc = val;
    }
    
    void max_y_acc(int16_t val) {
        _max_y_acc = val;
    }
    
    /********* Mpu-6050 class getters *********/
    uint8_t Instance::left() {
        return this->_left;
    }
    
    uint8_t Instance::right() {
        return this->_right;
    }
    
    uint8_t Instance::down() {
        return this->_down;
    }
    
    uint8_t Instance::up() {
        return this->_up;
    }

    int8_t Instance::temp() {
        return this->_temp;
    }
    
    /********* Mpu-6050 class setters *********/    
    void Instance::left(uint8_t val) {
        this->_left = val;
    }
    
    void Instance::right(uint8_t val) {
        this->_right = val;
    }
    
    void Instance::down(uint8_t val) {
        this->_down = val;
    }
    
    void Instance::up(uint8_t val) {
        this->_up = val;
    }

    void Instance::temp(int8_t val) {
        this->_temp = val;
    }
}
