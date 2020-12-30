
// ESP32 Read Write variables to SPIFFS
// Working on it, so far this is it, write 11 valeus and read 11 values
// directory listing and values
//
// outputs data on serial terminal 115200
//
// https://oshwlab.com/l.dijkman/esp32-dev-kit-38-pin-to-spi-touch-tft 
// https://github.com/ldijkman/ART-ESP32-Touch-TFT-Thermostat 
// http://www.Arduino.TK

#include "FS.h"
#include "SPIFFS.h"
int CharRead;
char CharArray[16];
/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED 0

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void readFile(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  int i = 1;
  String ThisString;

  while (file.available()) {
    
    CharRead = (file.read());

    if (CharRead == 10) {  //linefeed
      Serial.println(" line feed");
      Serial.print("CharArray=>"); Serial.print(CharArray); Serial.println("<=end");

      Serial.print("ThisString.toInt() "); Serial.println(ThisString.toInt());
      Serial.print("ThisString.toFloat() "); Serial.println(ThisString.toFloat());
      memset(CharArray, 0, sizeof(CharArray));//clear array
    }
    
    if (CharRead == 13) {
      Serial.print(" carriage return ");
      ThisString="";
      
      i = 0;
      delay(2500);
    }

    Serial.print(char(CharRead)); delay(50);
    CharArray[i] = char(CharRead);
    ThisString = String(CharArray);
    i = i + 1;
  }
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- frite failed");
  }
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\r\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("- failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    
   // Serial.print(" - message appended ");
    Serial.println(message); // this is written o file
    delay(2000);
  } else {
    Serial.println("- append failed");
  }
}

void renameFile(fs::FS &fs, const char * path1, const char * path2) {
  Serial.printf("Renaming file %s to %s\r\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("- file renamed");
  } else {
    Serial.println("- rename failed");
  }
}

void deleteFile(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path)) {
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}

void testFileIO(fs::FS &fs, const char * path) {
  Serial.printf("Testing file I/O with %s\r\n", path);

  static uint8_t buf[512];
  size_t len = 0;
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }

  size_t i;
  Serial.print("- writing" );
  uint32_t start = millis();
  for (i = 0; i < 2048; i++) {
    if ((i & 0x001F) == 0x001F) {
      Serial.print(".");
    }
    file.write(buf, 512);
  }
  Serial.println("");
  uint32_t end = millis() - start;
  Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
  file.close();

  file = fs.open(path);
  start = millis();
  end = start;
  i = 0;
  if (file && !file.isDirectory()) {
    len = file.size();
    size_t flen = len;
    start = millis();
    Serial.print("- reading" );
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      if ((i++ & 0x001F) == 0x001F) {
        Serial.print(".");
      }
      len -= toRead;
    }
    Serial.println("");
    end = millis() - start;
    Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
    file.close();
  } else {
    Serial.println("- failed to open file for reading");
  }
}

void setup() {
  Serial.begin(115200);
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
}

void loop() {
  listDir(SPIFFS, "/", 0);
  Serial.println("ListDir 5 second delay");
  delay(5000);

  int sensorValue = 123;
  char sensorstring[12];
  itoa(sensorValue , sensorstring, 10);

  float temperature = 456.789;
  char temp[10];
  String tempAsString;
  dtostrf(temperature, 7, 2, temp); // 7 characters long, with 2 characters after the decimal point.

  writeFile(SPIFFS, "/ART_Thermostat.txt", "start\r\n");

   
  appendFile(SPIFFS, "/ART_Thermostat.txt", sensorstring);//123
  appendFile(SPIFFS, "/ART_Thermostat.txt", "\r\n");    // must write end of line
   
  appendFile(SPIFFS, "/ART_Thermostat.txt", temp);//456,789
  appendFile(SPIFFS, "/ART_Thermostat.txt", "\r\n");    // must write end of line   

  appendFile(SPIFFS, "/ART_Thermostat.txt", dtostrf(temperature, 5, 2, temp));//456.789
  appendFile(SPIFFS, "/ART_Thermostat.txt", "\r\n");    // must write end of line
 
  appendFile(SPIFFS, "/ART_Thermostat.txt", "16000\r\n");
  appendFile(SPIFFS, "/ART_Thermostat.txt", "16000\r\n");
  appendFile(SPIFFS, "/ART_Thermostat.txt", "16000\r\n");
  appendFile(SPIFFS, "/ART_Thermostat.txt", "32000\r\n");
  appendFile(SPIFFS, "/ART_Thermostat.txt", "32000\r\n");
  appendFile(SPIFFS, "/ART_Thermostat.txt", "32000\r\n");
  appendFile(SPIFFS, "/ART_Thermostat.txt", "666.666\r\n");
  Serial.println("done writing to file");
  Serial.println("5 second delay");
  delay(5000);

  readFile(SPIFFS, "/ART_Thermostat.txt");

  Serial.println("done reading file");

  // renameFile(SPIFFS, "/hello.txt", "/foo.txt");
  // readFile(SPIFFS, "/TouchCalData");
  // deleteFile(SPIFFS, "/TouchCalData");
  // testFileIO(SPIFFS, "/test.txt");
  Serial.println( "Test complete" );
  Serial.println("5 second delay");
  delay(5000);

}
