#include "Patterns.h"

Noise::Noise(CRGB* leds, CRGBPalette16* palette)
  : Pattern(leds, palette)
{
}

void Noise::on_step(uint8_t beat, uint8_t step)
{
  uint16_t i;
  const uint8_t scale = 10;
  const uint8_t speed = 8;
  static uint16_t layer = 0;
  for (i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(*palette, inoise8(scale * i, layer));
  }
  layer += speed;
}
