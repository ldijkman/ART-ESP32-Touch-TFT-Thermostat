void redraw_mode_button() {

  if (fullscreenactive == 1) {
    
    
    tft.setTextColor(LIGHTGREY);
    tft.setTextSize(2);
    tft.setCursor(20, 170);
    tft.println("990.50 mbar => Rain");

    tft.setTextColor(LIGHTGREY);
    tft.setTextSize(2);
    tft.setCursor(20, 205);
    tft.println("Humidity  55.7 %");

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

  tft.setTextSize(4);// 0=normal 1=eco 2=a utoc3=
  tft.setCursor(35, 65); //ool
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
