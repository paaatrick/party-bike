#include <Arduino.h>
#include <FastLED.h>

#define TOP_TUBE_L    0
#define DOWN_TUBE_L   30
#define CHAIN_STAY_L  60
#define SEAT_STAY_L   79
#define TOP_TUBE_R    105
#define DOWN_TUBE_R   135
#define CHAIN_STAY_R  165
#define SEAT_STAY_R   184
#define NUM_LEDS      210

#define DATA_PIN           6
#define BRIGHTNESS         128
#define BEATS_PER_MINUTE   100
#define FRAMES_PER_SECOND  60
#define CYCLE_TIME         20

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

CRGB leds[NUM_LEDS];
CRGBPalette16 palette;
CRGB currentColor;
CRGB currentColor2;
CRGB currentColor3;

extern const TProgmemPalette16 GoldPalette_p PROGMEM;
extern const TProgmemPalette16 RosePalette_p PROGMEM;
extern const TProgmemPalette16 MardiGrasPalette_p PROGMEM;
extern const TProgmemPalette16 CMYPalette_p PROGMEM;

uint8_t last_beat = 0;
uint8_t current_palette_idx = 0;
uint8_t current_function_idx = 0;

void copy_left_to_right() {
  memmove(&leds[TOP_TUBE_R], &leds[TOP_TUBE_L], TOP_TUBE_R * sizeof(CRGB));
}

void fill_with_current_palette(uint8_t from, uint8_t to) {
  uint8_t i;
  for (i = from; i < to; i++) {
    leds[i] = ColorFromPalette(palette, i * 255 / TOP_TUBE_R);
  }
}

void fill_in_and_out(uint8_t measure, uint8_t beat) {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  if (measure % 2 == 0) {
    fill_with_current_palette(TOP_TUBE_L,   TOP_TUBE_L + scale8(DOWN_TUBE_L - TOP_TUBE_L, beat));
    fill_with_current_palette(DOWN_TUBE_L,  DOWN_TUBE_L + scale8(CHAIN_STAY_L - DOWN_TUBE_L, beat));
    fill_with_current_palette(CHAIN_STAY_L, CHAIN_STAY_L + scale8(SEAT_STAY_L - CHAIN_STAY_L, beat));
    fill_with_current_palette(SEAT_STAY_L,  SEAT_STAY_L + scale8(TOP_TUBE_R - SEAT_STAY_L, beat));
  } else {
    fill_with_current_palette(TOP_TUBE_L + scale8(DOWN_TUBE_L - TOP_TUBE_L, beat), DOWN_TUBE_L);
    fill_with_current_palette(DOWN_TUBE_L + scale8(CHAIN_STAY_L - DOWN_TUBE_L, beat), CHAIN_STAY_L);
    fill_with_current_palette(CHAIN_STAY_L + scale8(SEAT_STAY_L - CHAIN_STAY_L, beat), SEAT_STAY_L);
    fill_with_current_palette(SEAT_STAY_L + scale8(TOP_TUBE_R - SEAT_STAY_L, beat), TOP_TUBE_R);
  }
  copy_left_to_right();
}

void fill_noise(uint8_t measure, uint8_t beat) {
  uint16_t i;
  const uint8_t scale = 10;
  const uint8_t speed = 15;
  static uint16_t layer = 0;
  for (i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, inoise8(scale * i, layer));
  }
  layer += speed;
}

void sine_sweep(uint8_t measure, uint8_t beat) {
  fadeToBlackBy(leds, TOP_TUBE_R, 40);
  uint8_t t = (beat >> 2) | (measure << 6);
  uint8_t pos = scale8(quadwave8(t), TOP_TUBE_R);
  uint8_t i;
  for (i = 0; i < TOP_TUBE_R; i++) {
    if (abs(i - pos) < 5) {
      leds[i] = ColorFromPalette(palette, i * 255 / TOP_TUBE_R);
    }
  }
  copy_left_to_right();
}

void fireworks(uint8_t measure, uint8_t beat) {
  uint8_t i;
  fadeToBlackBy(leds, NUM_LEDS, 40);
  for (i = 1; i < TOP_TUBE_R - 1; i++) {
    if (measure % 2 == 0 && random8() < 10) {
      leds[i] = ColorFromPalette(palette, random8());
    } else {
      uint8_t center = TOP_TUBE_R + i;
      uint8_t left = center - 1;
      uint8_t right = center + 1;
      CRGB tmp = (leds[left] > leds[right]) ? leds[left] : leds[right];
      leds[i] = leds[center] > tmp ? leds[center] : tmp;
    }
  }
  copy_left_to_right();
}

void blocks_step(uint8_t measure, uint8_t beat) {
  uint8_t i;
  for (i = 0; i < NUM_LEDS; i++) {
    leds[i] = leds[(i + 1) % NUM_LEDS];
  }
}

void blocks_beat(uint8_t measure, uint8_t beat) {
  uint8_t i;
  static CRGB color = ColorFromPalette(palette, random8());
  for (i = 0; i < NUM_LEDS; i++) {
    if (random8() < 15) {
      color = ColorFromPalette(palette, random8());
    }
    leds[i] = color;
  }
}

void spin(uint8_t measure, uint8_t beat) {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  const uint8_t num = 5;
  const uint8_t width = 3;
  uint8_t max = TOP_TUBE_R / num;
  uint8_t pos = scale8(beat, max);
  uint8_t i, j;
  for (i = 0; i < max; i++) {
    if (abs(pos - i) < width) {
      for (j = 0; j < num; j++) {
        leds[(i + j * max) % TOP_TUBE_R] = ColorFromPalette(palette, i * 255 / max);
      }
    }
  }
  copy_left_to_right();
}

void loading(uint8_t measure, uint8_t beat) {
  const uint8_t gap = 32;
  const uint8_t speed = 1;
  static uint8_t rot = 1;
  uint8_t start;
  uint8_t end;
  uint8_t i;
  uint8_t f = ease8InOutQuad(ease8InOutQuad((beat >> 1) | (measure << 7)));
  if (measure < 2) {
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
      leds[i] = currentColor;
    }
  } else {
    for (i = start; i < TOP_TUBE_R; i++) {
      leds[i] = currentColor;
    }
    for (i = 0; i < end; i++) {
      leds[i] = currentColor;
    }
  }
  copy_left_to_right();
}

void pick_color(uint8_t measure, uint8_t beat) {
  static uint8_t cidx = 0;
  currentColor = ColorFromPalette(palette, cidx);
  if (measure == 0) {
    cidx += 37;
  }
}

void fill_gradient_partial(uint8_t from, CRGB c1, uint8_t to, CRGB c2, fract8 fill) {
  fill_gradient_RGB(leds, from, c1, to, c2);
  uint8_t mid = lerp8by8(from, to, 255 - fill);
  if (mid < to) {
    fill_solid(&(leds[mid]), to - mid + 1, CRGB::Black);
  } else {
    fill_solid(&(leds[to]), mid - to + 1, CRGB::Black);
  }
}

void vu_vert_beat(uint8_t measure, uint8_t beat) {
  if (measure % 2) {
    currentColor = ColorFromPalette(palette, random8());
  }
}

void vu_vert_step(uint8_t measure, uint8_t beat) {
  uint8_t fill = ease8InOutQuad(ease8InOutQuad(beat));
  fill_gradient_partial(
    CHAIN_STAY_L - 1, currentColor,
    DOWN_TUBE_L, currentColor + CRGB(128, 128, 128), fill);
  fill_gradient_partial(
    SEAT_STAY_L, currentColor,
    TOP_TUBE_R - 1, currentColor + CRGB(128, 128, 128), fill);
  copy_left_to_right();
}

void pulse_step(uint8_t measure, uint8_t beat) {
  static uint8_t last_fades[] = {0, 0};
  static bool last_deltas[] = {false, false};
  static uint8_t color_indices[] = {0, 128};
  uint8_t times[2];
  uint8_t fades[2];
  uint8_t i;

  times[0] = (beat >> 2) | (measure << 6);
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
  CRGB c1 = ColorFromPalette(palette, color_indices[0], fades[0]);
  CRGB c2 = ColorFromPalette(palette, color_indices[1], fades[1]);
  fill_solid(&leds[SEAT_STAY_L], TOP_TUBE_R - SEAT_STAY_L, c1);
  fill_solid(&leds[TOP_TUBE_L], DOWN_TUBE_L - TOP_TUBE_L, c1);
  fill_solid(&leds[DOWN_TUBE_L], SEAT_STAY_L - DOWN_TUBE_L, c2);
  copy_left_to_right();
}

void sparkle_step(uint8_t measure, uint8_t beat) {
  const uint8_t sparkle_stength = 175;
  const uint8_t sparkle_rate = 50;
  const uint8_t charge_time = 192;
  const uint8_t fill_time = charge_time - 20;
  const uint8_t charge_size = 8;
  const uint8_t white_boost = 75;
  const uint8_t charge_fade = 3;
  const uint8_t tail_fade = 2;

  static uint8_t last_t = 255;
  uint8_t t = (beat >> 1) | (measure << 7);
  if (t < last_t) {
    currentColor = ColorFromPalette(palette, random8());
  }
  last_t = t;
  uint8_t i;
  uint8_t N = SEAT_STAY_L - DOWN_TUBE_L;
  for (i = 0; i < N; i++) {
    uint8_t x = (uint16_t)(i) * 256 / N;
    x = charge_time + scale8(x, 256 - charge_time);
    uint8_t fade = qmul8(charge_fade, t - x);
    uint8_t boost = qsub8(white_boost, fade);
    leds[DOWN_TUBE_L + i] = (currentColor + CRGB(boost, boost, boost)).fadeToBlackBy(fade);
    if (random8() < sparkle_rate) {
      uint8_t sparkle = qsub8(sparkle_stength, fade);
      leds[DOWN_TUBE_L + i].addToRGB(sparkle);
    }
  }
  if (t < charge_time) {
    for (i = 0; i < charge_size; i++) {
      uint8_t fade = qmul8(tail_fade, qsub8(fill_time, t));
      uint8_t boost = qsub8(white_boost, fade);
      leds[DOWN_TUBE_L + i] |= (currentColor + CRGB(boost, boost, boost)).fadeToBlackBy(fade);
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
    leds[idx - 1] = (currentColor + CRGB(boost, boost, boost)).fadeToBlackBy(fade);
    if (random8() < sparkle_rate) {
      uint8_t sparkle = qsub8(sparkle_stength, fade);
      leds[idx - 1].addToRGB(sparkle);
    }
  }
  if (t < charge_time) {
    for (i = 0; i < charge_size; i++) {
      uint8_t fade = qmul8(tail_fade, qsub8(fill_time, t));
      uint8_t boost = qsub8(white_boost, fade);
      leds[DOWN_TUBE_L - i - 1] |= (currentColor + CRGB(boost, boost, boost)).fadeToBlackBy(fade);
      if (random8() < sparkle_rate) {
        uint8_t sparkle = qsub8(sparkle_stength, fade);
        leds[DOWN_TUBE_L - i - 1].addToRGB(sparkle);
      }
    }
  }
  copy_left_to_right();
}

void triple_chase_step(uint8_t measure, uint8_t beat) {
  fadeToBlackBy(leds, TOP_TUBE_R, 40);
  uint8_t t = (beat >> 2) | (measure << 6);
  uint8_t x1 = scale8(t, 192);
  uint8_t x2 = scale8(qsub8(t, 32), 192);
  uint8_t x3 = scale8(qsub8(t, 64), 192);

  uint8_t i;
  for (i = 0; i < TOP_TUBE_R; i++) {
    if (x1 > 0 && abs(i - x1) < 3) {
      leds[i] = currentColor;
    }
    if (x2 > 0 && abs(i - x2) < 3) {
      leds[i] = currentColor2;
    }
    if (x3 > 0 && abs(i - x3) < 3) {
      leds[i] = currentColor3;
    }
  }
  copy_left_to_right();
}

void triple_chase_beat(uint8_t measure, uint8_t beat) {
  static uint8_t cidx = 0;
  if (measure == 0) {
    currentColor = ColorFromPalette(palette, cidx);
    currentColor2 = ColorFromPalette(palette, cidx + 96);
    currentColor3 = ColorFromPalette(palette, cidx + 192);
    cidx += 57;
  }
}

void waterfall_step(uint8_t measure, uint8_t beat) {
  fadeToBlackBy(leds, TOP_TUBE_R, 40);
  uint8_t t = (beat >> 2) | (measure << 6);
  uint8_t pos1 = scale8(t, SEAT_STAY_L - DOWN_TUBE_L - 1) + DOWN_TUBE_L;
  uint8_t pos2 = scale8(t - 85, SEAT_STAY_L - DOWN_TUBE_L - 1) + DOWN_TUBE_L;
  uint8_t pos3 = scale8(t - 170, SEAT_STAY_L - DOWN_TUBE_L - 1) + DOWN_TUBE_L;
  leds[pos1] = CRGB::Cyan;
  leds[pos2] = CRGB::Yellow;
  leds[pos3] = CRGB::Magenta;
}


void no_op(uint8_t measure, uint8_t beat) {}

typedef void (*UpdateFunction)(uint8_t, uint8_t);
struct functions {
  UpdateFunction step;
  UpdateFunction beat;
};

struct functions function_list[] = {
  {loading, pick_color},                 // 1
  {spin, no_op},                         // 2
  {blocks_step, blocks_beat},            // 3
  {fireworks, no_op},                    // 4
  {fill_in_and_out, no_op},              // 5
  {fill_noise, no_op},                   // 6
  {sine_sweep, no_op},                   // 7
  {vu_vert_step, vu_vert_beat},          // 8
  {sparkle_step, no_op},                 // 9
  {triple_chase_step, triple_chase_beat},// 10
  {pulse_step, no_op},                   // 11
  // {waterfall_step, no_op}
};

const CRGBPalette16 palettes[] = {
  MardiGrasPalette_p, // 1
  RosePalette_p,      // 2
  CMYPalette_p,       // 3
  GoldPalette_p,      // 4
  CloudColors_p,      // 5
  PartyColors_p,      // 6
  ForestColors_p,     // 7
  RainbowColors_p,    // 8
  OceanColors_p,      // 9
  HeatColors_p,       // 10
};

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  palette = palettes[0];
}

void loop() {
  EVERY_N_SECONDS( CYCLE_TIME ) {
    current_palette_idx = (current_palette_idx + 1) % ARRAY_SIZE(palettes);
    palette = palettes[current_palette_idx];
    current_function_idx = (current_function_idx + 1) % ARRAY_SIZE(function_list);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
  }
  uint8_t beat = beat8(BEATS_PER_MINUTE);
  static uint8_t measure = 0;
  if (last_beat > beat) {
    measure = (measure + 1) % 4;
    function_list[current_function_idx].beat(measure, beat);
  }
  function_list[current_function_idx].step(measure, beat);
  last_beat = beat;
  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

const TProgmemPalette16 GoldPalette_p PROGMEM = {
    0xD6B708,
    0xD6B708,
    0xD6B708,
    0xD6B708,
    0xEDD63E,
    0xEDD63E,
    0xEDD63E,
    0xEDD63E,
    0xF1EC76,
    0xF1EC76,
    0xF1EC76,
    0xF1EC76,
    0xCC7008,
    0xCC7008,
    0xCC7008,
    0xCC7008,
};

const TProgmemPalette16 RosePalette_p PROGMEM = {
    0x8A0BDE,
    0x8A0BDE,
    0x8A0BDE,
    0x8A0BDE,
    0xFF00FF,
    0xFF00FF,
    0xFF00FF,
    0xFF00FF,
    0xE30A5E,
    0xE30A5E,
    0xE30A5E,
    0xE30A5E,
    0x8A0BDE,
    0x8A0BDE,
    0x8A0BDE,
    0x8A0BDE,
};

const TProgmemPalette16 MardiGrasPalette_p PROGMEM = {
    0xD6D014,
    0xD6D014,
    0xD6D014,
    0xD6D014,
    0x000000,
    0x00C207,
    0x00C207,
    0x00C207,
    0x00C207,
    0x00C207,
    0x000000,
    0xB2009B,
    0xB2009B,
    0xB2009B,
    0xB2009B,
    0x000000,
};

const TProgmemPalette16 CMYPalette_p PROGMEM = {
    0xFF00FF,
    0xFF00FF,
    0xFF00FF,
    0xFF00FF,
    0x000000,
    0xFFFF00,
    0xFFFF00,
    0xFFFF00,
    0xFFFF00,
    0xFFFF00,
    0x000000,
    0x00FFFF,
    0x00FFFF,
    0x00FFFF,
    0x00FFFF,
    0x000000,
};
