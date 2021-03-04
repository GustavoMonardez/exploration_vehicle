#pragma once

struct Mpu6050{
    uint8_t left;
    uint8_t right;
    uint8_t down;
    uint8_t up;

    uint8_t device_addr;
    uint8_t pwr_mgmt_reg_addr;
    uint8_t start_data_addr;
};

void init_mpu_6050(const uint8_t mpu_addr, const uint8_t pwr_mgmt_reg);
void calibrate_mpu_6050(const uint8_t mpu_addr, const uint8_t data_addr);
