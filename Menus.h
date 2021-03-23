/**
 * @file Menus.h
 *
 * @brief Menu constants and declarations
 *
 * @author Gustavo Monardez
 *
 */
#pragma once

#include <stdint.h>


enum class ActiveMenu {
    MAIN_MENU,
    COMMANDS,
    OPERATION_MODE,
    RETURN_HOME,
    LIGHTS  
};

// commands available
enum class Commands {
    OPERATION_MODE,
    RETURN_HOME ,
    LIGHTS,
    CANCEL       
};

// options available for the operation mode command
enum class OperationMode {
    MANUAL,
    AUTO,
    CANCEL  
};

// options available for the return home command
enum class ReturnHome {
    CONFIRM,
    CANCEL  
};

// options available for the lights command
enum class Lights {
    ON,
    OFF,
    AUTO,
    BACK  
};
