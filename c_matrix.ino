#include <FastLED.h>
#include <filter.h> //https://github.com/timonbraun02/digital_filter_arduino
#include "data.h"

CRGB leds[NUM_LEDS];

lowpass_filter lpf_1(LPF_HZ);
lowpass_filter lpf_2(LPF_HZ);

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  for (int i = 0; i < dataSize; i++) {
    leds[data[i].num] = data[i].color;
  }
  analogReference(EXTERNAL);
  FastLED.show();
}

int floori = 10;
int ceili = 20;
int threshold = 255 * BEAT_THRESHOLD;

void updateLEDArray(int* ledArray, int arraySize, int v, CRGB highColor, CRGB lowColor, int fade) {
  for (int i = 0; i < arraySize; i++) {
    int ledIndex = ledArray[i];
    if (v > threshold) {
      leds[ledIndex] = highColor;
    } else {
      leds[ledIndex].nscale8(fade);
    }
  }
}

void updateLEDs(int v) {
  updateLEDArray(flash_leds, flashSize, v, CRGB(random(1, 30), random(1, 30), random(1, 30)), CRGB::Black, 250);
  updateLEDArray(eye, eyeSize, v, CRGB(255, 255, 0), CRGB::Black, 200);
}

void loop() {
  unsigned long startMillis = millis();
  unsigned int peakToPeak = 0;  
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  
  if (millis() < 4000) {
    return;
  }

  // Run amplitude sample window
  while (millis() - startMillis < SAMPLE_WINDOW) {
    unsigned int sample = analogRead(AMP_PIN);

    // Apply LPF to the sample
    sample = lpf_1.filter(lpf_2.filter(sample)); 

    if (sample < 1024) {
      if (sample > signalMax) {
        signalMax = sample;  
      } else if (sample < signalMin) {
        signalMin = sample;  
      }
    }
  }
  
  // Set new peak to peak value (amplitude)
  peakToPeak = signalMax - signalMin;  
    
  if (peakToPeak < floori) {
    floori = peakToPeak;
  } else if (peakToPeak > ceili) {
    ceili = peakToPeak;
  } 
  
  // Scale latest amplitude to known floor and ceiling values
  int v = constrain(map(peakToPeak, floori, ceili, 1, 255), 1, 255);

  updateLEDs(v);
  FastLED.show();
}


