


void drawbrightnessscreen() {
  int oldx=0;

  tft.drawRoundRect(1, 1, 319, 239, 2, DARKGREY);

  x = map(brightval, 5, 255, 35, 285 ) + 1; // i add 1 otherwise it goes down==do not no why
  y = 0;
  delay(250);
  tft.fillScreen(BLACK);
  tft.drawRoundRect(1, 1, 319, 239, 2, DARKGREY);
  tft.setTextColor(GREEN, BLACK);
  tft.setCursor(20 , 30);
  tft.print("Set backlight brightness");


  TempLong = millis();  // store millis() counter in variable TempLong

  while (1 == 1) {                  // 1 wil always be 1   so forever
    tft.getTouch(&x, &y);

    if ((millis() - TempLong)  > 60000) { // compare stored TempLong to current millis() counter
      tft.fillScreen(BLACK);
      break;                              // after 60 seconds inactivetie timer break while loop timeout
    }

    tft.setCursor(275, 210);
    tft.setTextColor (LIGHTGREY, BLACK);
    tft.print(60 - ((millis() - TempLong) / 1000)); tft.print(" ");

    tft.setTextColor(LIGHTGREY);
    tft.setTextSize(2);
    tft.setCursor(0, 10);

    //tft.FilRoundRect(10, 110, 310, 50, 8, LIGHTGREY);         //draw buttons outline

    tft.drawRoundRect(160 - (145 / 2), 170, 145, 60, 8, LIGHTGREY);
    tft.setCursor(140, 190); tft.print("EXIT");
    if (x > 80 && x < 250 && y > 170 && y < 235) {
      tft.drawRoundRect(160 - (145 / 2), 170, 145, 60, 8, GREEN);
      tft.setTextColor(GREEN, BLACK);tft.setCursor(140, 190); tft.print("EXIT");
      delay(500);
      //should be something like wait for button released
      tft.fillScreen(BLACK);
      x=0;y=0;
      break;
    }
    tft.setTextColor(GREEN, BLACK);


    //yes ;-) i made my first slider without looking at code examples

    if (x < 35)x = 35;
    if (x > 295)x = 285;
    if (x != oldx) {
      tft.fillRoundRect(10, 110, 300, 50, 13, GREEN);
      tft.fillCircle(x, 135, 25, BLACK);
    }

    brightval = x;

    brightval = map(brightval, 35, 285, 5, 255);
    tft.setCursor(150, 55);
    tft.print(brightval);
    tft.print("  ");


    tft.setCursor(150, 80);
    //show it in procent
    tft.print(map(brightval, 5, 255, 2, 100)); //100/255*5 maybe 2% otherwise black screen = no visibility
    tft.print("%  ");

    oldx = x;

    if (brightval > 255)brightval = 255;
    ledcWrite(ledChannel, brightval); //output PWM for backlight swipe from left to right to set according to x positoin touch



    //print touch xy position to serial monitor
    Serial.print(x);
    Serial.print(",");
    Serial.println(y);
    // tft.setTextColor(GREEN, BLACK);
    // tft.setCursor(120 , 30);
    // tft.print("X="); tft.print(x); tft.print(" ");
    // tft.setCursor(200, 30);
    // tft.print("Y="); tft.print(y); tft.print(" ");
    if (drawgreendot)tft.drawPixel(x, y, GREEN);
  }
  //waitfortouchanywhere();

}
