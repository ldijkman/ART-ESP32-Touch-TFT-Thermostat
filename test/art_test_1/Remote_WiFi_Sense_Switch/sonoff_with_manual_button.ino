// remote light switch for art
// used sonoff basic wifi smart switch relais 10A
// why via chinese server
// reprogram it with my program
// connect usb with switch pressed to get into programming mode use esp01 programmer
// you wll loose the chinese software
/*
  GPIO00 - BUTTON
  GPIO12 - RELAY
  GPIO13 - Sonoff_Relais_pin1

  GPIO03 - RX PIN
  GPIO01 - TX PIN
*/

#include <ESP8266WiFi.h>

const char* ssid = "Bangert_30_Andijk";    //  Your Wi-Fi Name
const char* password = "ookikwilerin";        // Wi-Fi Password

const char* soft_ap_ssid = "ART Light Switch AP"; // AP wifi name broadcasted in the air
const char*  soft_ap_password = "";

int value = LOW;

// think D1 is the reliscontrolpin of relayshield d1 mini
//int Sonoff_Relais_pin = 5;   // relais connected to GPI05 (D1) (nodemcu / wmos d1 r1 mini
int Sonoff_Relais_pin = 12;    // sonoff relais control pin
WiFiServer server(80);



void setup()

{

  Serial.begin(115200); //Default Baudrate

  pinMode(Sonoff_Relais_pin, OUTPUT);
  digitalWrite(Sonoff_Relais_pin, LOW);

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  pinMode(0, INPUT);


  Serial.print("Connecting to the NetWork");
  WiFi.mode(WIFI_AP_STA);                                              // Connect to your wifi
  WiFi.softAP(soft_ap_ssid, soft_ap_password);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {

    delay(500);
    Serial.print(".");

  }

  Serial.println("WiFi connected");

  server.begin();  // Starts the Server

  Serial.println("Server started");



  Serial.print("IP Address of network: "); // will IP address on Serial Monitor

  Serial.println(WiFi.localIP());

  Serial.print("Copy and paste the following URL: https://"); // Will print IP address in URL format

  Serial.print(WiFi.localIP());

  Serial.println("/");

}



void loop(){

  
  if (digitalRead(0)== LOW){                 // manual control on/off toggle light with pushbutton on sonoff
    while (digitalRead(0)== LOW){/*wait for button release*/} 
    if (value == LOW){
      digitalWrite(Sonoff_Relais_pin, HIGH); // Turn Sonoff_Relais_pin ON
      value = HIGH;
      digitalWrite(13, LOW);
    }
    else{
      digitalWrite(Sonoff_Relais_pin, LOW); // Turn Sonoff_Relais_pin OFF
      value = LOW;
      digitalWrite(13, HIGH);
    }

  }

  WiFiClient client = server.available();

  if (!client)

  {

    return;

  }

  Serial.println("Waiting for new client");

  while (!client.available())
  {
    delay(1);
  }



  String request = client.readStringUntil('\r');

  Serial.println(request);

  client.flush();





  if (request.indexOf("/LED=ON") != -1)
  {
    digitalWrite(Sonoff_Relais_pin, HIGH); // Turn Sonoff_Relais_pin ON
    value = HIGH;
  }

  if (request.indexOf("/LED=OFF") != -1)
  {
    digitalWrite(Sonoff_Relais_pin, LOW); // Turn Sonoff_Relais_pin OFF
    value = LOW;
    digitalWrite(13, HIGH);
  }



  //*------------------HTML Page Code---------------------*//
  client.println("HTTP/1.1 200 OK"); //
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("<meta http-equiv=\"refresh\" content=\"5; URL=http://");
  client.print(WiFi.localIP());
  client.print("\">");
  client.print("<body><center><h1><br>");
  client.print(" LIGHT = ");

  if (value == HIGH)
  {
    client.print("ON<br>");
    client.println("<a href=\"/LED=OFF\"\"><button style=\"height:160px; width:320px; background-color:yellow;\"><h1> Turn OFF </h1></button></a><br>");
  }
  else
  {
    client.print("OFF<br>");
    client.println("<a href=\"/LED=ON\"\"><button style=\"height:160px; width:320px; background-color:gray;\"><h1>  Turn ON  </h1></button></a><br>");
  }

  client.print("<br><br><br><a href=\"http://");
  client.print(WiFi.localIP());
  client.print("\"><h1>");
  client.print(WiFi.localIP());
  client.println("</h1></a><br>");

  client.println("</h1></center></body></html>");



  delay(1);

  Serial.println("Client disonnected");

  Serial.println("");

}
