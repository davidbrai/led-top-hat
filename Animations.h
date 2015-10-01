
uint8_t sinelon(uint8_t bpmSpeed, uint8_t fadeAmount)
{
  fadeToBlackBy(leds, NUM_LEDS, fadeAmount);
  int pos = scale16(beat16(12), NUM_LEDS);
  leds[pos] += CHSV(gHue, 255, 192);
  
  return 0;
}

static uint8_t dropStripIndex = 0;
static uint8_t dropPos = 0;
static uint8_t dropHue = 0;

uint8_t drops(uint8_t colorful, uint8_t fadeAmount)
{
  fadeToBlackBy(leds, NUM_LEDS, fadeAmount);

  EVERY_N_MILLIS(100) {
    dropPos++;
    if (dropPos >= strips[dropStripIndex].mLength) {
      dropPos = 0;
      dropStripIndex = (dropStripIndex + 1 + random8(numberOfStrips-1)) % numberOfStrips;
      dropHue = random8();
    }
  }

  uint8_t saturation = colorful ? 255 : 0;
  leds[ledPosDown(dropStripIndex, dropPos)] += CHSV(dropHue, saturation, 192);
  
  return 0;
}

static uint8_t spiralStripIndex = 0;
static uint8_t spiralPos = 0;

uint8_t spiral(uint8_t arg0, uint8_t fadeAmount)
{
  fadeToBlackBy(leds, NUM_LEDS, fadeAmount);

  EVERY_N_MILLIS(100) {
    spiralStripIndex++;
    if (spiralStripIndex >= numberOfStrips) {
      spiralStripIndex = 0;
    }
  }

  EVERY_N_MILLIS(1000) {
    spiralPos++;
    if (spiralPos >= strips[spiralStripIndex].mLength) {
      spiralPos = 0;
    }
  }

  uint8_t x = ledPosUp(spiralStripIndex, spiralPos);
  leds[x] += CHSV(gHue, 255, 192);
  
  return 0;
}


static uint8_t circleStripIndex = 0;
static uint8_t circleStripIndex2 = 0;

uint8_t movingCircle(uint8_t arg0, uint8_t arg1)
{
  fadeToBlackBy(leds, NUM_LEDS, 8);
  
  EVERY_N_MILLIS(100) {
    circleStripIndex = (circleStripIndex + 1) % numberOfStrips;
    circleStripIndex2 = (circleStripIndex2 + (numberOfStrips-1)) % numberOfStrips;
  }

  Strip strip = strips[circleStripIndex];
  for (int i = strip.mStart; i < strip.mStart + strip.mLength; i++) {
    leds[i] += CHSV(gHue, 255, 192);
  }

  strip = strips[circleStripIndex2];
  for (int i = strip.mStart; i < strip.mStart + strip.mLength; i++) {
    leds[i] += CHSV(gHue, 255, 192);
  }
  
  return 0;
}

