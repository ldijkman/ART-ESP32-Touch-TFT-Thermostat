// esp8266 relais switch config portal mdns http//?????.local hostnames
// i noticed that my sonoffs are ESP8285 but it programs well with => generic ESP8266 settings of arduino ide
//
// https://www.youtube.com/watch?v=sRqMcyHR-Ls
//
// does anyone know howto add an OTA button on the switch webpage???????? (over the air update upgrade switch)
//
// each switch webpage automaticly linked lists other http://*.local devices on the local network
// no settings hardcoded => all settings configurable in wifimanager
//
// no hard to search or remember ip numbers
// find your esp devices easy with mdns http://kitchen.local http://living.local etcetera
// and each esp device shows a linked list of the other devices on the network on its webpage
//
// pitty android chrome does not do mdns => use bonjourbrowser app for android https://play.google.com/store/apps/details?id=de.wellenvogel.bonjourbrowser&hl=en&gl=US
// apple does do mdns?
// my raspberry pi does mdns
//
// still working on this code not all done yet
//
// code used from https://github.com/tzapu/WiFiManager/blob/master/examples/Parameters/SPIFFS/AutoConnectWithFSParameters/AutoConnectWithFSParameters.ino
// code used from https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/examples/LEAmDNS/mDNS_ServiceMonitor/mDNS_ServiceMonitor.ino
//
// https://www.electronicwings.com/users/LuberthDijkman/projects/1337/smart-thermostat-art-the-best-nest-thing
//
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
// keep above message intact?

#include <FS.h>                   // this needs to be first, or it all crashes and burns...
#include <WiFiManager.h>          // https://github.com/tzapu/WiFiManager

#ifdef ESP32
#include <SPIFFS.h>
#endif

#include <ArduinoJson.h>          // https://github.com/bblanchon/ArduinoJson
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>

WiFiServer server(80);

//define your default values here, if there are different values in config.json, they are overwritten.
char mdns_hostname[40] = "UniqueRoomName";
char relaispin[5] = "12";                     // should be byte or int ???
char buttonpin[5] = "0";                      // should be byte or int ???
char statusledpin[5] = "13";                  // should be byte or int ???
                                         // this should be done easier
int intrelaispin;                             // for char string to int
int intbuttonpin;                             // for char string to int  
int intstatusledpin;                          // for char string to int

int value = LOW;
/*
   Global defines and vars
*/

#define SERVICE_PORT                                    80                                  // HTTP port

#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif
//const char*                                    ssid                    = STASSID;
//const char*                                    password                = STAPSK;

char*                                          pcHostDomain            = 0;        // Negotiated host domain
bool                                           bHostDomainConfirmed    = false;    // Flags the confirmation of the host domain
MDNSResponder::hMDNSService                    hMDNSService            = 0;        // The handle of the http service in the MDNS responder
MDNSResponder::hMDNSServiceQuery               hMDNSServiceQuery       = 0;        // The handle of the 'http.tcp' service query in the MDNS responder

const String                                   cstrNoHTTPServices      = "Currently no 'http.tcp' services in the local network!<br/>";
String                                         strHTTPServices         = cstrNoHTTPServices;

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

  //clean FS, for testing              or just program with => all flash contents?
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
          strcpy(statusledpin, json["statusledpin"]);
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
  //here  "AutoConnectAP-chipid"
  //and goes into a blocking loop awaiting configuration

#define ESP_getChipId()   (ESP.getChipId())
  String soft_ap_ssid = "AutoConnectAP-" + String(ESP_getChipId(), HEX);  // wifi accesspoint name broadcasted in the air make it unique with chipid
  char buf[25];
  soft_ap_ssid.toCharArray(buf,25);                     // i dont know how strings an chars work could/should be done easier
  if (!wifiManager.autoConnect(buf, "")) {              // password field is left empty "" no password
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yipee..yeey! :)");

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
  intbuttonpin = String(buttonpin).toInt();
  Serial.println(intbuttonpin);
  intstatusledpin = String(statusledpin).toInt();
  Serial.println(intstatusledpin);


  pinMode(intrelaispin, OUTPUT);
  digitalWrite(intrelaispin, LOW);

  pinMode(intstatusledpin, OUTPUT);
  digitalWrite(intstatusledpin, HIGH);

  pinMode(intbuttonpin, INPUT);

  // Setup MDNS responders
  MDNS.setHostProbeResultCallback(hostProbeResult);

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

  IPAddress ip = WiFi.localIP();
  String ipStr = ip.toString();
  //String s = "<!DOCTYPE HTML>\r\n<html><h3><head>Hello from ";
  String s = "";
  //s += WiFi.hostname() + ".local at " + WiFi.localIP().toString() + "</h3></head>";
  s += "<h4>Local HTTP services are :</h4>";
  //s += "";
  for (auto info :  MDNS.answerInfo(hMDNSServiceQuery)) {
    //s += "";
    //s += info.serviceDomain();
    if (info.hostDomainAvailable()) {
      //s += "<br/>Hostname: ";
     // s += "<br/>";
      s += String(info.hostDomain());                                                   // .local name
      //s += (info.hostPortAvailable()) ? (":" + String(info.hostPort())) : "";
    }
    if (info.IP4AddressAvailable()) {
      //s += "<br/>IP4:";
      s += "<br/>";
      for (auto ip : info.IP4Adresses()) {
        s += "<a href=\"http://" + ip.toString() + "\">" + ip.toString() + "</a><br><br>";    // make ip adres a link
      }
    }
    if (info.txtAvailable()) {
      s += "<br/>TXT:<br/>";
      for (auto kv : info.keyValues()) {
        s += "\t" + String(kv.first) + " : " + String(kv.second) + "<br/>";
      }
    }
    s += "";
  }
  s += "<br/>";

  client.print(s);

  client.println("</center></body></html>");



  delay(1);

  Serial.println("Client disonnected");

  Serial.println("");
}




/*
   setStationHostname
*/
bool setStationHostname(const char* p_pcHostname) {

  if (p_pcHostname) {
    WiFi.hostname(p_pcHostname);
    Serial.printf("setStationHostname: Station hostname is set to '%s'\n", p_pcHostname);
    return true;
  }
  return false;
}

/*
   MDNSServiceQueryCallback
*/

void MDNSServiceQueryCallback(MDNSResponder::MDNSServiceInfo serviceInfo, MDNSResponder::AnswerType answerType, bool p_bSetContent) {
  String answerInfo;
  switch (answerType) {
    case MDNSResponder::AnswerType::ServiceDomain :
      answerInfo = "ServiceDomain " + String(serviceInfo.serviceDomain());
      break;
    case MDNSResponder::AnswerType::HostDomainAndPort :
      answerInfo = "HostDomainAndPort " + String(serviceInfo.hostDomain()) + ":" + String(serviceInfo.hostPort());
      break;
    case MDNSResponder::AnswerType::IP4Address :
      answerInfo = "IP4Address ";
      for (IPAddress ip : serviceInfo.IP4Adresses()) {
        answerInfo += "- " + ip.toString();
      };
      break;
    case MDNSResponder::AnswerType::Txt :
      answerInfo = "TXT " + String(serviceInfo.strKeyValue());
      for (auto kv : serviceInfo.keyValues()) {
        answerInfo += "\nkv : " + String(kv.first) + " : " + String(kv.second);
      }
      break;
    default :
      answerInfo = "Unknown Answertype";
  }
  Serial.printf("Answer %s %s\n", answerInfo.c_str(), p_bSetContent ? "Modified" : "Deleted");
}

/*
   MDNSServiceProbeResultCallback
   Probe result callback for Services
*/

void serviceProbeResult(String p_pcServiceName,
                        const MDNSResponder::hMDNSService p_hMDNSService,
                        bool p_bProbeResult) {
  (void) p_hMDNSService;
  Serial.printf("MDNSServiceProbeResultCallback: Service %s probe %s\n", p_pcServiceName.c_str(), (p_bProbeResult ? "succeeded." : "failed!"));
}

/*
   MDNSHostProbeResultCallback
   Probe result callback for the host domain.
   If the domain is free, the host domain is set and the http service is
   added.
   If the domain is already used, a new name is created and the probing is
   restarted via p_pMDNSResponder->setHostname().
*/

void hostProbeResult(String p_pcDomainName, bool p_bProbeResult) {

  Serial.printf("MDNSHostProbeResultCallback: Host domain '%s.local' is %s\n", p_pcDomainName.c_str(), (p_bProbeResult ? "free" : "already USED!"));

  if (true == p_bProbeResult) {
    // Set station hostname
    setStationHostname(pcHostDomain);

    if (!bHostDomainConfirmed) {
      // Hostname free -> setup clock service
      bHostDomainConfirmed = true;

      if (!hMDNSService) {
        // Add a 'http.tcp' service to port 'SERVICE_PORT', using the host domain as instance domain
        hMDNSService = MDNS.addService(0, "http", "tcp", SERVICE_PORT);
        if (hMDNSService) {
          MDNS.setServiceProbeResultCallback(hMDNSService, serviceProbeResult);

          // Add some '_http._tcp' protocol specific MDNS service TXT items
          // See: http://www.dns-sd.org/txtrecords.html#http
          MDNS.addServiceTxt(hMDNSService, "user", "");
          MDNS.addServiceTxt(hMDNSService, "password", "");
          MDNS.addServiceTxt(hMDNSService, "path", "/");
        }

        // Install dynamic 'http.tcp' service query
        if (!hMDNSServiceQuery) {
          hMDNSServiceQuery = MDNS.installServiceQuery("http", "tcp", MDNSServiceQueryCallback);
          if (hMDNSServiceQuery) {
            Serial.printf("MDNSProbeResultCallback: Service query for 'http.tcp' services installed.\n");
          } else {
            Serial.printf("MDNSProbeResultCallback: FAILED to install service query for 'http.tcp' services!\n");
          }
        }
      }
    }
  } else {
    // Change hostname, use '-' as divider between base name and index
    if (MDNSResponder::indexDomain(pcHostDomain, "-", 0)) {
      MDNS.setHostname(pcHostDomain);
    } else {
      Serial.println("MDNSProbeResultCallback: FAILED to update hostname!");
    }
  }
}
