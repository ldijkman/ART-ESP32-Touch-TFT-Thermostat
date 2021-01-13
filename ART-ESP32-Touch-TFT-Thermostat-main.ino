// hello
// https://oshwlab.com/l.dijkman/esp32-dev-kit-38-pin-to-spi-touch-tft
// https://github.com/ldijkman/ART-ESP32-Touch-TFT-Thermostat
// http://www.Arduino.TK
//
// https://m.facebook.com/groups/2643123052617990
// https://www.facebook.com/groups/esp32smartthermostat
//
// GNU General Public License,
// which basically means that you may freely copy, change, and distribute it,
// but you may not impose any restrictions on further distribution,
// and you must make the source code available.
//
//
//
// TFT white screen / blank screen
// make sure u set the file
// ......................../arduino/libraries/TFT_eSPI/User_Setup.h
// Correct to match your TFT screen driver and hardware
//
// my example User_Setup.h settings at http://Arduino.TK
//
//
//
//

//
//
//
//
//
//
//
//
//
//
//

//********************************************************************************
// 21 december 2020 switched to BME280 temp, mbar, humid, sensor == much better
//********************************************************************************

// ART ESP32 Thermostat by luberth dijkman
// https://oshwlab.com/l.dijkman/esp32-dev-kit-38-pin-to-spi-touch-tft
// https://create.arduino.cc/editor/luberth/becc77e8-4000-4673-9412-dbaac0a3b268/preview

// https://www.youtube.com/user/LuberthDijkman

// https://m.facebook.com/luberth.dijkman

// 320x240 screen or bigger but then the display is 320x240
// under construction
//
// https://youtu.be/CM0h_ad7ETU by Fabio Mastrotto, He got me into this trouble


// ART ESP32Thermostat by luberth dijkman
// https://oshwlab.com/l.dijkman/esp32-dev-kit-38-pin-to-spi-touch-tft


// under construction
// Thank you for the Coffee!
//           https://paypal.me/LDijkman

#include <WiFi.h>
#include <WebServer.h>
WebServer server(80);
#include <WiFiClient.h>
                                              // some say do not use spaces in broadcasted wifi router name
const char* ssid     = "Bangert-30-Andijk";  // wifi router name broadcasted in the air
const char* password = "password";          // wifi router password


#include <NTPClient.h>               // Include NTPClient library
#include <TimeLib.h>                 // Include Arduino time library https://github.com/PaulStoffregen/Time
WiFiUDP ntpUDP;


// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 24 * 60 * 60 * 1000);
int last_second = 0, second_ = 0, minute_ = 0, hour_ = 0, day_ = 0, month_ = 0, year_ = 0;

#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library https://github.com/Bodmer/TFT_eSPI
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

// The SPIFFS (FLASH filing system) is used to hold touch screen calibration data
// This is the file name used to store the calibration data file, name must start with "/".
#define CALIBRATION_FILE "/TouchCalData"

//#define FORMAT_SPIFFS_IF_FAILED 0

// Set REPEAT_CAL to true instead of false or 1 or 0 to run calibration
byte REPEAT_CAL = 0;

byte drawgreendot = 1;  // draw touch position with a greendot

// set to 0 if u want to use a higher resolution NTC Thermistor KY-013 0,25 resolution is not verry useful for a thermostat in my opinion
byte Use_TempSensor_from_DS3231 = 1;// 1 is yes use ds3231 temp res. 0.25C

// IMPORTANTE!
// set to 0 if RTClock is programmed and program arduino again with this set to 0
byte Force_DateTimeRewrite = 0;  // if 1 write time and date to DS3231 RTClock // 1= each reboot will set the time and date of first program loading
// you can set time and date on tft now

#include <Wire.h>

#define I2C_SDA 33
#define I2C_SCL 32

#include "RTClib.h" // https://github.com/adafruit/RTClib
RTC_DS3231 rtc;    // download zip from above and install library from zip
// Sketch=>include library=>Add ZIP Library

#include <Adafruit_Sensor.h>  // used zip from https://github.com/adafruit/Adafruit_Sensor 
#include <Adafruit_BME280.h>  // used zip from https://github.com/adafruit/Adafruit_BME280_Library
// Sketch=>include library=>Add ZIP Library
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 BME280; // I2C
bool BME280_status;

const int ledPin = 17;  // corresponds to GPI17
// connect to LED of SPI TFT display
// setting PWM properties
const int freq = 4000;           // hz
const int ledChannel = 0;        // think channel 0 is in use by buzzer
const int resolution = 8;        // 8bit 0 to 255

byte backgroundlightval = 127;    // not below 5 and upto to 255 backlight brightness better not totaly black


// Color definitions
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define NAVY        0x000F
#define DARKGREEN   0x03E0
#define DARKCYAN    0x03EF
#define MAROON      0x7800
#define PURPLE      0x780F
#define OLIVE       0x7BE0
#define LIGHTGREY   0xC618
#define DARKGREY    0x7BEF
#define ORANGE      0xFD20
#define GREENYELLOW 0xAFE5
#define PINK        0xF81F

#define dutchorange 0xfbc0
#define iceblue 0x1dfb
// pitty you can not enter a colorcode on next page
// https://chrishewett.com/blog/true-rgb565-colour-picker/

long TempLong;
long touchtime;
long runTime;
long runTime2;

const int ntc_thermistor_pin = 34;        // NTC thermistor module KY-013 signal pin gpio34
const int heat_relais_pin = 26;          // relais module connection pin gpio26
const int cool_relais_pin = 25;         // other contact for aico / fan

int oldstate;
double oldval;

float TempCelsius = 0;
float Tf = 0;
long ntc_analog_value;
int counter = 0;
int HeatState = 0;
byte CoolState = 0;


byte mode = 1;              // start with eco mode that is safest
byte oldmode;

double decrement = 0.1;
double temp_setpoint = 20;

double normal_setpoint = 21;    // save our planet
double auto_setpoint = 20;     // save our planet
double eco_setpoint = 15;     // save our planet
double cool_setpoint = 25;   // save our planet

int time_in_minutes;

float CalibrationOffset = 0;  // correction for actual temp +or-
float switchbelowset = 0.2;
float switchaboveset = 0.2;

// some things for sleepless nights
// float lowtempalarm = 5;
// float hightempalarm = 35;
// int timetosetpointtolong = 30; // min
// int timeheatontolong = 60;     // minutes
// int minumumheatontime = 4;     // minutes can imagine if not getting hot condesation corrosion
// int maximumheatontime = 60;    // desired temperature not reached within .. minutes

byte sensorfail = 0;

int oldminute;
int X;

const char *monthName[12] = {"Januari", "Februari", "March", "April", "May", "Juni",
                             "Juli", "Augustus", "September", "October", "November", "December"
                            };
char dayname[7][12] = {"Sunday ", "Monday ", "Tuesday ", "Wednesday ", "Thursday ", "Friday ", "Saturday "};

float HeatONhour[20], HeatONminute[20], tempON[20];
float HeatOFFhour[20], HeatOFFminute[20], tempOFF[20];

uint16_t x, y;
uint16_t calibrationData[5];
uint8_t calDataOK = 0;



byte out = 0; //goto label: did not work

byte fullscreenactive = 0;                      // flag fullscreen 0 or 1
int secondstoswitchtofullscreen = 20;          // seconds to go fullscreen with barometer and humdity
int bordercolor = dutchorange;



void setup(void) {



  // configure LED PWM functionalitites          // screen brightness background light
  ledcSetup(ledChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
  ledcWrite(ledChannel, backgroundlightval);                  // 100 +/- halfbright 0-255 PWM screen brightness background light
  // Jo less energy



  Wire.begin(I2C_SDA, I2C_SCL);

  pinMode(heat_relais_pin, OUTPUT);
  pinMode(cool_relais_pin, OUTPUT);
  digitalWrite(heat_relais_pin, HIGH);       // HIGH = heat output relais off
  digitalWrite(cool_relais_pin, HIGH);       // HIGH = cool output relais off


  // times must be from 00:00 to 23:59 following sequence??? maybe left 00:00 if not used

  // 3 on/off switch times a day monday to friday
  //5:30 to 6:30

  HeatONhour[0] = 5, HeatONminute[0] = 30, tempON[0] = 21;
  HeatOFFhour[0] = 6, HeatOFFminute[0] = 30, tempOFF[0] = 14;
  //11:00 to 12:30
  HeatONhour[1] = 11, HeatONminute[1] = 00, tempON[1] = 20;
  HeatOFFhour[1] = 12, HeatOFFminute[1] = 30, tempOFF[1] = 16;
  //16:00 to 23:00
  HeatONhour[2] = 16, HeatONminute[2] = 00, tempON[2] = 20;
  HeatOFFhour[2] = 23, HeatOFFminute[2] = 00, tempOFF[2] = 12;



  // 3 on/off switch times a day saturday to sunday
  //8:00 to 9:00
  HeatONhour[10] = 8, HeatONminute[10] = 00, tempON[10] = 21;
  HeatOFFhour[10] = 9, HeatOFFminute[10] = 00, tempOFF[10] = 14;
  //11:00 to 13:00
  HeatONhour[11] = 11, HeatONminute[11] = 00, tempON[11] = 20;
  HeatOFFhour[11] = 13, HeatOFFminute[11] = 00, tempOFF[11] = 16;
  //15:00 to 23:30
  HeatONhour[12] = 15, HeatONminute[12] = 00, tempON[12] = 20;
  HeatOFFhour[12] = 23, HeatOFFminute[12] = 30, tempOFF[12] = 12;



  Serial.begin(115200);   // serial monitor
  //   if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
  //      Serial.println("SPIFFS Mount Failed");
  //      return;
  //  }
  //readFile(SPIFFS, "/art_times.txt");

  tft.init();

  tft.setRotation(1); //setrotation before touch calibration

  // Calibrate the touch screen and retrieve the scaling factors
  touch_calibrate();

  // Clear the screen
  tft.fillScreen(TFT_BLACK);
  tft.drawRoundRect(1, 1, 319, 239, 2, DARKGREY);     // show screen size on bigger display
  tft.setCursor(0, 60);
  tft.setTextColor(LIGHTGREY);  tft.setTextSize(3);
  tft.println("  ART Thermostat");
  tft.println("   The ART of");
  tft.println("   Temperature");
  tft.println("   Controlled");
  tft.setTextSize(1); tft.setCursor(20, 230);
  tft.println("Made by Luberth Dijkman Andijk The Netherlands");
  delay(8000);

  tft.setTextColor(BLACK);

  if (! rtc.begin()) {
    tft.fillScreen(RED);
    tft.setTextColor(YELLOW);  tft.setTextSize(3);
    while (! rtc.begin()) {
      Serial.println("Could not find RTC, i2c DS3231 realtimeclock not found");
      tft.setCursor(0, 30);
      tft.println(" DS3231 RTC & BME280");
      tft.println(" Not Found");
      tft.println(" RTC i2c on pin");
      tft.println(" SDA=G33 SCL=G32");
      tft.println(" VCC=3.3V & GND");
      //example GPIO 33 as SDA and and GPIO 32 as SCL is as follows.
    }
    tft.fillScreen(GREEN);
    tft.setTextColor(BLACK);
    tft.setCursor(0, 80);
    tft.print("  YES found RTC");
    delay(10000);
  }
  tft.setTextColor(LIGHTGREY);

  BME280_status = BME280.begin(0x76);
  if (!BME280_status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (!BME280_status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
    }
  }


  // set the time from PC to DS3231 RTC
  if (rtc.lostPower() || Force_DateTimeRewrite == 1) {   // means OR
    tft.fillScreen(MAROON);
    tft.setCursor(0, 80);
    tft.setTextSize(2);
    tft.print(" RTC Setting xTime/Date");
    tft.setTextSize(3);
    delay(5000);               // 5 seconds delay
    // if you uncomment above if, forcing to program time date
    // then every time you reboot the thermostat
    // time is set to the time when you loaded the program into the arduino
    //
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    // This line sets the RTC with an explicit date & time, for example to set
    //           2020 juli 24 20:15:00 you would call:
    //   rtc.adjust(DateTime(2020, 7, 24, 20, 15, 0));
  }

  drawmainscreen();
  tft.print(" ");
  /*draw a grid
    tft.fillRect(1, 1, 4, 4, YELLOW);     // yellow origin xy
    tft.fillRect(316, 236, 4, 4, GREEN);  // green max xy
    //draw gridlines 20x20pixels
    for (int i = 0; i <= 320; i = i + 20) {
    tft.fillRect(i, 1, 1, 319, 0x1111);
    }
    for (int i = 0; i <= 320; i = i + 20) {
    tft.fillRect(1, i, 320, 1, 0x1111);
    }
  */
  tft.setTextColor(GREEN, BLACK);
  tft.setCursor(15, 30);
  WiFi.mode(WIFI_STA);        // Connect to your wifi

  WiFi.begin(ssid, password); // Start the Wi-Fi services
  Serial.println("Connecting to WiFi : " + String(ssid));
  tft.println("Connecting to WiFi : " + String(ssid));


  TempLong = millis();  // store millis() counter in variable TempLong

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    // Wait for WiFi to connect
    tft.setCursor(20, 40);
    tft.print(60 - ((millis() - TempLong) / 1000)); tft.print(" ");
    if ((millis() - TempLong)  > 60000)break;           // timeout exit if it takes to lomg 30 seconds = nowifi
  }
  Serial.println(" Connected to : " + String(ssid));

  Serial.print("Use IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  //----------------------------------------------------------------
  server.on("/", handleRoot);         // This displays the main webpage, it is called when you open a client connection on the IP address using a browser
  server.on("/temp", handleTEMP); // To update Temperature called by the function getSensorData
  server.on("/humid", handleHUMID); // To update Humidity called by the function getSensorData
  server.on("/pressure", handlePRESS); // To update Pressure called by the function getSensorData
  server.on("/mode", handlemode); // To update Pressure called by the function getSensorData

  server.on("/mode0", handlemode0); // To update Pressure called by the function getSensorData
  server.on("/mode1", handlemode1); // To update Pressure called by the function getSensorData
  server.on("/mode2", handlemode2); // To update Pressure called by the function getSensorData
  server.on("/mode3", handlemode3); // To update Pressure called by the function getSensorData
  //----------------------------------------------------------------

  server.begin();                     // Start the webserver

  timeClient.begin();                 // start ntp?

  tft.setTextColor(GREEN, BLACK);
  tft.setTextSize(1);
  tft.setCursor(120, 40);
  tft.println(WiFi.localIP());


  OUTSUB();
}





void loop() {

  DateTime now = rtc.now();  //ds3231

  timeClient.update();  //ntp

  // webserver
  server.handleClient();  // Keep checking for a client connection


  unsigned long unix_epoch = timeClient.getEpochTime();    // Get Unix epoch time from the NTP server

  second_ = second(unix_epoch);
  if (last_second != second_) {
    Serial.print("GetFormattedTime "); Serial.println(timeClient.getFormattedTime()); //ntp

    tft.setTextColor(GREEN, BLACK);
    tft.setTextSize(1);
    tft.setCursor(120, 50);
    tft.println("NTP Time: " + timeClient.getFormattedTime());

    Serial.print("DayOfWeek "); Serial.println(timeClient.getDay());                  //ntp
    Serial.print("seconds since 1-1-1970 "); Serial.println(timeClient.getEpochTime()); //ntp

    Serial.print("hour "); Serial.println(hour(unix_epoch));                          //ntp
    Serial.print("minute "); Serial.println(minute(unix_epoch));                      //ntp
    Serial.print("second "); Serial.println(second(unix_epoch));                      //ntp

    Serial.print("day "); Serial.println(day(unix_epoch));                            //ntp
    Serial.print("month "); Serial.println(month(unix_epoch));                        //ntp
    Serial.print("year "); Serial.println(year(unix_epoch));                          //ntp

    Serial.print("Task running on core ");
    Serial.println(xPortGetCoreID());


    TempCelsius = BME280.readTemperature();

    // Serial.print("Fahrenheit = "); Serial.print(Tf, 1);
    Serial.print(" Celsius = "); Serial.println(TempCelsius, 1);
    // https://www.google.com/search?q=3+fahrenheit+to+celsius



    Serial.print("Temperature = ");
    Serial.print(BME280.readTemperature());
    Serial.println(" *C");
    // Convert temperature to Fahrenheit
    // Serial.print("Temperature = ");
    // Serial.print(1.8 * BME280.readTemperature() + 32);
    // Serial.println(" *F");
    Serial.print("Pressure = ");
    Serial.print(BME280.readPressure() / 100.0F);
    Serial.println(" hPa");
    Serial.print("Approx. Altitude = ");
    Serial.print(BME280.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");
    Serial.print("Humidity = ");
    Serial.print(BME280.readHumidity());
    Serial.println(" %");
    Serial.println();


    tft.setTextSize (2);
    tft.setTextColor (LIGHTGREY, BLACK);
    tft.setCursor(30, 100);
    if (now.hour() < 10)tft.print(" ");        // print 01 to 09 as 1 to 9
    tft.print(now.hour());
    tft.print(":");
    if (now.minute() < 10)tft.print("0");      // print 01 to 09 as 01 to 09
    tft.print(now.minute());
    tft.print(":");
    if (now.second() < 10)tft.print("0");      // print 01 to 09 as 01 to 09
    tft.print(now.second());

    tft.setCursor(15, 12);
    tft.setTextSize (1);
    tft.print("Day ");
    tft.print(now.dayOfTheWeek());             // prints daynumber of the week

    tft.setTextSize (2);
    tft.setTextColor (LIGHTGREY, BLACK);
    tft.setCursor(17, 130);
    tft.print(dayname[now.dayOfTheWeek()]);
    tft.print(now.day());
    tft.print(" ");
    tft.print(monthName[now.month() - 1]);
    tft.print("   ");
    //tft.print(now.year());
    //tft.print("  ");


    oldminute = now.minute();

    last_second = second_;

  }  // end last_second not is  second_







  // Jo energy saving Backlight
  if (now.hour() < 8) {
    ledcWrite(ledChannel, 10);
  } else {
    ledcWrite(ledChannel, backgroundlightval);
  }
  if (now.hour() >= 23) {
    ledcWrite(ledChannel, 10);  // dim backround light display if hour >= 22uur            //backgroundlightval);
  }

  if (((millis() - touchtime) / 1000) <= 60) {
    ledcWrite(ledChannel, backgroundlightval);  // if display is touched keep it bright for 60 seconds
  }





  while (! rtc.begin() || rtc.lostPower() == 1) {
    //Glitch?
    delay(100);
    while (! rtc.begin() || rtc.lostPower() == 1) { // doublecheck try it again
      tft.fillScreen(RED);
      tft.setTextColor(YELLOW);  tft.setTextSize(2);
      delay(200);
      Serial.println("Couldn't find RTC i2c realtimeclock not found");
      tft.setCursor(0, 70);
      tft.println  ("  RTC realtimeclock lost");
      tft.println  (" RTC i2c on SDA SCL 20 21");
      tft.println  ("  or power on RTC lost   ");
      tft.println  ("    ");
      tft.println  ("  ShutDown Heat relais ");
      delay(500);
      tft.fillScreen(BLACK);
      tft.drawRoundRect(1, 1, 319, 239, 2, DARKGREY);     // show screen size on bigger display
      digitalWrite(heat_relais_pin, LOW);       // heat output off
    }
  }


  tft.setTextColor(RED, BLACK);
  tft.setTextSize(1);
  tft.setCursor(310, 3);
  tft.println("o");//char(3));              //Alive HEARTBEAT


  if (millis() - runTime >= 1000) { // Execute every 1000ms
    runTime = millis();             // store millis() counter in variable runtime


    server.handleClient();  // Keep checking for a client connection


    tft.setTextColor( BLACK, BLACK);
    tft.setTextSize(1);
    tft.setCursor(310, 3);
    tft.println("o");//char(3));          //Alive HEARTBEAT  https://github.com/Bodmer/TFT_eSPI







    time_in_minutes = (now.hour() * 60 + now.minute()); //time to minutes makes it easier to switch on a time


    //test hardcoded switch on day / time array

    Serial.println("mode == 2");

    // monday to friday
    if (now.dayOfTheWeek() >= 1 && now.dayOfTheWeek()  <= 5  ) { // 1=monday to 5=friday
      Serial.println("weekday");


      // timeinminutes is more easy to work switch with versus hours:minutes
      // timeinseconds is not needed minutes is accurate enough
      // 1440 minutes a day

      for (X = 0; X <= 2; X++) {
        //Serial.println(X);
        if (time_in_minutes >= HeatOFFhour[X] * 60 + HeatOFFminute[X] ) {
          auto_setpoint = tempOFF[X];
        }
        if (time_in_minutes >= HeatONhour[X] * 60 + HeatONminute[X] && time_in_minutes < HeatOFFhour[X] * 60 + HeatOFFminute[X]) {
          auto_setpoint = tempON[X];
          goto JumpOver;           // programmers do not like GOTO, i liked gwbasic ;-)
        }

      }
    }//weekday >=1 && <=5 mon...fri



    // saturday sunday;
    // stupid start the week with sunday => no more weekends
    // in The Netherlands the week starts with monday, we have weekends

    if (now.dayOfTheWeek() == 0 || now.dayOfTheWeek() == 6) { // 6=saturday 0=sunday;
      Serial.println("hieperdepiep hoera weekend");


      // timeinminutes is more easy to work switch with versus hours:minutes
      // timeinseconds is not needed minutes is accurate enough
      // 1440 minutes a day

      for (X = 10; X <= 12; X++) { // <= smaller or equal, x++ means x=x+1
        //Serial.println(X);
        if (time_in_minutes >= HeatOFFhour[X] * 60 + HeatOFFminute[X] ) {
          auto_setpoint = tempOFF[X];
        }
        if (time_in_minutes >= HeatONhour[X] * 60 + HeatONminute[X] && time_in_minutes < HeatOFFhour[X] * 60 + HeatOFFminute[X]) {
          auto_setpoint = tempON[X];
          goto JumpOver;           // programmers do not like GOTO, i liked gwbasic ;-)
        }
      }

    }// day 0 || 6 sat sun






JumpOver:
    Serial.println("i just jumped over");
    redraw_mode_button();


    // print actual temperature to the screen
    tft.setCursor(165, 65);
    tft.setTextColor(LIGHTGREY, BLACK);  tft.setTextSize(5);
    tft.println(TempCelsius, 1);


    counter = 0;
    OUTSUB();

    oldval = TempCelsius;




  } // end Execute every 1000ms=1second










  tft.setCursor(250, 0);
  tft.setTextColor (LIGHTGREY, BLACK);
  tft.print(((millis() - touchtime) / 1000));                   // last touch secondstoswitchtofullscreen seconds ago, go fullscreen
  if ((fullscreenactive == 0) && (((millis() - touchtime) / 1000) >= secondstoswitchtofullscreen)) {
    fullscreenactive = 1;                          // show fullscreen with milibar and humidity
    oldmode = mode;           //if mode changes online redraw buttons
    tft.fillRoundRect(5, 155, 310, 80, 1, BLACK);               // erase  buttons for fullscreen barometer en humidity text
  }



  tft.print("   ");


  if (tft.getTouch(&x, &y)) {
    touchtime = millis(); // store millis() for future screen aninmation if touch is longer as ?? time ago



    //print touch xy position to serial monitor
    Serial.print(x);
    Serial.print(",");
    Serial.println(y);
    tft.setCursor(140 , 30);
    tft.print("X="); tft.println(x);
    tft.setCursor(180, 30);
    tft.print("Y="); tft.println(y);;
    if (drawgreendot)tft.drawPixel(x, y, GREEN);


    // press on mainscreen opens settings menu
    if (x > 0 && x < 320 && y > 0 && y < 140) {
      settings_one_screen();    // open settings menu
      fullscreenactive = 0;
      drawmainscreen();         // restore main screen

    }

    if (fullscreenactive == 1) {
      // touch in bottomscreen makes buttons visible and active
      if (x > 0 && x < 320 && y > 165 && y < 240) {
        fullscreenactive = 0;                                        // show fullscreen with milibar and humidity
        delay(250);                                                  // better should be wait for touchrelease
        x = 0; y = 0;                                                // otherwise it could be seen as a button press
        tft.fillRoundRect(5, 155, 310, 80, 1, BLACK);                // erase old barometer and humidity text
      }
    }

    if (fullscreenactive == 0) {                                       // if not fullscreen touch buttons should not react to touch when in fullscreen

      // mode touch button
      if (x > 100 && x < 200 && y > 165 && y < 240) {

        mode = mode + 1;
        if (mode > 3)mode = 0;  // 0=normal 1=eco 2=auto 3=Cool




        if (mode == 0) { // nomal continu mode at comfort temperature

          tft.fillRoundRect(117, 162, 86, 66, 12, BLACK); // erase old button text
          tft.setCursor(130, 173);
          tft.setTextColor(LIGHTGREY);  tft.setTextSize(2);
          tft.println("NORMAL");
          tft.setCursor(137, 200);
          tft.println("MODE");

          tft.setTextSize(4);
          delay(750);

          OUTSUB();
        }



        if (mode == 1) { // eco  continu mode at goosebumbs temperature

          tft.fillRoundRect(117, 162, 86, 66, 12, BLACK); // erase old button text
          tft.setCursor(144, 173);
          tft.setTextColor(GREEN);  tft.setTextSize(2);
          tft.println("ECO");
          tft.setCursor(137, 200);
          tft.println("MODE");

          tft.setTextSize(4);
          delay(750);

          OUTSUB();
        }


        if (mode == 2) {  // 2=automode switch temperature by time program

          tft.fillRoundRect(117, 162, 86, 66, 12, BLACK); // erase old button text
          tft.setCursor(135, 173);
          tft.setTextColor(dutchorange);  tft.setTextSize(2);
          tft.println("AUTO");
          tft.setCursor(137, 200);
          tft.println("MODE");

          tft.setTextSize(4);
          delay(750);

          OUTSUB();
        }


        if (mode == 3) {  // 3=cool mode airco mode

          tft.fillRoundRect(117, 162, 86, 66, 12, BLACK); // erase old button text
          tft.setCursor(135, 173);
          tft.setTextColor(iceblue);  tft.setTextSize(2);
          tft.println("COOL");
          tft.setCursor(137, 200);
          tft.println("MODE");

          tft.setTextSize(4);
          delay(750);

          OUTSUB();
        }
      }




      // - touch button
      if (x > 0 && x < 90 && y > 165 && y < 240) {

        // 0=normal 1=eco 2=auto 3=Cool
        if (mode == 0) {                                        // normal
          normal_setpoint = (normal_setpoint - decrement);
          if (normal_setpoint < 10)normal_setpoint = 10;
        }
        if (mode == 1) {                                       // eco
          eco_setpoint = (eco_setpoint - decrement);
          if (eco_setpoint < 10)eco_setpoint = 10;
        }
        if (mode == 2) {                                       // auto
          auto_setpoint = (auto_setpoint - decrement);
          if (auto_setpoint < 10)auto_setpoint = 10;
        }
        if (mode == 3) {                                       // cool
          cool_setpoint = (cool_setpoint - decrement);
          if (cool_setpoint < 10)cool_setpoint = 10;
        }


        tft.setTextSize(4);   // 0=normal 1=eco 2=auto 3=Cool
        tft.setCursor(35, 65);
        if (mode == 0) {                                       // normal
          tft.setTextColor(LIGHTGREY, BLACK);
          tft.print(normal_setpoint, 1);
        }
        if (mode == 1) {                                      // eco
          tft.setTextColor(GREEN, BLACK);
          tft.print(eco_setpoint, 1);
        }
        if (mode == 2) {                                      // auto
          tft.setTextColor(dutchorange, BLACK);
          tft.print(auto_setpoint, 1);
        }
        if (mode == 3) {                                      // cool
          tft.setTextColor(iceblue, BLACK);
          tft.print(cool_setpoint, 1);
        }
        delay(50);
        //OUTSUB();
      }



      // + touch button
      if (x > 220 && x < 320 && y > 165 && y < 240) {

        // 0=normal 1=eco 2=auto 3=Cool
        if (mode == 0) {                                     // normal
          normal_setpoint = (normal_setpoint + decrement);
          if (normal_setpoint > 30)normal_setpoint = 30;
        }
        if (mode == 1) {                                     // eco
          eco_setpoint = (eco_setpoint + decrement);
          if (eco_setpoint > 30)eco_setpoint = 30;
        }
        if (mode == 2) {                                     // auto
          auto_setpoint = (auto_setpoint + decrement);
          if (auto_setpoint > 30)auto_setpoint = 30;
        }
        if (mode == 3) {                                     // cool
          cool_setpoint = (cool_setpoint  + decrement);
          if (cool_setpoint > 30)cool_setpoint = 30;
        }

        tft.setTextSize(4);
        tft.setCursor(35, 65);

        if (mode == 0) {                                // normal
          tft.setTextColor(LIGHTGREY, BLACK);          // 0=normal 1=eco 2=auto 3=Cool
          tft.print(normal_setpoint, 1);
        }
        if (mode == 1) {                               // eco
          tft.setTextColor(GREEN, BLACK);
          tft.print(eco_setpoint, 1);
        }
        if (mode == 2) {                             // auto
          tft.setTextColor(dutchorange, BLACK);
          tft.print(auto_setpoint, 1);
        }
        if (mode == 3) {                             // cool
          tft.setTextColor(iceblue, BLACK);
          tft.print(cool_setpoint, 1);
        }
        delay(50);
        // OUTSUB();


      }
    }           // end if not fullscreen touch buttons should not react to touch when in fullscreen
  }             // end if (tft.getTouch(&x, &y))

}               // end loop






void OUTSUB() {
  runTime2 = millis();


  // 0=normal 1=eco 2=auto 3=Cool
  if (mode == 0) {                                         // normal
    CoolState = 0;
    if (TempCelsius > (normal_setpoint + switchaboveset))HeatState = 0;
    if (TempCelsius < (normal_setpoint - switchbelowset))HeatState = 1;
  }
  if (mode == 1) {                                         // eco
    CoolState = 0;              
    if (TempCelsius > (eco_setpoint + switchaboveset))HeatState = 0;
    if (TempCelsius < (eco_setpoint - switchbelowset))HeatState = 1;
  }
  if (mode == 2) {                                         // auto
    CoolState = 0;
    if (TempCelsius > (auto_setpoint + switchaboveset))HeatState = 0;
    if (TempCelsius < (auto_setpoint - switchbelowset))HeatState = 1;
  }
  if (mode == 3) {                                          // cool
    HeatState = 0;
    if (TempCelsius > (cool_setpoint + switchaboveset)) CoolState = 1;
    if (TempCelsius < (cool_setpoint - switchbelowset)) CoolState = 0;
  }







  if (HeatState == 0 ) {

    if (!fullscreenactive) {
      tft.drawRoundRect(10, 10, 300, 140, 8, BLUE);
    }
    if (fullscreenactive) {
      tft.drawRoundRect(10, 10, 300, 220, 8, BLUE);
    }
    tft.setTextSize (1);
    tft.setTextColor (BLUE, BLACK);
    tft.setCursor(130, 13);
    tft.print("  ");
    tft.print(rtc.getTemperature());
    tft.print(char(247)); // C degree sign
    tft.print(" C  ");

    digitalWrite(heat_relais_pin, HIGH);       // HIGH = heat output relais off
  }



  if (HeatState == 1 ) {
    bordercolor = dutchorange;

    if (!fullscreenactive) {
      tft.drawRoundRect(10, 10, 300, 140, 8, bordercolor);
    }
    if (fullscreenactive) {
      tft.drawRoundRect(10, 10, 300, 220, 8, bordercolor);
    }
    tft.setTextSize (1);
    tft.setTextColor (dutchorange, BLACK);
    tft.setCursor(130, 13);
    tft.print("  ");
    tft.print(rtc.getTemperature());
    tft.print(char(247)); // C degree sign
    tft.print(" C  ");

    digitalWrite(heat_relais_pin, LOW);          // LOW = heat output relais on
  }

  if (CoolState == 0 ) {
    if (mode == 3) {

      if (!fullscreenactive) {
        tft.drawRoundRect(10, 10, 300, 140, 8, BLUE);
      }
      if (fullscreenactive) {
        tft.drawRoundRect(10, 10, 300, 220, 8, BLUE);
      }
      tft.setTextSize (1);
      tft.setTextColor (iceblue, BLACK);
      tft.setCursor(130, 13);
      tft.print("  ");
      tft.print(rtc.getTemperature());
      tft.print(char(247)); // C degree sign
      tft.print(" C  ");
    }
    digitalWrite(cool_relais_pin, HIGH);          // HIGH = airco / fan output relais off
  }

  if (CoolState == 1 ) {
    if (mode == 3) {

      if (!fullscreenactive) {
        tft.drawRoundRect(10, 10, 300, 140, 8, iceblue);
      }
      if (fullscreenactive) {
        tft.drawRoundRect(10, 10, 300, 220, 8, iceblue);
      }
      tft.setTextSize (1);
      tft.setTextColor (iceblue, BLACK);
      tft.setCursor(130, 13);
      tft.print("  ");
      tft.print(rtc.getTemperature());
      tft.print(char(247)); // C degree sign
      tft.print(" C  ");

      digitalWrite(cool_relais_pin, LOW);          // LOW = airco / fan output relais on
    }
  }

  // while ((millis() - runTime2) < 75) {  // delay to set execution time of outsub equal
  //    Serial.println(millis() - runTime2);
  // }


}             // end outsub













void waitfortouchanywhere() {
  // DONE => should be a timeout for return on this "forever" while loop
  //          TempLong = millis();  //reset innactive time counter (store count)
  //          if ((millis() - TempLong)  > 20000)break;
  // for safety i will turn off the heat now
  // digitalWrite(heat_relais_pin, LOW);       // heat output off
  TempLong = millis();  // store millis() counter in variable TempLong



  while (1 == 1) {                  // 1 wil always be 1   so forever
    if (tft.getTouch(&x, &y)) break; // touch anywhere on the screen to break while loop
    //  pinMode(XM, OUTPUT);
    // pinMode(YP, OUTPUT);


    if ((millis() - TempLong)  > 20000)break; // compare stored TempLong to current millis() counter
    // after 20 seconds inactivetie timer break while loop timeout
    tft.setCursor(0, 200);
    tft.setTextColor (RED);
    tft.print(" Touch to Continue, or ");
    tft.setTextColor (LIGHTGREY, BLACK);
    tft.print(20 - ((millis() - TempLong) / 1000)); tft.print(" ");
  }
  delay(200);

}







void timeout() {

  TempLong = millis();  // store millis() counter in variable TempLong

  while (1 == 1) {                  // 1 wil always be 1   so forever



    if ((millis() - TempLong)  > 60000)break; // compare stored TempLong to current millis() counter
    // after 20 seconds inactivetie timer break while loop timeout
    tft.setCursor(290, 200);

    tft.setTextColor (LIGHTGREY, BLACK);
    tft.print(60 - ((millis() - TempLong) / 1000)); tft.print(" ");
  }
  delay(200);

}

void readFile(fs::FS & fs, const char * path) {
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while (file.available()) {
    file.read();
  }
}

// The End == NO!
// for Copy/Paste people
// This is not realy The End
// There are TAB files


// No time 2 sleep, Please let me sleep.
// https://youtu.be/XzO9jGPtrhc
//ver 00001

// Thank you for the Coffee!
//           https://www.paypal.me/LDijkman

//bye
