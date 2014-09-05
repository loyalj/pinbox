// Because I'm too lazy to write my own bounce code today
#include <Bounce.h>

// This will light up when side bumpers are pressed
// creating a basic hardware response test
int pinLED = 13;

// Left side Bumper Button
int pinPushButtonLeft = 3;
Bounce pushButtonLeft = Bounce(pinPushButtonLeft, 10);

// Right side bumper button
int pinPushButtonRight = 4;
Bounce pushButtonRight = Bounce(pinPushButtonRight, 10);

// Ball plunger button either a physical spring loaded 
// device as on most older tables or a button as on pinbox
int pinPushButtonPlunger = 5;
Bounce pushButtonPlunger = Bounce(pinPushButtonPlunger, 10);

// Game start button almost always found on the left front 
// side of a pinball cabinet, used to start game play
int pinPushButtonStart = 6;
Bounce pushButtonStart = Bounce(pinPushButtonStart, 10);

// Tracks Teensy USB keyboard modifiers between loops
int lastModifiers = 0;
int currentModifiers = 0;


/**
 * Let's get the pinbox going! Open up the button ports! Get
 * the LED Blinker port ready to fire!
 */
void setup()
{
  pinMode(pinPushButtonLeft, INPUT_PULLUP);
  pinMode(pinPushButtonRight, INPUT_PULLUP);
  pinMode(pinPushButtonPlunger, INPUT_PULLUP);
  pinMode(pinPushButtonStart, INPUT_PULLUP);
  
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, LOW);
}

/**
 * During each update we'll check for changes in the state of
 * each of our buttons.  In the case of the shift keys we will
 * manage what is being pressed using set_modifier bit flags.  
 * In the case of the keys, we'll map the rising and falling 
 * edges of the button signals directly to 'key up' and 'key down'
 * by setting specific keycodes
 * 
 * With button polling complete, we'll then use Teensy's built-in
 * USB Keyboard functionality to send it back to the USB bus
 */
void loop()
{
  // Poll the left bumper button and set the bitflag to ON during
  // a falling edge and back to OFF on rising edges
  if (pushButtonLeft.update()) {
    if(pushButtonLeft.fallingEdge()) {
      currentModifiers = currentModifiers |= MODIFIERKEY_LEFT_SHIFT;
    }
    else {
      currentModifiers = currentModifiers ^= MODIFIERKEY_LEFT_SHIFT;
    }
    
  }
  
  // Now do the same to the right bumper button and set the correct bits
  // of current modifiers for this side
  if (pushButtonRight.update()) {
    if(pushButtonRight.fallingEdge()) {
      currentModifiers = currentModifiers |= MODIFIERKEY_RIGHT_SHIFT;   
    }
    else {
      currentModifiers = currentModifiers ^= MODIFIERKEY_RIGHT_SHIFT;
    }
  }
  
  // Poll the ball plunger button and set key #1 to space bar on falling
  // edges.  Reset key #1 to unset when a rising edge occurs
  if (pushButtonPlunger.update()) {
    if(pushButtonPlunger.fallingEdge()) {
      Keyboard.set_key1(KEY_SPACE);
      Keyboard.send_now();
    }
    else {
      Keyboard.set_key1(0);
      Keyboard.send_now();
    }
  }
  
  // Poll the game start button in the same way and set or unset the
  // enter key across the USB
  if (pushButtonStart.update()) {
    if(pushButtonStart.fallingEdge()) {
      Keyboard.set_key2(KEY_ENTER);
      Keyboard.send_now();
    }
    else {
      Keyboard.set_key2(0);
      Keyboard.send_now();
    }
  }
  
  // Because the modifiers are set in a single call as a bit mask
  // we can delay calling it until the end of the loop having set
  // currentModifiers up above. Also, may as well build in a value
  // change check and only send key events when the bitmask changes
  if(currentModifiers != lastModifiers) {
    Keyboard.set_modifier(currentModifiers);
    Keyboard.send_now();
    
    lastModifiers = currentModifiers;
  }
   
  // Turn off the bright light
  digitalWrite(pinLED, LOW);
  
  // Basic LED response indicator checks for button presses on the
  // left and right side bumpers and lights the bright light if it
  // detects a non-zero bit mask.
  if(currentModifiers != 0) {
    digitalWrite(pinLED, HIGH);
  }
}
