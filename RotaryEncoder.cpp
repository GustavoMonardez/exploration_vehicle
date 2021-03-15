/**
 * @file RotaryEncoder.cpp
 *
 * @brief Data processing variables initialization,
 *        and utility functions implementations.
 *
 * @author Gustavo Monardez
 *
 */
#include "RotaryEncoder.h"
#include "Arduino.h"

volatile int virtual_pos = 0;
int last_pos = 0;
int re_sw_state = 1;

/*********************************************************************
* @fn                - rot_encoder_isr
*
* @brief             - isr that detects when the knob has been turned
*
* @param[in]         - none
*
* @return            - none
*
* @Note              - none
*********************************************************************/
void rot_encoder_isr() {
      static unsigned long lastInterruptTime = 0;
      unsigned long interruptTime = millis();
    
      // If interrupts come faster than 5ms, assume it's a bounce and ignore
      if (interruptTime - lastInterruptTime > 5) {
        if (digitalRead(re_dt_pin) == LOW)
        {
            virtual_pos-- ; // Could be -5 or -10
        }
        else {
            virtual_pos++ ; // Could be +5 or +10
        }
    
        // Restrict value from 0 to +100
        //virtualPosition = min(100, max(0, virtualPosition));
      }
      // Keep track of when we were here last (no more than every 5ms)
      lastInterruptTime = interruptTime;
}

/*********************************************************************
* @fn                - config_rot_encoder
*
* @brief             - initializes and attaches isr to clk pin
*
* @param[in]         - none
*
* @return            - none
*
* @Note              - none
*********************************************************************/
void config_rot_encoder() {
    // config pins
    pinMode(re_clk_pin, INPUT);
    pinMode(re_dt_pin, INPUT);
    pinMode(re_sw_pin, INPUT_PULLUP);

    // Attach the routine to service the interrupts
    attachInterrupt(digitalPinToInterrupt(re_clk_pin), rot_encoder_isr, LOW);

    Serial.println("    Rotary Encoder config complete!");
}

// testing only
void process_rot_encoder_isr() {
     // If the current rotary switch position has changed then update everything
    if (virtual_pos != last_pos) {
        // Write out to serial monitor the value and direction
        Serial.print(virtual_pos > last_pos ? "Up  :" : "Down:");
        Serial.println(virtual_pos);
        
        // Keep track of this new value
        last_pos = virtual_pos ;
    }
}
