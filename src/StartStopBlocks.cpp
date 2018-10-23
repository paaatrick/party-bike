#include "Patterns.h"

StartStopBlocks::StartStopBlocks(CRGB* leds, CRGBPalette16* palette) 
  : Pattern(leds, palette)
  , cidx(0)
{
}

void StartStopBlocks::on_init() 
{
  color1 = ColorFromPalette(*palette, cidx);
  color2 = ColorFromPalette(*palette, cidx + 128);
  start1 = TOP_TUBE_L;
  start2 = DOWN_TUBE_L;
  end1 = start1 + random8(speed1);
  end2 = start2 + random8(speed2);
}

void StartStopBlocks::on_beat(uint8_t beat) 
{
  if (beat == 0) {
    cidx += 57;
    color1 = ColorFromPalette(*palette, cidx);
    color2 = ColorFromPalette(*palette, cidx + 128);
  }
  start1 = end1;
  start2 = end2;
  end1 = start1 + random8(speed1);
  end2 = start2 + random8(speed2);
}

void StartStopBlocks::on_step(uint8_t beat, uint8_t step) 
{
  fadeToBlackBy(leds, NUM_LEDS, 96);
  uint8_t position1 = map8(ease8InOutCubic(step), start1, end1);
  uint8_t position2 = map8(ease8InOutCubic(step), start2, end2);
  uint8_t index1 = map8(position1, 0, TOP_TUBE_R);
  uint8_t index2 = TOP_TUBE_R - map8(position2, 0, TOP_TUBE_R);
  uint8_t i;
  for (i = 0; i < width1; i++) {
    leds[(index1 + i) % TOP_TUBE_R] = color1;
  }
  for (i = 0; i < width2; i++) {
    leds[(index2 + i) % TOP_TUBE_R] = color2;
  }
}