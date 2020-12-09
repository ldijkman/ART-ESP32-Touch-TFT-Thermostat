// ESP32 test SPI TFT backlight PWM brightnes
// PWM = Pulse Width Modulation or pulse-duration modulation
// more or less PWM creates a voltage from 0 to 3.3volt on gpio 32
// But SPI connector LED goes to a transistor on the TFT PCB wich switches the VCC wich i have connected to 5Volt
// https://oshwlab.com/l.dijkman/esp32-dev-kit-38-pin-to-spi-touch-tft

const int ledPin = 32;  // corresponds to GPIO32
                        // connect to LED of SPI TFT display

// setting PWM properties
const int freq = 4000;           // 4khz
const int ledChannel = 1;        // think channel 0 is in use by buzzer
const int resolution = 8;        // 8bit 0 to 255

void setup(){
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
}

void loop(){
  // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
    delay(15);
  }

  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
    delay(15);
  }
}
