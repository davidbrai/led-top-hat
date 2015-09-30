
uint8_t sinelon(uint8_t bpmSpeed, uint8_t fadeAmount)
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, fadeAmount);
//  int pos = beatsin16(bpmSpeed, 0, NUM_LEDS, 4300);
//  leds[pos] += CHSV(gHue, 255, 192);
  int pos = scale16(beat16(12), NUM_LEDS);
  leds[pos] += CHSV(gHue, 255, 192);
  
  return 0;
}
