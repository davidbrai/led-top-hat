#include <FastLED.h>

#define LED_PIN1 6
#define LED_PIN2 1
#define NUM_LEDS1 70
#define NUM_LEDS2 68
#define NUM_LEDS (NUM_LEDS1+NUM_LEDS2)

#define MAX_SINGLE_STRIP_LENGTH 13

#define DEFAULT_BRIGHTNESS 64

struct CRGB leds[NUM_LEDS];

#define DEBUG
//#define DEBUG_ANIMATIONS

#include "DebugUtils.h"

CRGBPalette16 gPalettes[] = {HeatColors_p, LavaColors_p, RainbowColors_p, 
    CloudColors_p, OceanColors_p, ForestColors_p, PartyColors_p};
uint8_t gCurrentPaletteIndex = 0;


typedef struct { 
  uint8_t mStart;
  uint8_t mLength;
  bool isDown;
} Strip;

Strip strips[] = {
  {0, 13, true},
  {13, 12, false},
  {25, 11, true},
  {36, 11, false},
  {47, 11, true},
  {58, 12, false},

  {70, 13, true},
  {83, 10, false},
  {93, 11, true},
  {104, 11, false},
  {115, 11, true},
  {126, 12, false}
};

uint8_t ledPosDown(uint8_t stripIndex, uint8_t pixelIndex)
{
  if (strips[stripIndex].isDown) {
    return strips[stripIndex].mStart + pixelIndex;
  } else {
    return strips[stripIndex].mStart + strips[stripIndex].mLength - 1 - pixelIndex;
  }
}

uint8_t ledPosUp(uint8_t stripIndex, uint8_t pixelIndex)
{
  if (strips[stripIndex].isDown) {
    return strips[stripIndex].mStart + strips[stripIndex].mLength - 1 - pixelIndex;
  } else {
    return strips[stripIndex].mStart + pixelIndex;
  }
}

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

#include "SettingsMode.h"

typedef uint8_t (*Animation)(uint8_t arg1, uint8_t arg2);
typedef struct { 
  Animation mPattern;
  uint8_t mArg1;
  uint8_t mArg2;
} AnimationPattern;
 

AnimationPattern gAnimations[] = {
  {drops, 1, 4},
  {drops, 0, 4},
  {spiral, 0, 1},
  {sinelon,  5, 4},
  {soundAnimate, 0, 0},
  {soundAnimate, 1, 0},
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
  button.attachTripleClick(onTripleClick);
}

void onClick() { 
  PRINT("Next animation");
  
  static const int numberOfPatterns = sizeof(gAnimations) / sizeof(gAnimations[0]);  
  gCurrentPatternNumber = (gCurrentPatternNumber+1) % numberOfPatterns;

  Animation animate = gAnimations[gCurrentPatternNumber].mPattern;
}

void onTripleClick() { 
  PRINT("Opening settings");
  
  SettingsMode settings = SettingsMode(&button);
  settings.showSettings();

  uint8_t brightness = settings.getUserBrightness();
  PRINTX("TripleClick - New brightness: ", brightness);
  FastLED.setBrightness(brightness); 
}

void loop() {
  random16_add_entropy(random());
  
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
