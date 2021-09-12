
// remote light switch for art
// used nodemcu /ESP8266 wemos d1 r1 mini with relais shield




#include <ESP8266WiFi.h>

const char* ssid = "Bangert_30_Andijk";    //  Your Wi-Fi Name
const char* password = "ookikwilerin";        // Wi-Fi Password

const char* soft_ap_ssid = "ART Light Switch Access Point"; // AP wifi name broadcasted in the air
const char*  soft_ap_password= "";

int value = LOW;

// think D1 is the reliscontrolpin of relayshield d1 mini
int LED = 5;   // relais connected to GPI05 (D1) (nodemcu / wmos d1 r1 mini

WiFiServer server(80);



void setup()

{

  Serial.begin(115200); //Default Baudrate

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);



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



void loop()

{

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
    digitalWrite(LED, HIGH); // Turn LED ON
    value = HIGH;
  }

  if (request.indexOf("/LED=OFF") != -1)
  {
    digitalWrite(LED, LOW); // Turn LED OFF
    value = LOW;
  }



  //*------------------HTML Page Code---------------------*//
  client.println("HTTP/1.1 200 OK"); //
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html><body><center><h1><br>");
  client.print(" LIGHT = ");

  if (value == HIGH)
  {
    client.print("ON<br><br>");
    client.println("<a href=\"/LED=OFF\"\"><button style=\"height:160px; width:320px;\"><h1> OFF </h1></button></a><br>");
  }
  else
  {
    client.print("OFF<br><br>"); 
    client.println("<a href=\"/LED=ON\"\"><button style=\"height:160px; width:320px;\"><h1>  ON  </h1></button></a><br>");
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
