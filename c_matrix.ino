#include <FastLED.h>
#include <filter.h> //https://github.com/timonbraun02/digital_filter_arduino
#include "data.h"

// Create struct for LED control state
struct LEDState {
    CRGB leds[NUM_LEDS];
    uint16_t floori;
    uint16_t ceili;
    unsigned long lastColorUpdate;  // Add timestamp
    uint8_t currentHue;  // Add hue tracking
} ledState;

lowpass_filter lpf_1(LPF_HZ);

// Add this helper function
CRGB varyColor(CRGB color, uint8_t amount) {
  // Find dominant channel
  uint8_t max_channel = max(max(color.r, color.g), color.b);
  
  // Calculate variation amounts (smaller for dominant channel)
  uint8_t var_amount_dom = amount / 4;  // smaller variation for dominant channel
  uint8_t var_amount_other = amount / 2;  // medium variation for other channels
  
  CRGB newColor = color;
  // Apply appropriate variation based on whether channel is dominant
  newColor.r = (color.r == max_channel) ? 
    max(0, min(255, (int)color.r + random(-var_amount_dom, var_amount_dom))) :
    max(0, min(255, (int)color.r + random(-var_amount_other, var_amount_other)));
    
  newColor.g = (color.g == max_channel) ? 
    max(0, min(255, (int)color.g + random(-var_amount_dom, var_amount_dom))) :
    max(0, min(255, (int)color.g + random(-var_amount_other, var_amount_other)));
    
  newColor.b = (color.b == max_channel) ? 
    max(0, min(255, (int)color.b + random(-var_amount_dom, var_amount_dom))) :
    max(0, min(255, (int)color.b + random(-var_amount_other, var_amount_other)));
    
  return newColor;
}

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(ledState.leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  
  // Initialize LEDs with variations
  for (int i = 0; i < dataSize; i++) {
    ledState.leds[data_group_1[i].num] = varyColor(data_group_1[i].color, 20);
  }
  
  analogReference(EXTERNAL);
  ledState.floori = 1023;
  ledState.ceili = 1;
  ledState.lastColorUpdate = millis();
  ledState.currentHue = 0;
}

void updateLEDArray(Data* ledArray, int arraySize, uint16_t v, int fade) {
  // Calculate threshold based on floor and ceiling
  const uint16_t threshold = (ledState.floori + ledState.ceili) * BEAT_THRESHOLD;
  
  for (int i = 0; i < arraySize; i++) {
    if (ledArray[i].num < NUM_LEDS) {
      if (v > threshold) {
        ledState.leds[ledArray[i].num] = ledArray[i].color;
      } else {
        ledState.leds[ledArray[i].num].nscale8(fade);
      }
    }
  }
}

void updateLEDs(uint16_t v) {
  updateLEDArray(flash_leds, flashSize, v, 252);
  updateLEDArray(eye, eyeSize, v, 235);
}

void loop() {

  unsigned long startMillis = millis();
  uint16_t peakToPeak = 0;  
  uint16_t signalMax = 0;
  uint16_t signalMin = 1023;
  
  if (millis() < 5000) {
    return;
  }

  // Run amplitude sample window
  while (millis() - startMillis < SAMPLE_WINDOW) {
    uint16_t sample = analogRead(AMP_PIN);

    // Validate and bound sample value
    if (sample > 1023) sample = 1023;  // Ensure sample doesn't exceed ADC max
    
    // Apply LPF to the sample
    sample = lpf_1.filter(sample); 

    // Update min/max with bounds checking
    if (sample < 1023) {
      if (sample > signalMax) {
        signalMax = sample;  
      }
      if (sample < signalMin) {  // Changed from else if to if
        signalMin = sample;  
      }
    }
  }
  
  // Safe peak-to-peak calculation
  peakToPeak = min(1023, signalMax - signalMin);
  peakToPeak = max(0, peakToPeak);


  if (peakToPeak < ledState.floori) {
    ledState.floori = peakToPeak;
  }
  
  if (peakToPeak > ledState.ceili) {
    ledState.ceili = peakToPeak;
  } 
  // Serial.print(peakToPeak);
  // Serial.print(" ");
  // Serial.print(ledState.floori);  // Print with 4 decimal places
  // Serial.print(" ");
  // Serial.println(ledState.ceili);  // Print with 4 decimal places

  updateLEDs(peakToPeak);
  FastLED.show();
}


