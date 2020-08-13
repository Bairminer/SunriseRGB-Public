 
// SunriseRGB Default Program
// Required libraries
// FastLED library - https://github.com/FastLED/FastLED
// JCButton Library - https://github.com/JChristensen/Button
// Arduino EEPROM (if missing from your Arduino install) - https://github.com/PaulStoffregen/EEPROM
 
#include "FastLED.h"
#include "JC_Button.h"                                          
#include "EEPROM.h"
 
 
// Pushbutton setup
const int modeButton = 0;         //Button located under silver/gold side                          
const int powerButton = 2;        //Button located under black side
 
Button mode(2, 50, true, true);                    
Button power(0, 50, true, true);
 
int powerStatus = 1;
 
// LED Strip setup
#define DATA_PIN   1
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    91
CRGB leds[NUM_LEDS];
 
// Animation frame rate (We reccomend 60 for best visual to memory consumption ratio)
#define FRAMES_PER_SECOND  60
 
// Brightness (0 is off, 255 is max brightness)
uint8_t maxBrightness = 255;
 
uint8_t currentAnimation = EEPROM.read(0);
uint8_t hue = 0;
 
void setup() {
  mode.begin();
  power.begin();
 
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(maxBrightness);
 
// Sets EEPROM to correct state on new chip
   if (currentAnimation == 255){
    currentAnimation = 0;
  }
}
 
 
// Enter names of animation loops
typedef void (*animationList[])();
animationList animation = {rainbow, red, green, blue, orange, purple, yellow, white};
 
 
void loop()
{
  // update LED strip with animation
  animation[currentAnimation]();
  FastLED.show();  
  FastLED.delay(1000/FRAMES_PER_SECOND);
 
  // Cycles FastLED rainbow
  EVERY_N_MILLISECONDS( 20 ) { hue++; }
 
  // Check for button presses
    readbutton();                                              
    readpower();
   
}
 
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
 
void nextAnimation()
{
  // Moves to next animation
  currentAnimation = (currentAnimation + 1) % ARRAY_SIZE( animation);
}
 
// Enter your own effects here (make sure to add the loop name to the animationList)
void rainbow()
{
  // FastLED rainbow effect
  fill_rainbow( leds, NUM_LEDS, hue, 7);
}
 
void red()
{
  fill_solid( leds, NUM_LEDS, CRGB(255,0,0));
}
 
void green()
{
  fill_solid( leds, NUM_LEDS, CRGB(0,255,0));
}
 
void blue()
{
  fill_solid( leds, NUM_LEDS, CRGB(0,0,255));
}
 
void orange()
{
  fill_solid( leds, NUM_LEDS, CRGB(255,128,0));
}
 
void purple()
{
  fill_solid( leds, NUM_LEDS, CRGB(128,0,255));
}
 
void yellow()
{
  fill_solid( leds, NUM_LEDS, CRGB(255,255,0));
}
 
void white()
{
  fill_solid( leds, NUM_LEDS, CRGB(255,255,255));
}
 
 
// Button press changes power state  
void readpower() {                                          
  power.read();
  if(power.wasReleased()) {
    if ( powerStatus == 0) {
      FastLED.setBrightness(0);               //Turn LEDs off
      EEPROM.update(0, currentAnimation);     //Save current animation to EEPROM
      powerStatus = 1;
      delay(50);
    }
    else {
      FastLED.setBrightness(255);
      powerStatus = 0;
    }
    }
}
 
// Button press changes animation
  void readbutton() {                                          
  mode.read();
  if(mode.wasReleased()) {
    nextAnimation();
  }
}
