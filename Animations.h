
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

uint8_t drops(uint8_t arg0, uint8_t fadeAmount)
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

  leds[ledPosDown(dropStripIndex, dropPos)] += CHSV(dropHue, 255, 192);
  
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

