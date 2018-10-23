#include "Patterns.h"

SineSweep::SineSweep(CRGB* leds, CRGBPalette16* palette)
  : Pattern(leds, palette)
{
}

void SineSweep::on_step(uint8_t beat, uint8_t step)
{
  fadeToBlackBy(leds, TOP_TUBE_R, 40);
  uint8_t t = (step >> 2) | (beat << 6);
  uint8_t pos = scale8(quadwave8(t), TOP_TUBE_R);
  uint8_t i;
  for (i = 0; i < TOP_TUBE_R; i++) {
    if (abs(i - pos) < 5) {
      leds[i] = ColorFromPalette(*palette, i * 255 / TOP_TUBE_R);
    }
  }
  copy_left_to_right();
}
