#pragma once

#include "Joystick.h"
#include "Mpu6050.h"
    
struct DataPackage {
    Joystick j1;
    Joystick j2;
    
    Mpu6050::Instance mpu;

    uint8_t menu_select;
};
