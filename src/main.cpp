#include <Arduino.h>
#include <FastLED.h>

#include "constants.h"
#include "Patterns.h"
#include "palettes.h"

CRGB leds[NUM_LEDS];
CRGBPalette16 palette;

const CRGBPalette16 palettes[] = {
  MichiganPalette_p,  // 1
  MardiGrasPalette_p, // 2
  RosePalette_p,      // 3
  CMYPalette_p,       // 4
  GoldPalette_p,      // 5
  CloudColors_p,      // 6
  PartyColors_p,      // 7
  ForestColors_p,     // 8
  RainbowColors_p,    // 9
  OceanColors_p,      // 10
  HeatColors_p,       // 11
  HalloweenPalette_p  // 12
};

Pattern* patterns[] = {
  new Blink(leds, &palette),            // 1
  new Pulse(leds, &palette),            // 2
  new TripleStep(leds, &palette),       // 3
  new Sparkle(leds, &palette),          // 4
  new VerticalVU(leds, &palette),       // 5
  new SineSweep(leds, &palette),        // 6
  new Noise(leds, &palette),            // 7
  new FillInOut(leds, &palette),        // 8
  new Fireworks(leds, &palette),        // 9
  new Blocks(leds, &palette),           // 10
  new Spin(leds, &palette),             // 11
  new Loading(leds, &palette),          // 12
  new StartStopBlocks(leds, &palette),  // 13
};

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  palette = palettes[0];
}

void loop() {
  static uint8_t pattern_idx = 0;
  static uint8_t palette_idx = 0;
  static uint8_t measure = 0;
  static uint8_t last_beat = 0;
  static Pattern* pattern = patterns[0];

  uint8_t beat = beat8(BEATS_PER_MINUTE);
  EVERY_N_SECONDS( CYCLE_TIME ) {
    palette_idx = addmod8(palette_idx, 1, ARRAY_SIZE(palettes));
    pattern_idx = addmod8(pattern_idx, 1, ARRAY_SIZE(patterns));
    palette = palettes[palette_idx];
    pattern = patterns[pattern_idx];
    pattern->on_init();
  }
  if (last_beat > beat) {
    measure = (measure + 1) % 4;
    pattern->on_beat(measure);
  }
  pattern->on_step(measure, beat);
  last_beat = beat;
  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}
