void redraw_mode_button() {


  //tft.print(mode);
  //tft.println(oldmode);


  if (mode == 0) {
    tft.setTextColor(LIGHTGREY, BLACK); tft.setTextSize(2);
    tft.setCursor(20,45); tft.println(" Normal ");
  }
  if (mode == 1) {
    tft.setTextColor(GREEN, BLACK); tft.setTextSize(2);
    tft.setCursor(20, 45); tft.println("  Eco  ");
  }
  if (mode == 2) {
    tft.setTextColor(dutchorange, BLACK); tft.setTextSize(2);
    tft.setCursor(20, 45); tft.println(" Auto  ");
  }
  if (mode == 3) {
    tft.setTextColor(iceblue, BLACK); tft.setTextSize(2);
    tft.setCursor(20, 45); tft.println(" Cool  ");
  }

  if (fullscreenactive == 1) {

    if (mode != oldmode) {
      fullscreenactive = 0;  // if mode changed online exit fullmode to redraw buttons
      touchtime = millis();
      TempLong = millis(); // reset last touch count
      tft.fillRoundRect(5, 155, 310, 80, 1, BLACK);                // erase old barometer and humidity
      return;
    }

    float bartemp = BME280.readPressure() / 100.0F ;

    tft.fillRoundRect(11, 147, 297, 8, 0, BLACK); // erase a piece of bottom rectangle of main screen rectangle

    tft.setTextColor(LIGHTGREY, BLACK);
    tft.setTextSize(2);
    tft.setCursor(20, 160);

    tft.print(bartemp);
    tft.println(" mbar ");
    /*
      if ( bartemp  <= 985) {
      tft.println(" mbar = Stormy    ");
      }
      if (bartemp > 985 && bartemp <= 1005) {
      tft.println(" mbar = Rain     ");
      }
      if (bartemp > 1005 && bartemp <= 1025) {
      tft.println(" mbar = Change   ");
      }
      if (bartemp > 1025 && bartemp <= 1045) {
      tft.println(" mbar = Fair     ");
      }
      if (bartemp >= 1045)  {
      tft.println(" mbar = Very Dry ");
      }
    */
    // bartemp = 955; // test scale min
    // bartemp=1075; // test scale max
    //tft.fillRoundRect (25, 183, 270, 2, 1, LIGHTGREY);              // draw grey scale bar for linear barometer
    tft.fillRoundRect (25, 180, 270, 8, 1, BLACK);              // erase green ball for linear barometer
    // delay(250); // for scale min / max test
    tft.setTextColor(LIGHTGREY, BLACK);
    tft.setTextSize(0);
    tft.setCursor(20, 188);
    tft.println("  Stormy |  Rain  | Change |  Fair  | Very Dry");
    tft.fillCircle(map(bartemp, 955, 1075, 25, 295), 183, 3, GREEN);    // draw circle at current barometer pressure
    // scale current mbar (range 955to1075) to (xscreenposition range25to295)
    //tft.fillRoundRect(map(bartemp, 955, 1075, 25, 295) - 3, 182, 6, 4, 0, RED); // draw rect at current barometer pressure

    // bit off weather predict by barometric pressure
    // is this correct?
    // 975 stormy
    // 995 Rain
    // 1015 change
    // 1035 Fair
    // 1055 very dry

    float humidtemp = BME280.readHumidity();

    tft.setTextColor(LIGHTGREY, BLACK);
    tft.setTextSize(2);
    tft.setCursor(20, 205);
    tft.print("Humidity ");
    // if (humidtemp >= 40 && humidtemp <= 60) {  // draw green text between 40% and 60% is good for in house
    //  tft.setTextColor(GREEN, BLACK);
    // }
    tft.print(humidtemp);
    tft.setTextColor(LIGHTGREY, BLACK);
    tft.println(" %  ");


    // scale makes better vissible how whe are doing with moisture comfort
    //   humidtemp=100; // for testing scale, to see if blue circle is drawn at correct location
    //   humidtemp=0;   // for testing scale, to see if blue circle is drawn at correct location

    tft.fillRoundRect (205, 233, 90, 2, 2, RED);                                // draw scale for humidity
    tft.fillRoundRect (115, 233, 90, 2, 2, GREEN);                              // draw scale for humidity
    tft.fillRoundRect (25, 233, 90, 2, 2, YELLOW);                              // draw scale for humidity
    tft.fillCircle(map(humidtemp, 0, 100, 25, 295), 233, 3, BLACK);             // draw circle, scale currenthumid range 0to100 to xscreenpos range 25to295
    tft.fillRoundRect(map(humidtemp, 0, 100, 25, 295) - 3, 233, 6, 2, 0, MAGENTA); // draw rect at current barometer pressure

    return;   // skip redraw button if fullscreen active

  }



  //tft.drawRoundRect(10, 10, 300, 140, 8, LIGHTGREY);  // main sreen outline

  tft.drawRoundRect(10, 160, 90, 70, 8, LIGHTGREY);   // -     draw buttons outline
  tft.drawRoundRect(115, 160, 90, 70, 8, LIGHTGREY);  // mode  draw buttons outline
  tft.drawRoundRect(220, 160, 90, 70, 8, LIGHTGREY);  // +     draw buttons outline


  tft.setTextColor(LIGHTGREY);
  tft.setTextSize(6);
  tft.setCursor(41, 175);
  tft.println("-");
  tft.setCursor(251, 175);
  tft.println("+");

  tft.setTextSize(4);// 0=normal 1=eco 2=auto 3=//cool
  tft.setCursor(35, 65);
  if (mode == 0) {
    tft.setTextColor(LIGHTGREY, BLACK);  // 0=normal 1=eco 2=auto 3=Cool
    tft.print(normal_setpoint, 1);
  }
  if (mode == 1) {
    tft.setTextColor(GREEN, BLACK);
    tft.print(eco_setpoint, 1);
  }
  if (mode == 2) {
    tft.setTextColor(dutchorange, BLACK);
    tft.print(auto_setpoint, 1);
  }
  if (mode == 3) {
    tft.setTextColor(iceblue, BLACK);
    tft.print(cool_setpoint, 1);
  }


  if (mode == 0) {  // normal mode

    tft.fillRoundRect(117, 162, 86, 66, 12, BLACK); // erase old button text
    tft.setTextColor(LIGHTGREY);
    tft.setTextSize(2);
    tft.setCursor(130, 173);
    tft.println("NORMAL");
    tft.setCursor(137, 200);
    tft.println("MODE");
    tft.drawRoundRect(115, 160, 90, 70, 8, LIGHTGREY);  // mode  draw buttons outline
  }

  if (mode == 1) {  // eco mode

    tft.fillRoundRect(117, 162, 86, 66, 12, BLACK); // erase old button text
    tft.setTextColor(GREEN);
    tft.setTextSize(2);
    tft.setCursor(144, 173);
    tft.println("ECO");
    tft.setCursor(137, 200);
    tft.println("MODE");
    tft.drawRoundRect(115, 160, 90, 70, 8, GREEN);  // mode  draw buttons outline
  }

  if (mode == 2) {  // auto mode

    tft.fillRoundRect(117, 162, 86, 66, 12, BLACK); // erase old button text
    tft.setTextColor(dutchorange);
    tft.setTextSize(2);
    tft.setCursor(135, 173);
    tft.println("AUTO");
    tft.setCursor(137, 200);
    tft.println("MODE");
    tft.drawRoundRect(115, 160, 90, 70, 8, dutchorange);  // mode  draw buttons outline
  }

  if (mode == 3) {  // cool mode

    tft.fillRoundRect(117, 162, 86, 66, 12, BLACK); // erase old button text
    tft.setTextColor(iceblue);
    tft.setTextSize(2);
    tft.setCursor(135, 173);
    tft.println("COOL");
    tft.setCursor(137, 200);
    tft.println("MODE");
    tft.drawRoundRect(115, 160, 90, 70, 8, iceblue);  // mode  draw buttons outline
  }

  OUTSUB();

}
