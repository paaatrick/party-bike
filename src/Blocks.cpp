#include "Patterns.h"

Blocks::Blocks(CRGB* leds, CRGBPalette16* palette)
  : Pattern(leds, palette)
{
}

void Blocks::on_beat(uint8_t beat)
{
  uint8_t i;
  CRGB color = ColorFromPalette(*palette, random8());
  for (i = 0; i < NUM_LEDS; i++) {
    if (random8() < 15) {
      color = ColorFromPalette(*palette, random8());
    }
    leds[i] = color;
  }
}

void Blocks::on_step(uint8_t beat, uint8_t step)
{
  uint8_t i;
  for (i = 0; i < NUM_LEDS; i++) {
    leds[i] = leds[(i + 1) % NUM_LEDS];
  }
}
