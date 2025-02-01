#ifndef DATA_H
#define DATA_H

#include <FastLED.h>

#define NUM_LEDS 256
#define DATA_PIN 6
#define AMP_PIN A0
#define LPF_HZ 150
#define BRIGHTNESS 20

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
extern int eyeSize;
#endif
