void redraw_mode_button() {

  if (fullscreenactive == 1) {

    float bartemp = BME280.readPressure() / 100.0F ;

    tft.fillRoundRect(11, 147, 297, 8, 0, BLACK); // erase a piece of bottom rectangle of main screen rectangle

    tft.setTextColor(LIGHTGREY, BLACK);
    tft.setTextSize(2);
    tft.setCursor(20, 170);

    tft.print(bartemp);

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
    if (humidtemp >= 40 && humidtemp <= 60) {  // between 40% and 60% is good for in house
      tft.setTextColor(GREEN, BLACK);
    }
    tft.print(BME280.readHumidity());
    tft.setTextColor(LIGHTGREY, BLACK);
    tft.println(" %  ");
    return;   // skip redraw button if fullscreen active

  }



  tft.drawRoundRect(10, 10, 300, 140, 8, LIGHTGREY);  // main sreen outline

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
