
// This function returns an HTML webpage
// It uses the Raw string macro 'R' to place commands in PROGMEM
const char Web_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html><head>
<title>Art, ESP32 Smart Thermostat</title>
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

 


  <script>

      setInterval(function(){getSensorDataeen(); }, 2500); // Call function every set interval 
    setInterval(function(){getSensorDatatwee(); }, 2000);
    setInterval(function(){getSensorDatadrie(); }, 1500);
    setInterval(function(){getSensorDatavier(); }, 1000);

    //document.getElementById("modeid").innerHTML = "Cool Mode"; 
      
       function getSensorDataeen() {
        var xa = new XMLHttpRequest();  
         xa.onreadystatechange = function() {
            if (xa.readyState == 4 && xa.status == 200) {
              document.getElementById("tempid").innerHTML = this.responseText;
            } 
          }
          xa.open("GET", "temp", true);
          xa.send();
       }


      function getSensorDatatwee() {
        var ya= new XMLHttpRequest();        
        ya.onreadystatechange = function() {
          if (ya.readyState == 4 && ya.status == 200) {
            document.getElementById("humidid").innerHTML = this.responseText;
          }  
        }     
        ya.open("GET", "humid", true);
        ya.send();
      }
      
      function getSensorDatadrie() {
        var zza = new XMLHttpRequest();        
        zza.onreadystatechange = function() {
          if (zza.readyState == 4 && zza.status == 200) {
            document.getElementById("pressid").innerHTML = this.responseText;
          } 
        }
        zza.open("GET", "pressure", true);
        zza.send(); 
      }
      
      function getSensorDatavier() {    
        var qqa = new XMLHttpRequest();
        qqa.onreadystatechange = function() {  
          if (qqa.readyState == 4 && qqa.status == 200) {
            document.getElementById("modeid").innerHTML = this.responseText;
            document.getElementById("modeidtext").value = this.responseText;
         }
        }
        qqa.open("GET", "mode", true);
        qqa.send(); 
      }  
       
</script>


  </head>
  <body>
     <div class = "displayobject">
     <center>
       <h1>Art Smart Thermostat <br>
       Art in the Air<br>
       The Art of Temperature Controlled</h1>
       </center>
       <h4><a href="/temp">Temperature</a> <span id="tempid">-- </span> &degC</h4>
       <h4><a href="/humid">Humidity</a> <span id="humidid">-- </span> %</h4>
       <h4><a href="/pressure">Pressure</a> <span id="pressid">-- </span> mbar</h4>
       <h4><a href="/mode">Mode</a> <span id="modeid">-- </span></h4>
       Active Mode = <input type="button" id="modeidtext" value="?" size="2" >=<input type="button" id="textmodeid" value="?"><br><br>

<script>

setInterval(function(){myFunction(); }, 1000);

function myFunction(val) { 
  //alert("The input value has changed. The new value is: " + val);
 
  if(document.getElementById("modeidtext").value==0)document.getElementById("textmodeid").value="Normal Mode";
  if(document.getElementById("modeidtext").value==1)document.getElementById("textmodeid").value="Eco Mode";
  if(document.getElementById("modeidtext").value==2)document.getElementById("textmodeid").value="Auto Mode";
  if(document.getElementById("modeidtext").value==3)document.getElementById("textmodeid").value="Cool Mode";
}
</script>

       <input type="button" onclick="location.href='mode0';" value="Normal Mode" />
       <input type="button" onclick="location.href='mode1';" value="Eco Mode" />
       <input type="button" onclick="location.href='mode2';" value="Auto Mode" />
       <input type="button" onclick="location.href='mode3';" value="Cool Mode" />
     

       <br>
       <br>
       <br><br><br><br><br><br><br><br><br><br><br><br>
       </h1>
     </div>
  
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

void handlemode() { // This function is called by the script to update the sensor value, in this example random data!
  String modevalue = String(mode);
  server.send(200, "text/plain", modevalue); //Send sensor reading when there's a client ajax request
}



void handlemode0() { // This function is called by the script to update the sensor value, in this example random data!
  mode=0;
   String modevalue = String(mode);
    server.send(200, "text/html", Web_page); //Send web page
  //server.send(200, "text/plain", modevalue); //Send sensor reading when there's a client ajax request
}


void handlemode1() { // This function is called by the script to update the sensor value, in this example random data!
  mode=1;
   String modevalue = String(mode);
   server.send(200, "text/html", Web_page); //Send web page
  //server.send(200, "text/plain", modevalue); //Send sensor reading when there's a client ajax request
}



void handlemode2() { // This function is called by the script to update the sensor value, in this example random data!
  mode=2;
   String modevalue = String(mode);
   server.send(200, "text/html", Web_page); //Send web page
  //server.send(200, "text/plain", modevalue); //Send sensor reading when there's a client ajax request
}

void handlemode3() { // This function is called by the script to update the sensor value, in this example random data!
  mode=3;
   String modevalue = String(mode);
   server.send(200, "text/html", Web_page); //Send web page
  //server.send(200, "text/plain", modevalue); //Send sensor reading when there's a client ajax request
}
