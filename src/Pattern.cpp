#include "Patterns.h"

Pattern::Pattern(CRGB* leds, CRGBPalette16* palette)
  : leds(leds)
  , palette(palette)
{
}

void Pattern::on_init() 
{
}

void Pattern::on_beat(uint8_t beat) 
{
}

void Pattern::on_step(uint8_t beat, uint8_t step) 
{
}

void Pattern::copy_left_to_right()
{
  memmove(&leds[TOP_TUBE_R], &leds[TOP_TUBE_L], TOP_TUBE_R * sizeof(CRGB));
}