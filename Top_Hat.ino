#include <FastLED.h>

#define LED_PIN1 6
#define LED_PIN2 1
#define NUM_LEDS1 70
#define NUM_LEDS2 68
#define NUM_LEDS NUM_LEDS1+NUM_LEDS2

#define MAX_SINGLE_STRIP_LENGTH 13

#define DEFAULT_BRIGHTNESS 64

struct CRGB leds[NUM_LEDS];

//#define DEBUG
//#define DEBUG_ANIMATIONS

#include "DebugUtils.h"

CRGBPalette16 gPalettes[] = {HeatColors_p, LavaColors_p, RainbowColors_p, 
    CloudColors_p, OceanColors_p, ForestColors_p, PartyColors_p};
uint8_t gCurrentPaletteIndex = 0;


typedef struct { 
  uint8_t mStart;
  uint8_t mLength;
} Strip;

Strip strips[] = {
  {0, 13},
  {13, 12},
  {25, 11},
  {36, 11},
  {47, 11},
  {58, 12},

  {70, 13},
  {83, 10},
  {93, 11},
  {104, 11},
  {115, 11},
  {126, 12}
};

static const int numberOfStrips = sizeof(strips) / sizeof(strips[0]);  

#define MIC_PIN A10
#include "SoundReactive.h"

uint8_t gHue = 0;

#include "Animations.h"

/** 
 * Button Switcher
 */ 
#include "Button.h"
#define BUTTON_PIN      12
Button button(BUTTON_PIN, true);

typedef uint8_t (*Animation)(uint8_t arg1, uint8_t arg2);
typedef struct { 
  Animation mPattern;
  uint8_t mArg1;
  uint8_t mArg2;
} AnimationPattern;
 

AnimationPattern gAnimations[] = {
  {sinelon,  5, 4},
  {soundAnimate, 0, 0},
};

uint8_t gCurrentPatternNumber = 0; 

void setup() {

  DEBUG_START(57600);
  
  FastLED.addLeds<NEOPIXEL, LED_PIN1>(leds, 0, NUM_LEDS1).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<NEOPIXEL, LED_PIN2>(leds, NUM_LEDS1, NUM_LEDS2).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
  
  set_max_power_in_volts_and_milliamps(5, 500);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  show_at_max_brightness_for_power();
  delay(100);

  // Button
  button.attachClick(onClick);
}

void onClick() { 
  PRINT("Next animation");
  
  static const int numberOfPatterns = sizeof(gAnimations) / sizeof(gAnimations[0]);  
  gCurrentPatternNumber = (gCurrentPatternNumber+1) % numberOfPatterns;

  Animation animate = gAnimations[gCurrentPatternNumber].mPattern;
}

void loop() {
  button.tick();

  uint8_t arg1 = gAnimations[gCurrentPatternNumber].mArg1;
  uint8_t arg2 = gAnimations[gCurrentPatternNumber].mArg2;
  Animation animate = gAnimations[gCurrentPatternNumber].mPattern;
  
  animate(arg1, arg2);
  
//  soundAnimate(0, 0);
//  sinelon(5, 4);

  show_at_max_brightness_for_power();
  gHue++;

  #ifdef DEBUG_ANIMATIONS
  EVERY_N_MILLISECONDS(500)  {Serial.print("FPS: ");Serial.println(FastLED.getFPS());}
  #endif
}
