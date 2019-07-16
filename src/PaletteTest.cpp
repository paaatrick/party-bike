#include "utils.h"
#include "Patterns.h"

PaletteTest::PaletteTest(CRGB* leds, CRGBPalette16* palette) 
  : Pattern(leds, palette)
{
}

void PaletteTest::on_init() 
{
    for (uint8_t i = 0; i < TOP_TUBE_R; i++) {
        leds[i] = ColorFromPalette(*palette, i * 255 / TOP_TUBE_R);
    }
}