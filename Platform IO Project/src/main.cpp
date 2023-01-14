#include "DigiKeyboard.h"
#include "stdint.h"

// Constants
#define LED_ON_PCB                             1 // digital pin connected to LED
#define ALREADY_TRIGGERED                   true // controls interrupt handler
#define NOT_TRIGGERED                      false // controls interrupt handler
#define INTERRUPT_TYPE                   FALLING // interrupt triggered on a FALLING edge
#define DEBOUNCE                              10 // time to wait in milli secs
#define BUTTON_PIN                             2 // PIN P2
#define BUTTON_PIN_INT                         0 // INT0 (PIN P2)
#define KEYSTROKE                (byte)KEY_ENTER // Keystroke we want to to, and casting to avoid warning

// Macros
#define TOGGLE_LED(led) digitalWrite(led, !digitalRead(led))

// Globals
bool initialisation_complete            = false;          // inhibit any interrupts until initialisation is complete
volatile  bool interrupt_process_status = NOT_TRIGGERED;  // start with no switch press pending, ie false (!triggered)

// Button ISR
void button_interrupt_handler(){
  
    if (interrupt_process_status == ALREADY_TRIGGERED or !initialisation_complete == true)
        return;
        
    interrupt_process_status = ALREADY_TRIGGERED;
    
}

bool send_char() {
  bool            proceed           = false;
  static bool     switching_pending = false;
  static uint16_t elapse_timer;
  
  if (interrupt_process_status == ALREADY_TRIGGERED) {
      
      if (!switching_pending) {
           switching_pending = true;
           elapse_timer      = millis(); // start elapse timing for debounce checking
           proceed           = true;     // advise that switch has been pressed
           TOGGLE_LED(LED_ON_PCB);
      }
      
      else if (switching_pending && digitalRead(BUTTON_PIN) == HIGH) {
          if (millis() - elapse_timer >= DEBOUNCE) {
             switching_pending        = false;          // reset for next button press interrupt cycle
             interrupt_process_status = NOT_TRIGGERED; // reopen ISR for business now button on/off/debounce cycle complete
             TOGGLE_LED(LED_ON_PCB);
          }
      }
  } 
  
  return proceed; // either no press request or debounce period not elapsed
}


void setup() {

    // Declare GPIOs
    pinMode(LED_ON_PCB,  OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Init GPIOs
    digitalWrite(LED_ON_PCB, LOW);

    // IRQ and ISR FOR BUTTON
    attachInterrupt(BUTTON_PIN_INT, button_interrupt_handler, INTERRUPT_TYPE);
    initialisation_complete = true;
}


void loop() {

    if(send_char())
        DigiKeyboard.sendKeyStroke(KEYSTROKE);
    else
        DigiKeyboard.delay(100);
    
}