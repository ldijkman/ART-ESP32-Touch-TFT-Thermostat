// esp8266 relais switch config portal mdns http//?????.local hostnames
// pitty android chrome does not do mdns
// use bonjourbrowser app for android
// apple does do mdns?
// my raspberry pi does mdns
//
// still working on this code not all done yet
// https://github.com/ldijkman/ART-ESP32-Touch-TFT-Thermostat/blob/main/test/art_test_1/Remote_WiFi_Sense_Switch/ESP8266_MDNS_Hostname_relais_switch.ino.ino
//
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

#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#ifdef ESP32
#include <SPIFFS.h>
#endif

#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

WiFiServer server(80);

//define your default values here, if there are different values in config.json, they are overwritten.
char mdns_hostname[40] = "Unique Room name?";
char relaispin[6] = "12";
char buttonpin[34] = "0";
char statusledpin[34] = "13";

int intrelaispin;
int intbuttonpin;
int intstatusledpin;

int value = LOW;

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  //clean FS, for testing
  //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);

#ifdef ARDUINOJSON_VERSION_MAJOR >= 6
        DynamicJsonDocument json(1024);
        auto deserializeError = deserializeJson(json, buf.get());
        serializeJson(json, Serial);
        if ( ! deserializeError ) {
#else
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
#endif
          Serial.println("\nparsed json");
          strcpy(mdns_hostname, json["mdns_hostname"]);
          strcpy(relaispin, json["relaispin"]);
          strcpy(buttonpin, json["buttonpin"]);
        } else {
          Serial.println("failed to load json config");
        }
        configFile.close();
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read

  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_MDNS_HostName("mdnshostname", "MDNS Http://????.local hostname", mdns_hostname, 40);
  WiFiManagerParameter custom_relaispin("relaispin", "Relais sonoff 12, Wemos mini 5.", relaispin, 6);
  WiFiManagerParameter custom_buttonpin("buttonpin", "Button Sonoff 0.", buttonpin, 32);
  WiFiManagerParameter custom_statusledpin("statusled", "Status LED Sonoff 13", statusledpin, 32);


  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
  //wifiManager.setSTAStaticIPConfig(IPAddress(10, 0, 1, 99), IPAddress(10, 0, 1, 1), IPAddress(255, 255, 255, 0));

  //add all your parameters here
  wifiManager.addParameter(&custom_MDNS_HostName);
  wifiManager.addParameter(&custom_relaispin);
  wifiManager.addParameter(&custom_buttonpin);
  wifiManager.addParameter(&custom_statusledpin);

  //reset settings - for testing
  // wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifiManager.setTimeout(300);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("AutoConnectAP", "")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  //read updated parameters
  strcpy(mdns_hostname, custom_MDNS_HostName.getValue());
  strcpy(relaispin, custom_relaispin.getValue());
  strcpy(buttonpin, custom_buttonpin.getValue());
  strcpy(statusledpin, custom_statusledpin.getValue());

  Serial.println("The values in the file are: ");
  Serial.println("\tmdns_hostname : " + String(mdns_hostname));
  Serial.println("\trelaispin : " + String(relaispin));
  Serial.println("\tbuttonpin : " + String(buttonpin));
  Serial.println("\tstatusledpin : " + String(statusledpin));

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
#ifdef ARDUINOJSON_VERSION_MAJOR >= 6
    DynamicJsonDocument json(1024);
#else
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
#endif
    json["mdns_hostname"] = mdns_hostname;
    json["relaispin"] = relaispin;
    json["buttonpin"] = buttonpin;
    json["statusledpin"] = statusledpin;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

#ifdef ARDUINOJSON_VERSION_MAJOR >= 6
    serializeJson(json, Serial);
    serializeJson(json, configFile);
#else
    json.printTo(Serial);
    json.printTo(configFile);
#endif
    configFile.close();
    //end save
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());





  server.begin();  // Starts the Server

  Serial.println("Server started");

  while (!MDNS.begin(mdns_hostname)) {             // Start the mDNS responder for esp8266.local
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.print("mDNS responder started http://");
  Serial.print(mdns_hostname);
  Serial.println(".local");

  MDNS.addService("http", "tcp", 80);
  Serial.println("mDNS responder started");


  intrelaispin = String(relaispin).toInt();
  Serial.println(intrelaispin);
intbuttonpin = String(intbuttonpin).toInt();
  Serial.println(intbuttonpin);
intstatusledpin = String(intstatusledpin).toInt();
  Serial.println(intstatusledpin);


  pinMode(intrelaispin, OUTPUT);
  digitalWrite(intrelaispin, LOW);

  pinMode(intstatusledpin, OUTPUT);
  digitalWrite(intstatusledpin, HIGH);

  pinMode(intbuttonpin, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:




  MDNS.update();

  if (digitalRead(intbuttonpin) == LOW) {               // manual control on/off toggle light with pushbutton on sonoff
    while (digitalRead(intbuttonpin) == LOW) {
      /*wait for button release*/
    }
    if (value == LOW) {
      digitalWrite(String(relaispin).toInt(), HIGH); // Turn relaispin ON
      value = HIGH;
      digitalWrite(intstatusledpin, LOW);
    }
    else {
      digitalWrite(String(relaispin).toInt(), LOW); // Turn relaispin OFF
      value = LOW;
      digitalWrite(intstatusledpin, HIGH);
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

  if (request.indexOf("/value") != -1)
  {
    client.println("HTTP/1.1 200 OK"); //
    client.println("Content-Type: text/html");
    client.println("");
    //client.println("<!DOCTYPE HTML>");
    //client.println("<html>");
    client.println(value); //
    return;
  }



  if (request.indexOf("/LED=ON") != -1)
  {
    digitalWrite(intrelaispin, HIGH); // Turn relaispin ON
    value = HIGH;
    digitalWrite(intstatusledpin, LOW  );
    Serial.println(intrelaispin);
  }

  if (request.indexOf("/LED=OFF") != -1)
  {
    digitalWrite(intrelaispin, LOW); // Turn relaispin OFF
    value = LOW;
    digitalWrite(intstatusledpin, HIGH);
    Serial.println(intrelaispin);
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
  client.print("<body><center><h4>");
  client.print(" LIGHT = ");

  if (value == HIGH)
  {
    client.print("ON</h4>");
    client.println("<a href=\"/LED=OFF\"\"><button style=\"height:160px; width:320px; background-color:yellow;\"><h1> Turn OFF </h1></button></a>");
  }
  else
  {
    client.print("OFF</h4>");
    client.println("<a href=\"/LED=ON\"\"><button style=\"height:160px; width:320px; background-color:gray;\"><h1>  Turn ON  </h1></button></a>");
  }
  client.print("<a href=\"http://");
  client.print(mdns_hostname);
  client.print(".local\"><h4>http://");
  client.print(mdns_hostname);
  client.println(".local</h4></a>");


  client.print("<a href=\"http://");
  client.print(WiFi.localIP());
  client.print("\"><h4>http://");
  client.print(WiFi.localIP());
  client.println("</h4></a>");

  /*
    <script type = "text/javascript" >
    function preventBack() {
      window.history.forward();
    }
    setTimeout("preventBack()", 0);
    window.onunload = function () {
      null
    };
    < / script >
  */



  client.println("</center></body></html>");



  delay(1);

  Serial.println("Client disonnected");

  Serial.println("");
}
