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

int floori = 10;
int ceili = 20;

void updateLEDArray(int* ledArray, int arraySize, int v, CRGB highColor, CRGB lowColor, int fade, int b) {
  for (int i = 0; i < arraySize; i++) {
    int ledIndex = ledArray[i];
    if (v > 100) {
      leds[ledIndex] = CRGB((highColor.r * v / b), (highColor.g * v / b), (highColor.b * v / b));
    } else {
      leds[ledIndex].nscale8(fade);
    }
  }
}

void updateLEDs(int v) {
  updateLEDArray(flash_leds, flashSize, v, CRGB(random(30, 60), random(100, 140), random(100, 150)), CRGB::Black, 230, 1000);
  updateLEDArray(eye, eyeSize, v, CRGB(255, 255, 0), CRGB::Black, 50, 255);
}

void loop() {
  unsigned long startMillis = millis();
  unsigned int peakToPeak = 0;  
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  if (millis() < 2000) {
    return;
  }

  // Run amplitude sample window
  while (millis() - startMillis < SAMPLE_WINDOW)
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
    
  if (peakToPeak < floori) {
    floori = peakToPeak;
  } else if (peakToPeak > ceili) {
    ceili = peakToPeak;
  } 
  
  
  // Scale latest amplitude to known floor and ceiling values
  int v = constrain(map(peakToPeak, floori, ceili, 1, 255), 1, 255);

  updateLEDs(v);
  
  leds[0] = CRGB(0,0,0);
  FastLED.show();
}


