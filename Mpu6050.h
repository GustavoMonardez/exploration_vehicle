#pragma once

void init_mpu_6050(const uint8_t mpu_addr, const uint8_t pwr_mgmt_reg);
void calibrate_mpu_6050(const uint8_t mpu_addr, const uint8_t start_addr);