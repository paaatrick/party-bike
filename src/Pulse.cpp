#include "Patterns.h"

Pulse::Pulse(CRGB* leds, CRGBPalette16* palette)
  : Pattern(leds, palette)
{
}

void Pulse::on_step(uint8_t beat, uint8_t step)
{
  static uint8_t last_fades[] = {0, 0};
  static bool last_deltas[] = {false, false};
  static uint8_t color_indices[] = {0, 128};
  uint8_t times[2];
  uint8_t fades[2];
  uint8_t i;

  times[0] = (step >> 2) | (beat << 6);
  times[1] = times[0] + 128;

  for (i = 0; i < 2; i++) {
    fades[i] = scale8(triwave8(times[i]), 230) + 20;
    bool delta = last_fades[i] > fades[i];
    if (!delta && last_deltas[i]) {
      color_indices[i] += 73;
    }
    last_deltas[i] = delta;
    last_fades[i] = fades[i];
  }
  CRGB c1 = ColorFromPalette(*palette, color_indices[0], fades[0]);
  CRGB c2 = ColorFromPalette(*palette, color_indices[1], fades[1]);
  fill_solid(&leds[SEAT_STAY_L], TOP_TUBE_R - SEAT_STAY_L, c1);
  fill_solid(&leds[TOP_TUBE_L], DOWN_TUBE_L - TOP_TUBE_L, c1);
  fill_solid(&leds[DOWN_TUBE_L], SEAT_STAY_L - DOWN_TUBE_L, c2);
  copy_left_to_right();
}
