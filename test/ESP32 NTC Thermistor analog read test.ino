// ESP32 analog read test
// KY-013 NTC thermistor signal is connected to GPIO 34 (Analog ADC1_CH6)
// KY-013 NTC thermistor - is connected to 3.3V //strange but correct - to +3.3V
// KY-013 NTC thermistor + is connected to GND  //strange but correct + to GND
// outputs Fahrenheit and Celcius to serial monitor
// https://oshwlab.com/l.dijkman/esp32-dev-kit-38-pin-to-spi-touch-tft
// https://github.com/ldijkman/ART-ESP32-Touch-TFT-Thermostat

int ThermistorPin = 34;   //gpio34
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
float voltage = 3.3;   //use 3.3volt

void setup() {
  Serial.begin(115200);  //serial monitor
}

void loop() {
  Vo = 0;
  for (int i = 0; i <= 500; i++) {          // do 500 readings makes it more stable
    Vo = Vo + analogRead(Thermisto// ESP32 analog read test
// KY-013 NTC thermistor signal is connected to GPIO 34 (Analog ADC1_CH6)
// KY-013 NTC thermistor - is connected to 3.3V //strange but correct - to +3.3V
// KY-013 NTC thermistor + is connected to GND  //strange but correct + to GND
// outputs Fahrenheit and Celcius to serial monitor
// https://oshwlab.com/l.dijkman/esp32-dev-kit-38-pin-to-spi-touch-tft

int ThermistorPin = 34;   //gpio34
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
float voltage = 3.3;   //use 3.3volt

void setup() {
  Serial.begin(115200);  //serial monitor
}

void loop() {
  Vo = 0;
  for (int i = 0; i <= 500; i++) {          // do 500 readings makes it more stable
    Vo = Vo + analogRead(ThermistorPin);
  }
  Vo = Vo / 500;                           // divide result of 500 radings by 500

  R2 = R1 * (4095.0 / (float)Vo - 1.0);    // 4095=12bit analog read
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  Tc = T - 273.15;
  Tf = (Tc * 9.0) / 5 + 32.0;

  Serial.print("Temperature: ");
  Serial.print(Tf);
  Serial.print(" F; ");
  Serial.print(Tc);
  Serial.println(" C");
  Serial.print(Tc,1);    // 1 decimal?
  Serial.println(" C");

  delay(500);
}
rPin);
  }
  Vo = Vo / 500;                           // divide result of 500 radings by 500

  R2 = R1 * (4095.0 / (float)Vo - 1.0);    // 4095=12bit analog read
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  Tc = T - 273.15;
  Tf = (Tc * 9.0) / 5 + 32.0;

  Serial.print("Temperature: ");
  Serial.print(Tf);
  Serial.print(" F; ");
  Serial.print(Tc);
  Serial.println(" C");
  Serial.print(Tc,1);    // 1 decimal?
  Serial.println(" C");

  delay(500);
}
ESP32 NTC thermistor test analog input
