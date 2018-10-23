#include "Patterns.h"

Fireworks::Fireworks(CRGB* leds, CRGBPalette16* palette)
  : Pattern(leds, palette)
{
}

void Fireworks::on_step(uint8_t beat, uint8_t step)
{
  uint8_t i;
  fadeToBlackBy(leds, NUM_LEDS, 40);
  for (i = 1; i < TOP_TUBE_R - 1; i++) {
    if (beat % 2 == 0 && random8() < 10) {
      leds[i] = ColorFromPalette(*palette, random8());
    } else {
      uint8_t center = TOP_TUBE_R + i;
      uint8_t left = center - 1;
      uint8_t right = center + 1;
      CRGB tmp = (leds[left] > leds[right]) ? leds[left] : leds[right];
      leds[i] = leds[center] > tmp ? leds[center] : tmp;
    }
  }
  copy_left_to_right();
}