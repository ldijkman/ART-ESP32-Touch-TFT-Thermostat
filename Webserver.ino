
// This function returns an HTML webpage 
// It uses the Raw string macro 'R' to place commands in PROGMEM
const char Web_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <style>
    .displayobject{
       font-family: sans-serif;
       margin: auto;
       text-align: center;
       width: 100%;
       height: 100%;
       border: 3px solid #000000;
       padding: 10px;
       background: #558ED5;
    }
    h1 {
      font-size: 30px;
      color: white;
    }
    h4 {
      font-size: 30px;
      color: yellow;
    }
  </style>
  <body>
     <div class = "displayobject">
     <center>
       <h1>Art Smart Thermostat <br>
       Art in the Air<br>
       The Art of Temperature Controlled</h1>
       </center>
       <br>
       <h4><a href="/temp">Temperature</a> <span id="TEMPvalue">0</span>&deg</h4>
       <h4><a href="/humid">Humidity</a> <span id="HUMIDvalue">0</span>%</h4>
       <h4><a href="/pressure">Pressure</a> <span id="PRESSvalue">0</span>mbar</h4><br>
       <h1><br>
       <br>
       <br>
       <br><br><br><br><br><br><br><br><br><br><br><br>
       </h1>
     </div>
     <script>
       setInterval(function() {getSensorData();}, 1000); // Call the update function every set interval e.g. 1000mS or 1-sec
  
       function getSensorData() {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("TEMPvalue").innerHTML = this.responseText;
          }
        };
        xhttp.open("GET", "temp", true);
        xhttp.send();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("HUMIDvalue").innerHTML = this.responseText;
          }
        };
        xhttp.open("GET", "humid", true);
        xhttp.send();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("PRESSvalue").innerHTML = this.responseText;}
        };  
        xhttp.open("GET", "pressure", true);
        xhttp.send(); 
      }
    </script>
  </body>
</html>
)=====";


void handleRoot() {
  //String s = Web_page;             //Display HTML contents
  server.send(200, "text/html", Web_page); //Send web page
}

void handleTEMP() { // This function is called by the script to update the sensor value, in this example random data!
  String temp = String(BME280.readTemperature(),2);
  server.send(200, "text/plain", temp);
}

void handleHUMID() { // This function is called by the script to update the sensor value, in this example random data!
  String humid = String(BME280.readHumidity(),2);
  server.send(200, "text/plain", humid); //Send sensor reading when there's a client ajax request
}

void handlePRESS() { // This function is called by the script to update the sensor value, in this example random data!
  String pressure = String(BME280.readPressure() / 100.0F,2);
  server.send(200, "text/plain", pressure); //Send sensor reading when there's a client ajax request
}
