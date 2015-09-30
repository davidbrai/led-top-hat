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

void setup() {

  DEBUG_START(57600);
  
  FastLED.addLeds<NEOPIXEL, LED_PIN1>(leds, 0, NUM_LEDS1).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<NEOPIXEL, LED_PIN2>(leds, NUM_LEDS1, NUM_LEDS2).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
  
  set_max_power_in_volts_and_milliamps(5, 500);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  show_at_max_brightness_for_power();
  delay(100);
}

void loop() {
  soundAnimate(0, 0);

  show_at_max_brightness_for_power();
}
