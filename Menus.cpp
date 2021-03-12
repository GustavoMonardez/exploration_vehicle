#include "Menus.h"

bool main_menu_item = true;

/************* menus *************/
extern char main_menus[max_menu_items][max_chars_per_row] = {
    {"OP MODE"},
    {"VEH LIGHTS"},
    {"RET HOME"}
};

/*********** submenus ***********/
char veh_tx_submenu[veh_tx_max_items][max_chars_per_row] = {
    {"DISP TX DATA"},
    {"DISP VEH DATA"},
    {"GO BACK"}
};
