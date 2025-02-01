# LED Amplitude Response with FastLED

This project reads audio signal amplitude from an analog pin, applies a low-pass filter, and uses the FastLED library to control a set of WS2812 LEDs based on the amplitude of the signal. It creates dynamic lighting effects that react to changes in the audio signal.

## Requirements
- Arduino IDE
- FastLED Library
- Digital Filter Library
- WS2812-compatible LED matrix (16x16)
- Arduino board with at least one analog input pin (e.g., Arduino Uno)

## Hardware Setup
- **LEDs**: Connect a strip of WS2812 (or compatible) LEDs to the appropriate pin on your Arduino (configured as `DATA_PIN`).
- **Microphone/Audio Input**: Connect an analog microphone (such as MAX4466) or audio sensor to the `AMP_PIN` (configured as `A0`).

## Setup Instructions
1. Download and install the [FastLED library](https://github.com/FastLED/FastLED) and the [digital_filter_arduino library](https://github.com/timonbraun02/digital_filter_arduino).
2. Connect your WS2812 LEDs to the `DATA_PIN` (defined in your code) on your Arduino board.
3. Connect an analog microphone or any audio sensor to the `AMP_PIN` (A0).
4. Upload the sketch to your Arduino.

## Code Explanation
- **Low-pass filtering**: This is used to smooth out high-frequency noise from the analog input signal, making the LED responses more stable.
- **Amplitude Calculation**: The difference between the maximum and minimum signal values over a short sampling window is used to calculate the "peak-to-peak" amplitude.
- **LED Response**: The LEDs change their color and brightness based on the calculated amplitude, with different effects applied based on the values.

## Customization
- **Floor & Ceiling Values**: These control the range of response. The `floori` and `ceili` values are updated dynamically based on the input signal over time.
- **LED Effects**: Modify the `for` loops that control how each LED changes. You can adjust color, brightness, or introduce new effects based on the signal amplitude.

## Troubleshooting
- Ensure the analog input pin is receiving a clean signal from the audio sensor.
- Verify that your LED strip is correctly wired and powered.
- If the LEDs don’t react as expected, try adjusting the `AMP_PIN` and the filter values.

## License
This code is open-source and free to use and modify under the MIT license.

---

Feel free to adjust parameters or add new features to this setup to suit your needs!
