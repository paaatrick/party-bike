#include "Patterns.h"

Blink::Blink(CRGB* leds, CRGBPalette16* palette) 
  : Pattern(leds, palette)
{
}

void Blink::on_init() 
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

void Blink::on_beat(uint8_t beat) 
{
  uint8_t i = 0;
  CRGB color = ColorFromPalette(*palette, random8());
  for (i = 0; i < NUM_LEDS; i++) {
    if (random8() < 64) {
      leds[i] = color;
    }
  }
}

void Blink::on_step(uint8_t beat, uint8_t step) 
{
  fadeLightBy(leds, NUM_LEDS, 12);
}