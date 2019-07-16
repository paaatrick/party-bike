#ifndef PATTERNS_H
#define PATTERNS_H

#include <Arduino.h>
#include <FastLED.h>
#include "constants.h"

class Pattern 
{
  protected:
    CRGB* leds;
    CRGBPalette16* palette;
    void copy_left_to_right();

  public:
    Pattern(CRGB* leds, CRGBPalette16* palette);
    virtual void on_init();
    virtual void on_beat(uint8_t beat);
    virtual void on_step(uint8_t beat, uint8_t step);
};

class StartStopBlocks : public Pattern 
{
  private:
    CRGB color1;
    CRGB color2;
    uint8_t start1;
    uint8_t start2;
    uint8_t end1;
    uint8_t end2;
    static const uint8_t width1 = 10;
    static const uint8_t width2 = 18;
    static const uint8_t speed1 = 170;
    static const uint8_t speed2 = 70;
    uint8_t cidx;

  public:
    StartStopBlocks(CRGB* leds, CRGBPalette16* palette);
    void on_init();
    void on_beat(uint8_t beat);
    void on_step(uint8_t beat, uint8_t step);
};

class Loading : public Pattern 
{
  private:
    CRGB color;

  public:
    Loading(CRGB* leds, CRGBPalette16* palette);
    void on_beat(uint8_t beat);
    void on_step(uint8_t beat, uint8_t step);
};

class Spin : public Pattern
{
  public:
    Spin(CRGB* leds, CRGBPalette16* palette);
    void on_step(uint8_t beat, uint8_t step);
};

class Blocks : public Pattern
{
  public:
    Blocks(CRGB* leds, CRGBPalette16* palette);
    void on_beat(uint8_t beat);
    void on_step(uint8_t beat, uint8_t step);
};

class Fireworks : public Pattern
{
  public:
    Fireworks(CRGB* leds, CRGBPalette16* palette);
    void on_step(uint8_t beat, uint8_t step);
};

class FillInOut : public Pattern
{
  private:
    void fill_with_current_palette(uint8_t from, uint8_t to);

  public:
    FillInOut(CRGB* leds, CRGBPalette16* palette);
    void on_step(uint8_t beat, uint8_t step);
};

class Noise : public Pattern
{
  public:
    Noise(CRGB* leds, CRGBPalette16* palette);
    void on_step(uint8_t beat, uint8_t step);
};

class SineSweep : public Pattern
{
  public:
    SineSweep(CRGB* leds, CRGBPalette16* palette);
    void on_step(uint8_t beat, uint8_t step);
};

class VerticalVU : public Pattern
{
  private:
    CRGB color;
    void fill_gradient_partial(uint8_t from, CRGB c1, uint8_t to, CRGB c2, fract8 fill);
    
  public:
    VerticalVU(CRGB* leds, CRGBPalette16* palette);
    void on_init();
    void on_beat(uint8_t beat);
    void on_step(uint8_t beat, uint8_t step);
};

class Sparkle : public Pattern
{
  private:
    CRGB color;

  public:
    Sparkle(CRGB* leds, CRGBPalette16* palette);
    void on_init();
    void on_step(uint8_t beat, uint8_t step);
};

class TripleStep : public Pattern
{
  private:
    uint8_t cidx;
    
  public:
    TripleStep(CRGB* leds, CRGBPalette16* palette);
    void on_init();
    void on_beat(uint8_t beat);
    void on_step(uint8_t beat, uint8_t step);
};

class Pulse : public Pattern
{
  public:
    Pulse(CRGB* leds, CRGBPalette16* palette);
    void on_step(uint8_t beat, uint8_t step);
};

class Blink : public Pattern
{
  public:
    Blink(CRGB* leds, CRGBPalette16* palette);
    void on_init();
    void on_beat(uint8_t beat);
    void on_step(uint8_t beat, uint8_t step);
};

class FallingStars : public Pattern
{
  private:
    uint8_t cidx;
    uint8_t num_stars;
    uint8_t positions[];

  public:
    FallingStars(CRGB* leds, CRGBPalette16* palette);
    void on_init();
    void on_beat(uint8_t beat);
    void on_step(uint8_t beat, uint8_t step);
};

#endif