#include "Mpu6050.h"

Mpu6050::Mpu6050() {
    // boundary values work for current project
    // and were decided upon, after testing
    // setters exists if different values are needed
    this->_lower_boundary = -12000;
    this->_upper_boundary  =  12000;
}

// getters: values
uint8_t Mpu6050::left() {
    return this->_left;
}

uint8_t Mpu6050::right() {
    return this->_right;
}

uint8_t Mpu6050::down() {
    return this->_down;
}

uint8_t Mpu6050::up() {
    return this->_up;
}

// getters: addresses
uint8_t Mpu6050::device_addr() {
    return this->_device_addr;
}

uint8_t Mpu6050::pwr_mgmt_reg_addr() {
    return this->_pwr_mgmt_reg_addr;
}

uint8_t Mpu6050::start_data_addr() {
    return this->_start_data_addr;
}

// getters: boundaries
int16_t Mpu6050::lower_boundary() {
    return this->_lower_boundary;
}

int16_t Mpu6050::upper_boundary() {
    return this->_upper_boundary;
}

// getters: oscillating range
int16_t Mpu6050::min_x_acc() {
    return this->_min_x_acc;
}

int16_t Mpu6050::max_x_acc() {
    return this->_max_x_acc;
}

int16_t Mpu6050::min_y_acc() {
    return this->_min_y_acc;
}

int16_t Mpu6050::max_y_acc() {
    return this->_max_y_acc;
}

// setters: values
void Mpu6050::left(uint8_t val) {
    this->_left = val;
}

void Mpu6050::right(uint8_t val) {
    this->_right = val;
}

void Mpu6050::down(uint8_t val) {
    this->_down = val;
}

void Mpu6050::up(uint8_t val) {
    this->_up = val;
}

// setters: addresses
void Mpu6050::device_addr(uint8_t val) {
    this->_device_addr = val;
}

void Mpu6050::pwr_mgmt_reg_addr(uint8_t val) {
    this->_pwr_mgmt_reg_addr = val;
}

void Mpu6050::start_data_addr(uint8_t val) {
    this->_start_data_addr = val;
}

// setters: boundaries
void Mpu6050::lower_boundary(int16_t val) {
    this->_lower_boundary = val;
}

void Mpu6050::upper_boundary(int16_t val) {
    this->_upper_boundary = val;
}

// setters: oscillating range
void Mpu6050::min_x_acc(int16_t val) {
    this->_min_x_acc = val;
}

void Mpu6050::max_x_acc(int16_t val) {
    this->_max_x_acc = val;
}
void Mpu6050::min_y_acc(int16_t val) {
    this->_min_y_acc = val;
}

void Mpu6050::max_y_acc(int16_t val) {
    this->_max_y_acc = val;
}
