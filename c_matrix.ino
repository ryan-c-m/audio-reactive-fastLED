#include <FastLED.h>
#include <filter.h> //https://github.com/timonbraun02/digital_filter_arduino
#include "data.h"

CRGB leds[NUM_LEDS];

lowpass_filter my_filter_1(LPF_HZ);
lowpass_filter my_filter_2; 

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  for (int i = 0; i < dataSize; i++) {
      leds[data[i].num] = data[i].color;
  }
  FastLED.show();
}

const int sampleWindow = 50;
int floori = 10;
int ceili = 20;

void updateLEDArray(int* ledArray, int arraySize, int v, CRGB highColor, CRGB lowColor, int fade) {
  for (int i = 0; i < arraySize; i++) {
    int ledIndex = ledArray[i];
    if (v > 50) {
      leds[ledIndex] = CRGB(highColor.r * v / 255, highColor.g * v / 255, highColor.b * v / 255);
    } else {
      leds[ledIndex].nscale8(fade);
    }
  }
}

void updateLEDs(int v) {
  updateLEDArray(flash_leds, flashSize, v, CRGB(random(30, 60), random(100, 140), random(100, 150)), CRGB::Black, 10);
  updateLEDArray(eye, eyeSize, v, CRGB(200, 220, 120), CRGB::Black, 230);
}

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

  updateLEDs(v);
  
  leds[0] = CRGB(0,0,0);
  FastLED.show();
}


