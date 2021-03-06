#pragma once

class Mpu6050{
public:
    // getters: values
    uint8_t left();
    uint8_t right();
    uint8_t down();
    uint8_t up();

    // getters: addresses
    uint8_t device_addr();
    uint8_t pwr_mgmt_reg_addr();
    uint8_t start_data_addr();

    // getters: boundaries
    int16_t lower_boundary();
    int16_t upper_boundary();
    
    // getters: oscillating range
    int16_t min_x_acc();
    int16_t max_x_acc();
    int16_t min_y_acc();
    int16_t max_y_acc();

    // setters: values
    void left(uint8_t val);
    void right(uint8_t val);
    void down(uint8_t val);
    void up(uint8_t val);

    // setters: addresses
    void device_addr(uint8_t val);
    void pwr_mgmt_reg_addr(uint8_t val);
    void start_data_addr(uint8_t val);

    // setters: boundaries
    void lower_boundary(int16_t val);
    void upper_boundary(int16_t val);
    
    // setters: oscillating range
    void min_x_acc(int16_t val);
    void max_x_acc(int16_t val);
    void min_y_acc(int16_t val);
    void max_y_acc(int16_t val);

private:
    uint8_t _left;
    uint8_t _right;
    uint8_t _down;
    uint8_t _up;

    // hardware addresses
    uint8_t _device_addr;
    uint8_t _pwr_mgmt_reg_addr;
    uint8_t _start_data_addr;

    // max lower/upper values to consider
    int16_t _lower_boundary;
    int16_t _upper_boundary;

    // even in a static, resting position, x,y,z values 
    // will oscillate, going up and down in a range
    // the below variables will help determine that range in order
    // to provide with a more consistent "zero" position.
    int16_t _min_x_acc;
    int16_t _max_x_acc;
    int16_t _min_y_acc;
    int16_t _max_y_acc;
};
