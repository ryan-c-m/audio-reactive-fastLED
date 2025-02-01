#include <FastLED.h>
#include <filter.h> //https://github.com/timonbraun02/digital_filter_arduino
#include "data.h"

CRGB leds[NUM_LEDS];

lowpass_filter my_filter_1(150);
lowpass_filter my_filter_2; 

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  for (int i = 0; i < dataSize; i++) {
      leds[data[i].num] = data[i].color;
  }
  FastLED.show();
}

const int sampleWindow = 50;
int floori = 10;
int ceili = 20;
int const AMP_PIN = A0;    

void loop() {
  unsigned long startMillis = millis();
  unsigned int peakToPeak = 0;  
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // Run amplitude sample window
  while (millis() - startMillis < sampleWindow)
  {
    unsigned int sample = analogRead(AMP_PIN);

    // Apply LPF to the sample
    sample = my_filter_1.filter(sample); 

    if (sample < 1024)  
    {
      if (sample > signalMax)
      {
        signalMax = sample;  
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  
      }
    }
  }
  
  // Set new peak to peak value (amplitude)
  peakToPeak = signalMax - signalMin;  

  if (millis() > 4000) {
    if (peakToPeak < floori) {
      floori = peakToPeak;
    } else if (peakToPeak > ceili) {
      ceili = peakToPeak;
    } 
  }
  
  // Scale latest amplitude to known floor and ceiling values
  int v = constrain(map(peakToPeak, floori, ceili, 1, 255), 1, 255);

  for (int i = 0; i < 63; i++) {
      int ledIndex = flash_leds[i];
      if (v > 50) {
        leds[ledIndex] = CRGB(random(30,50) * v / 255, random(120,140) * v / 255, random(120,150) * v / 255);
      } else {
        leds[ledIndex].nscale8(random(10, 254));
      }
  }

  for (int i = 0; i < 6; i++) {
    int ledIndex = eye[i];
    if (v > 50) {
        leds[ledIndex] = CRGB(60 * v / 255, 220 * v / 255, 120 * v / 255);
    } else {
        leds[ledIndex].nscale8(5);
    }
  }
  leds[0] = CRGB(0,0,0);
  FastLED.show();
}


