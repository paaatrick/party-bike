#include "Patterns.h"

Sparkle::Sparkle(CRGB* leds, CRGBPalette16* palette)
  : Pattern(leds, palette)
{
}

void Sparkle::on_init()
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  color = ColorFromPalette(*palette, random8());
}

void Sparkle::on_step(uint8_t beat, uint8_t step)
{
  const uint8_t sparkle_stength = 175;
  const uint8_t sparkle_rate = 50;
  const uint8_t charge_time = 192;
  const uint8_t fill_time = charge_time - 20;
  const uint8_t charge_size = 8;
  const uint8_t white_boost = 75;
  const uint8_t charge_fade = 3;
  const uint8_t tail_fade = 2;

  static uint8_t last_t = 255;
  uint8_t t = (step >> 1) | (beat << 7);
  if (t < last_t) {
    color = ColorFromPalette(*palette, random8());
  }
  last_t = t;
  uint8_t i;
  uint8_t N = SEAT_STAY_L - DOWN_TUBE_L;
  for (i = 0; i < N; i++) {
    uint8_t x = (uint16_t)(i) * 256 / N;
    x = charge_time + scale8(x, 256 - charge_time);
    uint8_t fade = qmul8(charge_fade, t - x);
    uint8_t boost = qsub8(white_boost, fade);
    leds[DOWN_TUBE_L + i] = (color + CRGB(boost, boost, boost)).fadeToBlackBy(fade);
    if (random8() < sparkle_rate) {
      uint8_t sparkle = qsub8(sparkle_stength, fade);
      leds[DOWN_TUBE_L + i].addToRGB(sparkle);
    }
  }
  if (t < charge_time) {
    for (i = 0; i < charge_size; i++) {
      uint8_t fade = qmul8(tail_fade, qsub8(fill_time, t));
      uint8_t boost = qsub8(white_boost, fade);
      leds[DOWN_TUBE_L + i] |= (color + CRGB(boost, boost, boost)).fadeToBlackBy(fade);
      if (random8() < sparkle_rate) {
        uint8_t sparkle = qsub8(sparkle_stength, fade);
        leds[DOWN_TUBE_L + i].addToRGB(sparkle);
      }
    }
  }

  N = DOWN_TUBE_L + TOP_TUBE_R - SEAT_STAY_L;
  for (i = 0; i < N; i++) {
    uint8_t x = (uint16_t)(i) * 256 / N;
    x = charge_time + scale8(x, 256 - charge_time);
    uint8_t fade = qmul8(charge_fade, t - x);
    uint8_t boost = qsub8(white_boost, fade);
    uint8_t idx;
    if (i > DOWN_TUBE_L) {
      idx = TOP_TUBE_R - (i - DOWN_TUBE_L);
    } else {
      idx = DOWN_TUBE_L - i;
    }
    leds[idx - 1] = (color + CRGB(boost, boost, boost)).fadeToBlackBy(fade);
    if (random8() < sparkle_rate) {
      uint8_t sparkle = qsub8(sparkle_stength, fade);
      leds[idx - 1].addToRGB(sparkle);
    }
  }
  if (t < charge_time) {
    for (i = 0; i < charge_size; i++) {
      uint8_t fade = qmul8(tail_fade, qsub8(fill_time, t));
      uint8_t boost = qsub8(white_boost, fade);
      leds[DOWN_TUBE_L - i - 1] |= (color + CRGB(boost, boost, boost)).fadeToBlackBy(fade);
      if (random8() < sparkle_rate) {
        uint8_t sparkle = qsub8(sparkle_stength, fade);
        leds[DOWN_TUBE_L - i - 1].addToRGB(sparkle);
      }
    }
  }
  copy_left_to_right();
}
