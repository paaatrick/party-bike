#include "Patterns.h"

Spin::Spin(CRGB* leds, CRGBPalette16* palette)
  : Pattern(leds, palette)
{
}

void Spin::on_step(uint8_t beat, uint8_t step)
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  const uint8_t num = 5;
  const uint8_t width = 3;
  uint8_t max = TOP_TUBE_R / num;
  uint8_t pos = scale8(step, max);
  uint8_t i, j;
  for (i = 0; i < max; i++) {
    if (abs(pos - i) < width) {
      for (j = 0; j < num; j++) {
        leds[(i + j * max) % TOP_TUBE_R] = ColorFromPalette(*palette, i * 255 / max);
      }
    }
  }
  copy_left_to_right();
}