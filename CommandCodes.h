enum class CommandCodes : uint8_t {
    // no code
    /*0*/NONE,
    
    // operation mode
    /*1*/OP_MANUAL,
    /*2*/OP_AUTO,

    // return home
    /*3*/RET_HOME,

    // lights
    /*4*/LIGHTS_ON,
    /*5*/LIGHTS_OFF,
    /*6*/LIGHTS_AUTO      
};

char command_msgs[][16] = {
    "",
    "OP MAN CMD SENT",
    "OP AUT CMD SENT",  
    "RET HM CMD SENT",
    "LT ON CMD SENT",
    "LT OFF CMD SENT",
    "LT AUT CMD SENT"
};
