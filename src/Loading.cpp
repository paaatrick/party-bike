#include "Patterns.h"

Loading::Loading(CRGB* leds, CRGBPalette16* palette)
  : Pattern(leds, palette)
{
}

void Loading::on_beat(uint8_t beat) 
{
  static uint8_t cidx = 0;
  color = ColorFromPalette(*palette, cidx);
  if (beat == 0) {
    cidx += 37;
  }
}

void Loading::on_step(uint8_t beat, uint8_t step) 
{
  const uint8_t gap = 32;
  const uint8_t speed = 1;
  static uint8_t rot = 1;
  uint8_t start;
  uint8_t end;
  uint8_t i;
  uint8_t f = ease8InOutQuad(ease8InOutQuad((step >> 1) | (beat << 7)));
  if (beat < 2) {
    start = scale8(f, gap);
    end = gap + scale8(f, 256 - gap);
  } else {
    start = gap + scale8(f, 256 - gap);
    end = scale8(f, gap);
  }
  start = scale8(start + rot, TOP_TUBE_R);
  end = scale8(end + rot, TOP_TUBE_R);
  rot += speed;

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  if (start <= end) {
    for (i = start; i < end; i++) {
      leds[i] = color;
    }
  } else {
    for (i = start; i < TOP_TUBE_R; i++) {
      leds[i] = color;
    }
    for (i = 0; i < end; i++) {
      leds[i] = color;
    }
  }
  copy_left_to_right();
}