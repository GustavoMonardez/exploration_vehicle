const int num_of_items = 2; // number of rows
const int item_size = 16;   // max number of characters per row

/*Note: the rotary encoder can generate values beyond the 
number of menu items */
const int min_menu_val = 0; // lower boundary
const int max_menu_val = 3; // upper boundary

extern char header_veh_data_page[num_of_items][item_size];
extern char op_mode_lights_page[num_of_items][item_size];
extern char ret_home_page[num_of_items][item_size];

// it helps with menu navigation
enum class Menus {
    VEHICLE_DATA,
    OPERATION_MODE,
    LIGHTS,
    RETURN_HOME        
};

// this helps determine if we are on the main menu
// or navigating submenus of a main menu item
extern bool main_menu_item;
