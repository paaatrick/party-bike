#include "Patterns.h"

TripleStep::TripleStep(CRGB* leds, CRGBPalette16* palette) 
  : Pattern(leds, palette)
  , cidx(0)
{
}

void TripleStep::on_init() 
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

void TripleStep::on_beat(uint8_t beat) 
{
  if (beat == 0) {
    cidx += 57;
  }
}

void TripleStep::on_step(uint8_t beat, uint8_t step) 
{
  fadeToBlackBy(leds, TOP_TUBE_R, 40);
  uint8_t t = (step >> 2) | (beat << 6);
  uint8_t x1 = scale8(t, 192);
  uint8_t x2 = scale8(qsub8(t, 32), 192);
  uint8_t x3 = scale8(qsub8(t, 64), 192);
  uint8_t i;
  for (i = 0; i < TOP_TUBE_R; i++) {
    if (x1 > 0 && abs(i - x1) < 3) {
      leds[i] = ColorFromPalette(*palette, cidx);
    }
    if (x2 > 0 && abs(i - x2) < 3) {
      leds[i] = ColorFromPalette(*palette, cidx + 96);
    }
    if (x3 > 0 && abs(i - x3) < 3) {
      leds[i] = ColorFromPalette(*palette, cidx + 192);
    }
  }
  copy_left_to_right();
}