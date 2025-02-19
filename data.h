#ifndef DATA_H
#define DATA_H

#include <FastLED.h>

#define NUM_LEDS 256
#define DATA_PIN 6
#define AMP_PIN A1
#define LPF_HZ 80
#define BRIGHTNESS 200
#define SAMPLE_WINDOW 10
#define BEAT_THRESHOLD 0.6

struct Data {
  uint8_t num;      // LED number
  CRGB color;   // Color of the LED
};

extern Data data_group_1[];
extern int dataSize;
extern Data flash_leds[];
extern int flashSize;
extern Data eye[];
extern int eyeSize;
#endif
