// ESP32 relays test KY-019  tick like a clock
// VCC = 5v
// GND
// IN from gpio 26
// https://oshwlab.com/l.dijkman/esp32-dev-kit-38-pin-to-spi-touch-tft 
// https://github.com/ldijkman/ART-ESP32-Touch-TFT-Thermostat 
// http://www.Arduino.TK

const int relay = 26;

void setup() {
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
}

void loop() {
  digitalWrite(relay, LOW);
  Serial.println("relays off");
  delay(500); 

  digitalWrite(relay, HIGH);
  Serial.println("relays on");
  delay(500);
}
