#include "Patterns.h"

VerticalVU::VerticalVU(CRGB* leds, CRGBPalette16* palette) 
  : Pattern(leds, palette)
{
}

void VerticalVU::fill_gradient_partial(uint8_t from, CRGB c1, uint8_t to, CRGB c2, fract8 fill) {
  fill_gradient_RGB(leds, from, c1, to, c2);
  uint8_t mid = lerp8by8(from, to, 255 - fill);
  if (mid < to) {
    fill_solid(&(leds[mid]), to - mid + 1, CRGB::Black);
  } else {
    fill_solid(&(leds[to]), mid - to + 1, CRGB::Black);
  }
}

void VerticalVU::on_init() 
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  color = ColorFromPalette(*palette, random());
}

void VerticalVU::on_beat(uint8_t beat) 
{
  if (beat % 2) {
    color = ColorFromPalette(*palette, random8());
  }
}

void VerticalVU::on_step(uint8_t beat, uint8_t step) 
{
  uint8_t fill = ease8InOutQuad(ease8InOutQuad(step));
  fill_gradient_partial(
    CHAIN_STAY_L - 1, color,
    DOWN_TUBE_L, color + CRGB(128, 128, 128), fill);
  fill_gradient_partial(
    SEAT_STAY_L, color,
    TOP_TUBE_R - 1, color + CRGB(128, 128, 128), fill);
  copy_left_to_right();
}