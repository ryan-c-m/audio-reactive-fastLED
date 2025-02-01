#ifndef DATA_H
#define DATA_H

#include <FastLED.h>

#define NUM_LEDS 256
#define DATA_PIN 6

struct Color {
  uint8_t r; // Red
  uint8_t g; // Green
  uint8_t b; // Blue
};

struct Data {
  int num;      // LED number
  CRGB color;   // Color of the LED
};

extern Data data[];
extern int dataSize;
extern int flash_leds[];
extern int flashSize;
extern int eye[];
#endif
