void settings_two_screen() {
  x = 0; y = 0;
  delay(250);
  tft.fillScreen(BLACK);


  TempLong = millis();  // store millis() counter in variable TempLong

  while (1 == 1) {                  // 1 wil always be 1   so forever
   
    if (tft.getTouch(&x, &y)) {
      //print touch xy position to serial monitor
      Serial.print(x);
      Serial.print(",");
      Serial.println(y);
      // tft.setTextColor(GREEN, BLACK);
      // tft.setCursor(120 , 30);
      // tft.print("X="); tft.print(x); tft.print(" ");
      // tft.setCursor(200, 30);
      // tft.print("Y="); tft.print(y); tft.print(" ");
    }

    if ((millis() - TempLong)  > 60000) { // compare stored TempLong to current millis() counter
      tft.fillScreen(BLACK);
      break;                              // after 60 seconds inactivetie timer break while loop timeout
    }

    tft.setCursor(275, 210);
    tft.setTextColor (LIGHTGREY, BLACK);
    tft.print(60 - ((millis() - TempLong) / 1000)); tft.print(" ");

    tft.setTextSize(2);

    tft.drawRoundRect(1, 1, 319, 239, 2, DARKGREY);

    tft.drawRoundRect(10, 85, 145, 70, 8, LIGHTGREY);
    tft.setCursor(25, 110); tft.print("NetWork IP");
    if (x > 10 && x < 150 && y > 60 && y < 140) {
      tft.drawRoundRect(10, 85, 145, 70, 8, GREEN);
      Serial.println("brightness button pressed");
      delay(500);
    }

    tft.drawRoundRect(10, 10, 145, 70, 8, LIGHTGREY);
    tft.setCursor(30, 40); tft.print("1");
    if (x > 10 && x < 150 && y > 10 && y < 80) {
      tft.drawRoundRect(10, 10, 145, 70, 8, GREEN);
      Serial.println("calibrate button pressed");
      delay(500);
    }

    tft.drawRoundRect(165, 10, 145, 70, 8, LIGHTGREY);
    tft.setCursor(175, 40); tft.print("2");
    if (x > 165 && x < 315 && y > 5 && y < 65) {
      tft.drawRoundRect(165, 10, 145, 70, 8, GREEN);
      delay(500);
      break;
    }

    tft.drawRoundRect(165, 85, 145, 70, 8, LIGHTGREY);
    tft.setCursor(180, 110); tft.print("4");
    if (x > 165 && x < 315 && y > 70 && y < 145) {
      tft.drawRoundRect(165, 85, 145, 70, 8, GREEN);
      delay(500);
      drawtimedatesetscreen();
      delay(250);
      break;
    }

    tft.drawRoundRect(10, 165, 145, 70, 8, LIGHTGREY);
    tft.setCursor(70, 190); tft.print("<<");
    if (x > 10 && x < 150 && y > 165 && y < 230) {
      tft.setTextColor(GREEN, BLACK); tft.setCursor(70, 190); tft.print("<<");
      tft.drawRoundRect(10, 165, 145, 70, 8, GREEN);
      delay(500);
      break;
    }

    tft.drawRoundRect(165, 165, 145, 70, 8, LIGHTGREY);
    tft.setCursor(220, 190); tft.print(">>");
    if (x > 160 && x < 310 && y > 165 && y < 230) {
      tft.setTextColor(GREEN, BLACK); tft.setCursor(220, 190); tft.print(">>");
      tft.drawRoundRect(165, 165, 145, 70, 8, GREEN);
      delay(500);
      settings_two_screen();
      break;
    }

    //print touch xy position to serial monitor
    Serial.print(x);
    Serial.print(",");
    Serial.println(y);
    //  tft.setTextColor(GREEN, BLACK);
    // tft.setCursor(120 , 30);
    // tft.print("X="); tft.print(x); tft.print(" ");
    // tft.setCursor(200, 30);
    // tft.print("Y="); tft.print(y); tft.print(" ");
    if (drawgreendot)tft.drawPixel(x, y, GREEN);
  }

}
