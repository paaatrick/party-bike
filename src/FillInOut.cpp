#include "Patterns.h"

FillInOut::FillInOut(CRGB* leds, CRGBPalette16* palette)
  : Pattern(leds, palette)
{
}

void FillInOut::fill_with_current_palette(uint8_t from, uint8_t to) {
  uint8_t i;
  for (i = from; i < to; i++) {
    leds[i] = ColorFromPalette(*palette, i * 255 / TOP_TUBE_R);
  }
}

void FillInOut::on_step(uint8_t beat, uint8_t step)
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  if (beat % 2 == 0) {
    fill_with_current_palette(TOP_TUBE_L,   TOP_TUBE_L + scale8(DOWN_TUBE_L - TOP_TUBE_L, step));
    fill_with_current_palette(DOWN_TUBE_L,  DOWN_TUBE_L + scale8(CHAIN_STAY_L - DOWN_TUBE_L, step));
    fill_with_current_palette(CHAIN_STAY_L, CHAIN_STAY_L + scale8(SEAT_STAY_L - CHAIN_STAY_L, step));
    fill_with_current_palette(SEAT_STAY_L,  SEAT_STAY_L + scale8(TOP_TUBE_R - SEAT_STAY_L, step));
  } else {
    fill_with_current_palette(TOP_TUBE_L + scale8(DOWN_TUBE_L - TOP_TUBE_L, step), DOWN_TUBE_L);
    fill_with_current_palette(DOWN_TUBE_L + scale8(CHAIN_STAY_L - DOWN_TUBE_L, step), CHAIN_STAY_L);
    fill_with_current_palette(CHAIN_STAY_L + scale8(SEAT_STAY_L - CHAIN_STAY_L, step), SEAT_STAY_L);
    fill_with_current_palette(SEAT_STAY_L + scale8(TOP_TUBE_R - SEAT_STAY_L, step), TOP_TUBE_R);
  }
  copy_left_to_right();
}
