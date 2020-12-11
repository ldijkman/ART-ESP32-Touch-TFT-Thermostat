// Single File Code from Fabio Mastrotti 
// https://www.youtube.com/watch?v=CM0h_ad7ETU
//
// this video started it all for me
//
// the setrotation changes => drives you crazy
// possible because of display and touch locations with adafruit librarys

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include <math.h>
#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif
#define YP A3
#define XM A2
#define YM 9
#define XP 8
#define TS_MINX 120
#define TS_MAXX 900
#define TS_MINY 70
#define TS_MAXY 920
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
// Color definitions
#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
const int pin = A5;
double celsius = 0;
float millivolts;
int sensor; 
int contatore = 0;
int caldaia_s = 0;
int old_caldaia_s = 0;
int eco_m = 0;
int lettura_seriale;
double beginTemperature = 18;
double U_D_T = 0.5;
double memoria;
double eco_m_temp;
void setup(void) {
  Serial.begin(9600);
  tft.reset();  
  uint16_t identifier = tft.readID();
  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier=0x9341;   
  }
  sensor = analogRead(pin);
  millivolts = ( sensor/1023.0)*3300;
  celsius =millivolts/10; 
  tft.begin(identifier);
  tft.setRotation(3);
  tft.fillScreen(WHITE);
  tft.setCursor(102, 105);
  tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(3);
  tft.println("Wellcome");
  delay(2000);
  tft.fillScreen(BLACK);
  tft.setCursor(102, 105);
  tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(3);
  tft.println("designed"); 
  delay(300);
  tft.fillScreen(WHITE); 
  tft.setCursor(102, 105);
  tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(3);
  tft.println("   by");
  delay(300);
  tft.fillScreen(BLACK);
  tft.setCursor(102, 105);
  tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(3);
  tft.println("  SPM");   
  pinMode(13, OUTPUT);
  delay(1000);
  tft.setRotation(2);
  tft.fillScreen(BLACK);
  tft.drawRect(10, 10, 70, 90, ILI9341_LIGHTGREY);
  tft.drawRect(10, 115, 70, 90, ILI9341_LIGHTGREY);
  tft.drawRect(10, 220, 70, 90, ILI9341_LIGHTGREY);
  tft.drawRect(90, 10, 140, 300, ILI9341_LIGHTGREY);
 tft.setRotation(3);
  tft.setCursor(41, 175);
  tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(6);
  tft.println("-");
  tft.setCursor(251, 175);
  tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(6);
  tft.println("+");
  tft.setCursor(144, 173);
  tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
  tft.println("ECO");
  tft.setCursor(137, 200);
  tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
  tft.println("MODE");
  tft.setCursor(25, 35);
  tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
  tft.println("Temp. amb.:         C");
  tft.setCursor(25, 75);
  tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
  tft.println("Temp. imp.:         C");
  tft.setCursor(175, 75);
  tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
  tft.println(beginTemperature);
  tft.setCursor(175, 35);
  tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
  tft.println(celsius);
  tft.setCursor(25, 115);
  tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
  tft.println("Stato caldaia:");
}
#define MINPRESSURE 10
#define MAXPRESSURE 1000
void loop(){
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  sensor = analogRead(pin);
  millivolts = ( sensor/1023.0)*3300; 
  celsius =millivolts/10; 
  contatore=contatore+1;
  if (contatore>=2000){
    tft.setRotation(2);
    tft.fillRect(186, 170, 25, 75, BLACK);
    tft.setRotation(3);          
    tft.setCursor(175, 35);
    tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
    tft.println(celsius);
    contatore=0;
  }
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    tft.setRotation(2);
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = (tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
    if (p.x > 10 && p.x < 80) {
          if (p.y > 10 && p.y < 100) {
          beginTemperature= (beginTemperature-U_D_T);
          //Serial.print(("5"));
          //tft.setRotation(2);
          tft.fillRect(145, 170, 25, 75, BLACK);
          tft.setRotation(3);
          tft.setCursor(175, 75);
          tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
          tft.println(beginTemperature);
          delay(100);
        }
        if (p.y > 115 && p.y < 205) {
          tft.setRotation(3);
          if (eco_m==0){
            eco_m=1;            
            tft.setCursor(144, 173);
            tft.setTextColor(ILI9341_DARKGREEN);  tft.setTextSize(2);
            tft.println("ECO");
            tft.setCursor(137, 200);
            tft.setTextColor(ILI9341_DARKGREEN);  tft.setTextSize(2);
            tft.println("MODE");
            memoria = beginTemperature;
            eco_m_temp = beginTemperature-3;

            tft.setRotation(2);
            tft.fillRect(145, 15, 25, 280, BLACK);
            tft.setRotation(3);
            tft.setCursor(25, 75);
            tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
            tft.println("Eco Temp.:          C");
            tft.setCursor(175, 75);
            tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
            tft.println(eco_m_temp);  
            delay(1000);
            goto OUTSUB;
          }
          if (eco_m==1){
            eco_m=0;
            tft.setCursor(144, 173);
            tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
            tft.println("ECO");
            tft.setCursor(137, 200);
            tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
            tft.println("MODE");
            beginTemperature = memoria;
            tft.setRotation(2);
            tft.fillRect(145, 15, 25, 280, BLACK);
            tft.setRotation(3);
            tft.setCursor(25, 75);
            tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
            tft.println("Temp. imp.:         C");
            tft.setCursor(175, 75);
            tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
            tft.println(beginTemperature);
            delay(1000);
            goto OUTSUB;
          }       
        }        
        if (p.y > 220 && p.y < 310) {
          beginTemperature= (beginTemperature+U_D_T);
          //Serial.print(("4"));
          //tft.setRotation(2);
          tft.fillRect(145, 170, 25, 75, BLACK);
          tft.setRotation(3);
          tft.setCursor(175, 75);
          tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
          tft.println(beginTemperature);
          delay(100);
        }
      }
    }
OUTSUB:
  if (beginTemperature>30){
    beginTemperature=30;
    tft.setRotation(2);
    tft.fillRect(145, 170, 25, 75, BLACK);
    delay(600);
    tft.setRotation(3);
    tft.setCursor(175, 75);
    tft.setTextColor(RED);  tft.setTextSize(2);
    tft.println(" MAX");
    delay(600); 
    tft.setRotation(2);
    tft.fillRect(145, 170, 25, 75, BLACK);
    delay(600);
    tft.setRotation(3);
    tft.setCursor(175, 75);
    tft.setTextColor(RED);  tft.setTextSize(2);
    tft.println(" MAX");
    delay(600); 
    tft.setRotation(2);
    tft.fillRect(145, 170, 25, 75, BLACK);
    delay(600);
    tft.setRotation(3);
    tft.setCursor(175, 75);
    tft.setTextColor(RED);  tft.setTextSize(2);
    tft.println(" MAX");
    delay(600); 
    tft.setRotation(2);
    tft.fillRect(145, 170, 25, 75, BLACK);
    delay(600);
    tft.setRotation(3);
    tft.setCursor(175, 75);
    tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
    tft.println(beginTemperature);
   }
  if (beginTemperature<10){
    beginTemperature=10;
    tft.setRotation(2);
    tft.fillRect(145, 170, 25, 75, BLACK);
    delay(600);
    tft.setRotation(3);
    tft.setCursor(175, 75);
    tft.setTextColor(BLUE);  tft.setTextSize(2);
    tft.println(" MIN");
    delay(600); 
    tft.setRotation(2);
    tft.fillRect(145, 170, 25, 75, BLACK);
    delay(600);
    tft.setRotation(3);
    tft.setCursor(175, 75);
    tft.setTextColor(BLUE);  tft.setTextSize(2);
    tft.println(" MIN");
    delay(600); 
    tft.setRotation(2);
    tft.fillRect(145, 170, 25, 75, BLACK);
    delay(600);
    tft.setRotation(3);
    tft.setCursor(175, 75);
    tft.setTextColor(BLUE);  tft.setTextSize(2);
    tft.println(" MIN");
    delay(600); 
    tft.setRotation(2);
    tft.fillRect(145, 170, 25, 75, BLACK);
    delay(600);
    tft.setRotation(3);
    tft.setCursor(175, 75);
    tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
    tft.println(beginTemperature);
    }
  if (eco_m==0){
    if (celsius>(beginTemperature+1.5)){
    caldaia_s = 0;
    }               
  if (celsius<(beginTemperature-1.5)){
    caldaia_s = 1;
    }
  }
  if (eco_m==1){
    if (celsius>(eco_m_temp+1.5)){
    caldaia_s = 0;
    }               
  if (celsius<(eco_m_temp-1.5)){
    caldaia_s = 1;
    }
  }
 if (caldaia_s == 0){
        if (old_caldaia_s == 1){
            tft.setRotation(2);
      tft.fillRect(104, 195, 30, 80, BLACK);
      tft.setRotation(3);
      tft.setCursor(215, 115);
      tft.setTextColor(ILI9341_LIGHTGREY);  tft.setTextSize(2);
      tft.println("OFF");
      old_caldaia_s = 0;
      //Serial.print(("2"));
      delay(400);
      
      }
     }
     
  if (caldaia_s == 1){
        if (old_caldaia_s == 0){
      tft.setRotation(2);
      tft.fillRect(104, 195, 30, 80, BLACK);
      tft.setRotation(3);
      tft.setCursor(215, 115);
      tft.setTextColor(RED);  tft.setTextSize(2);
      tft.println("ON");
      old_caldaia_s = 1;
      //Serial.print(("3"));
      delay(400);
      }
     }    
}

