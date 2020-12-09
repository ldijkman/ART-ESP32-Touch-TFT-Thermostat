// ESP32 LDR test should work 
// https://oshwlab.com/l.dijkman/esp32-dev-kit-38-pin-to-spi-touch-tft
// https://github.com/ldijkman/ART-ESP32-Touch-TFT-Thermostat

int LDRValue=0;
byte LIGHT_SENSOR = 36; //  GPIO 36 Analog Input

void setup() 
{
  Serial.begin(115200);
  pinMode(LIGHT_SENSOR,INPUT);

}

void loop() 
{
  //Get the ADC values
  LDRValue = analogRead(LIGHT_SENSOR);
  Serial.print(LDRValue);
  Serial.print("     ");
  //Get brightness Percentage
  double lumLevel = (LDRValue / 4095.00);
  Serial.print((lumLevel * 100.00),2);
  Serial.println("%");
  delay(1000);
}
