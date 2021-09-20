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
String button1text = "Sonoff 1";
String button2text = "Sonoff 2";
String button3text = "Sonoff 3";
String button4text = "Sonoff 4";
String button5text = "Sonoff 5";
String button6text = "Sonoff 6";
String button7text = "<<";
String button8text = ">>";

void settings_three_screen() {
  x = 0; y = 0;
  delay(250);
  tft.fillScreen(BLACK);

  long slowdown;



  TempLong = millis();  // store millis() counter in variable TempLong for screen timeout

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
      if (drawgreendot)tft.drawPixel(x, y, GREEN);
      TempLong = millis();  // reset when touch timeout timer millis() counter in variable TempLong
    }

    if ((millis() - TempLong)  > 60000) { // compare stored TempLong to current millis() counter  screen timeout
      tft.fillScreen(BLACK);
      break;                              // after 60 seconds inactivetie timer break while loop timeout
    }



    if ((millis() - slowdown)  > 1000) {  // ckeck status of sonoff switches every second
      for (int i = 0; i < 6; i++) {
        slowdown = millis();

        Serial.println(i);
        HTTPClient http;

        http.begin(sonoffaddress[i] + "/value");                            // Specify the URL
        //Serial.println(sonoffaddress[i] + "/value");
        http.setConnectTimeout(100);
        int httpCode = http.GET();                                         // Make the request
        sonoffstatus[i] = 2;                                               // if not changed status is 2 = error
        if (httpCode > 0) {                                                // Check for the returning code

          String payload = http.getString();
          payload.trim(); // trim the white space off the string:  other wise the 0 or 1 compare does not work 
          //Serial.println(httpCode);
            Serial.print("payload >");Serial.print(payload); Serial.print("<");Serial.println("-");
          //     Serial.println(payload.toInt());
          if (payload == "0") {
            //   Serial.println("UIT");
            sonoffstatus[i] = 0;
          }
          if (payload == "1") {
            //  Serial.println("AAN");
            sonoffstatus[i] = 1;
          }

          payload = "-";

        }
        else {
          Serial.println("Error on HTTP request");

        }

        http.end(); //Free the resources
        // Serial.print("sonoffstatus[0]=");Serial.println(sonoffstatus[0]);
        // Serial.print("sonoffstatus[1]=");Serial.println(sonoffstatus[1]);
        // Serial.print("sonoffstatus[2]=");Serial.println(sonoffstatus[2]);
        // Serial.print("sonoffstatus[3]=");Serial.println(sonoffstatus[3]);
        // Serial.print("sonoffstatus[4]=");Serial.println(sonoffstatus[4]);
        //  Serial.print("sonoffstatus[5]=");Serial.println(sonoffstatus[5]);
        // Serial.println("");Serial.println("");
     
      } // end for i

    } // end slowdown sonoff status check to every second




    tft.setCursor(275, 210);
    tft.setTextColor (LIGHTGREY, BLACK);
    tft.print(60 - ((millis() - TempLong) / 1000)); tft.print(" ");             // print screen time out counter

    tft.setTextSize(2);




    if ( sonoffstatus[0] == 0) button1(LIGHTGREY, button1text);
    if ( sonoffstatus[0] == 1) button1(GREEN, button1text);
    if ( sonoffstatus[0] == 2) button1(RED, button1text);
    if (button1touch()) {
      sendit(0);                          // sendit does toggle sonoff switch on url address stringarray 0
      delay(250);
    }



    if ( sonoffstatus[1] == 0) button2(LIGHTGREY, button2text);
    if ( sonoffstatus[1] == 1) button2(GREEN, button2text);
    if ( sonoffstatus[1] == 2) button2(RED, button2text);
    if (button2touch()) {
      sendit(1);                           // sendit does toggle sonoff switch on url address stringarray 1
      delay(250);
    }





    if ( sonoffstatus[2] == 0) button3(LIGHTGREY, button3text);
    if ( sonoffstatus[2] == 1) button3(GREEN, button3text);
    if ( sonoffstatus[2] == 2) button3(RED, button3text);
    if (button3touch()) {
      sendit(2);                            // sendit does toggle sonoff switch on url address stringarray 2
      delay(250);
    }



    if ( sonoffstatus[3] == 0) button4(LIGHTGREY, button4text);
    if ( sonoffstatus[3] == 1) button4(GREEN, button4text);
    if ( sonoffstatus[3] == 2) button4(RED, button4text);
    if (button4touch()) {
      sendit(3);                           // sendit does toggle sonoff switch on url address stringarray 3
      delay(250);
    }


    if ( sonoffstatus[4] == 0) button5(LIGHTGREY, button5text);
    if ( sonoffstatus[4] == 1) button5(GREEN, button5text);
    if ( sonoffstatus[4] == 2) button5(RED, button5text);
    if (button5touch()) {
      sendit(4);                           // sendit does toggle sonoff switch on url address stringarray 4
      delay(250);
    }

    if ( sonoffstatus[5] == 0) button6(LIGHTGREY, button6text);
    if ( sonoffstatus[5] == 1) button6(GREEN, button6text);
    if ( sonoffstatus[5] == 2) button6(RED, button6text);
    if (button6touch()) {
      sendit(5);                           // sendit does toggle sonoff switch on url address stringarray 5
      delay(250);
    }


    button7(LIGHTGREY, button7text);
    if (button7touch()) {
      delay(250);
      settings_two_screen();
      break;
    }


    button8(LIGHTGREY, button8text);
    if (button8touch()) {
      delay(250);
      settings_one_screen();
      break;
    }





    x = 0; y = 0;                     // reset touch position

  }                                  // end while 1==1 loop forever




}





void button1(uint32_t color, String text) {
  tft.drawRoundRect(10, 10, 145, 40, 8, color);
  tft.setTextColor(color, BLACK); tft.setCursor(20, 25); tft.print(text);
}
bool button1touch() {
  if (x > 10 && x < 10 + 145 && y > 10 && y < 10 + 40) {
    button1(ORANGE, button1text);
    return 1;
  } else {
    return 0;
  }
}


void button2(uint32_t color, String text) {
  tft.drawRoundRect(165, 10, 145, 40, 8, color);
  tft.setTextColor(color, BLACK); tft.setCursor(175, 25); tft.print(text);
}
bool button2touch() {
  if (x > 165 && x < 165 + 145 && y > 10 && y < 10 + 40) {
    button2(ORANGE, button2text);
    return 1;
  } else {
    return 0;
  }
}

void button3(uint32_t color, String text) {
  tft.drawRoundRect(10, 60, 145, 40, 8,  color);
  tft.setTextColor(color, BLACK); tft.setCursor(20, 75); tft.print(text);
}
bool button3touch() {
  if (x > 10 && x < 10 + 145 && y > 60 && y < 60 + 40) {
    button3(ORANGE, button3text);
    return 1;
  } else {
    return 0;
  }
}

void button4(uint32_t color, String text) {
  tft.drawRoundRect(165, 60, 145, 40, 8,  color);
  tft.setTextColor(color, BLACK); tft.setCursor(175, 75); tft.print(text);
}
bool button4touch() {
  if (x > 165 && x < 165 + 145 && y > 60 && y < 60 + 40) {
    button4(ORANGE, button4text);
    return 1;
  } else {
    return 0;
  }
}

void button5(uint32_t color, String text) {
  tft.drawRoundRect(10, 110, 145, 40, 8,  color);
  tft.setTextColor(color, BLACK); tft.setCursor(20, 125); tft.print(text);
}
bool button5touch() {
  if (x > 10 && x < 10 + 145 && y > 110 && y < 110 + 40) {
    button5(ORANGE, button5text);
    return 1;
  } else {
    return 0;
  }
}

void button6(uint32_t color, String text) {
  tft.drawRoundRect(165, 110, 145, 40, 8,  color);
  tft.setTextColor(color, BLACK); tft.setCursor(175, 125); tft.print(text);
}
bool button6touch() {
  if (x > 165 && x < 165 + 145 && y > 110 && y < 110 + 40) {
    button6(ORANGE, button6text);
    return 1;
  } else {
    return 0;
  }
}


void button7(uint32_t color, String text) {
  tft.drawRoundRect(10, 165, 145, 70, 8, color);
  tft.setTextColor(color, BLACK); tft.setCursor(70, 190); tft.print(text);
}
bool button7touch() {
  if (x > 10 && x < 10 + 145 && y > 165 && y < 165 + 70) {
    button7(GREEN, button7text);
    return 1;
  } else {
    return 0;
  }
}

void button8(uint32_t color, String text) {
  tft.drawRoundRect(165, 165, 145, 70, 8,  color);
  tft.setTextColor(color, BLACK); tft.setCursor(220, 190); tft.print(text);
}
bool button8touch() {
  if (x > 165 && x < 165 + 145 && y > 165 && y < 165 + 70) {
    button8(GREEN, button8text);
    return 1;
  } else {
    return 0;
  }
}



void sendit(int wich) {
  HTTPClient http;
  http.setConnectTimeout(100);
  // toggle
  if ( sonoffstatus[wich] == 0)http.begin(sonoffaddress[wich] + "/LED=ON"); //Specify the URL
  if ( sonoffstatus[wich] == 1)http.begin(sonoffaddress[wich] + "/LED=OFF"); //Specify the URL

  int httpCode = http.GET();                                                  //Make the request

  if (httpCode > 0) { //Check for the returning code

    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
  }

  else {
    Serial.println("Error on HTTP request");

  }

  http.end(); //Free the resources

}

