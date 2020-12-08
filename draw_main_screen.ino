void drawmainscreen() {

  // main screen layout
  tft.fillScreen(BLACK);
  tft.drawRoundRect(1, 1, 319, 239, 2, DARKGREY);     // show screen size on bigger display

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

  DateTime now = rtc.now();
  
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
    tft.print(" ");
    tft.print(now.year());
    tft.print("  ");


    tft.setTextSize (1);
    tft.setTextColor (LIGHTGREY, BLACK);
    tft.setCursor(130, 13);
    tft.print("  ");
    tft.print(rtc.getTemperature());
    //tft.print(char(247)); //degree sign
    tft.print(" C  ");

  redraw_mode_button();

//while(1==1){

if (tft.getTouch(&x, &y)){
  TempLong = millis();  // store millis() 
}
   tft.setCursor(0, 20);
    tft.setTextColor (RED);
 
    tft.setTextColor (LIGHTGREY, BLACK);
    //tft.print(((millis() - TempLong) / 1000)); tft.print(" ");
    tft.drawRoundRect(10, 10, 300, 140, 8, LIGHTGREY);  // main sreen outline
  //  for (int i = 138; i <= 220; i++) {
    //  tft.drawRoundRect(10, 10, 300, i-1, 8, BLACK);
    //  tft.drawRoundRect(10, 10, 300, i, 8, bordercolor); 
  
  //  delay(25);
  //}
//}
fullscreenactive=1;
}
