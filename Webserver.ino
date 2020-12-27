void handleRoot() {
  //String s = Web_page;             //Display HTML contents
  server.send(200, "text/html", Web_page); //Send web page
}

void handleTEMP() { // This function is called by the script to update the sensor value, in this example random data!
  String temp = String(BME280.readTemperature(),2);
  server.send(200, "text/plain", temp);
}

void handleHUMI() { // This function is called by the script to update the sensor value, in this example random data!
  String humi = String(BME280.readHumidity(),2);
  server.send(200, "text/plain", humi); //Send sensor reading when there's a client ajax request
}

void handlePRES() { // This function is called by the script to update the sensor value, in this example random data!
  String pres = String(BME280.readPressure() / 100.0F,2);
  server.send(200, "text/plain", pres); //Send sensor reading when there's a client ajax request
}
