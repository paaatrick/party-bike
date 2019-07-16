#include "utils.h"
#include "Patterns.h"

FallingStars::FallingStars(CRGB* leds, CRGBPalette16* palette) 
  : Pattern(leds, palette)
  , num_stars(20)
  , cidx(0)
{
}

void FallingStars::on_init() 
{
    for (uint8_t i = 0; i < num_stars; i++) {
        positions[i] = 0;
    }
}

void FallingStars::on_beat(uint8_t beat) 
{
    if (beat == 0) {
        cidx += 61;
    }
}

void FallingStars::on_step(uint8_t beat, uint8_t step) 
{
    fadeToBlackBy(leds, NUM_LEDS, 128);
    for (uint8_t i = 0; i < num_stars; i++) {
        if (positions[i] == 0) {
            if (random8() < 3) {
                positions[i] = 1;
            }
        } else {
            positions[i] = qadd8(positions[i], map8(positions[i], 1, 10));
        }
        if (positions[i] == 255) {
            positions[i] = 0;
        }
    }
    for (uint8_t j = 0; j < num_stars; j++) {
        if (j < num_stars / 2) {
            leds[mapTopPath(positions[j])] = ColorFromPalette(*palette, cidx);
        } else {
            leds[mapBottomPath(positions[j])] = ColorFromPalette(*palette, cidx + 128);
        }
    }
    copy_left_to_right();
}