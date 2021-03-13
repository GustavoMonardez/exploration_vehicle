#pragma once

#include <stdint.h>

const uint8_t max_menu_items = 3;       // number of rows
const uint8_t max_chars_per_row = 16;   // number of cols

/*Note: the rotary encoder can generate values beyond the 
number of menu items */
const int min_menu_val = 0; // lower boundary
const int max_menu_val = 4; // upper boundary

// menu items matrix
extern char main_menus[max_menu_items][max_chars_per_row];

// veh/tx data disp submenu
const uint8_t veh_tx_max_items = 3;
extern char veh_tx_submenu[veh_tx_max_items][max_chars_per_row];

// it helps with menu navigation
enum class Menus {
    VEHICLE_DATA,
    OPERATION_MODE,
    LIGHTS,
    RETURN_HOME        
};

// it helps with menu navigation
enum class VehicleData {
    DISP_TX_DATA,
    DISP_VEH_DATA,
    CANCEL        
};

// this helps determine if we are on the main menu
// or navigating submenus of a main menu item
extern bool main_menu_item;
