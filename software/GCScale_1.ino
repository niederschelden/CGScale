
// Import required libraries
//erst server und dateisystem
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
//dann Wägezellen
#include <HX711_ADC.h>

// Zunächst statisch die Zugangsdaten /TODO: Konfiguration des Zugangs
/*
const char* ssid = "FRITZ!Box";
const char* password = "CCCCCCXXXXXXXXX";
const char* wifiHostname = "CGScale";
*/
const char* ssid = "YOURSSID";
const char* password = "YOURSSIDPASS";
const char* wifiHostname = "GCScale";

// dann definiere ich den abschaltzeitpunkt falls keiner Daten abruft und die Variable zur Sofortabschaltung
unsigned long offTime = millis() + 120000;
bool shutNow = false;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//Zellen
//ich muss jeweils zwei zellen mit einer clock laufen lassen, da mir am d1 mini nicht genung pins zur verfügung stehen
const int HX711_dout_1 = D5;  //Daten 1
const int HX711_dout_2 = D6; //Daten 2
const int HX711_sck_1 = D7;//Clockpin 1 und 2
const int HX711_sck_2 = D7;//Clockpin 1 und 2
const int HX711_dout_3 = D1;  //Daten 3
const int HX711_dout_4 = D2; //Daten 4
const int HX711_sck_3 = D3; //Clock 3 und 4
const int HX711_sck_4 = D3; //Clock 3 und 4

//HX711 constructor (dout pin, sck pin)
HX711_ADC LoadCell_1(HX711_dout_1, HX711_sck_1); //HX711 1 
HX711_ADC LoadCell_2(HX711_dout_2, HX711_sck_2); //HX711 2 
HX711_ADC LoadCell_3(HX711_dout_3, HX711_sck_3); //HX711 3
HX711_ADC LoadCell_4(HX711_dout_4, HX711_sck_4); //HX711 4
unsigned long t = 0;

String holeErgebnisse(){
  LoadCell_1.update();
  LoadCell_2.update();
  LoadCell_3.update();
  LoadCell_4.update();
  float a = LoadCell_1.getData();
  float b = LoadCell_2.getData();
  float c = LoadCell_3.getData();
  float d = LoadCell_4.getData();
  return "{\"VL\":" + String(b) + ",\"VR\":" + String(a) + ",\"HL\":" + String(c) + ",\"HR\":" + String(d) + "}";
  }
  
void taraAlle(){
      LoadCell_1.tareNoDelay();
      LoadCell_2.tareNoDelay();
      LoadCell_3.tareNoDelay();
      LoadCell_4.tareNoDelay();
  }

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  delay(100);
  //D8=Wii LED
  pinMode(D8, OUTPUT);
  digitalWrite(D8, HIGH);
  pinMode(D0, OUTPUT);
  digitalWrite(D0, HIGH);

   // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  WiFi.setHostname(wifiHostname);
  Serial.print("Connecting to WiFi.");
  unsigned int zaehler = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(D8, LOW);
    delay(500);
    digitalWrite(D8, HIGH);
    Serial.print(".");
    if(20 < ++zaehler) {
      Serial.println("fehlgeschlagen.... ich schalte ab");
      ESP.deepSleep(0);
      }
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("done");
    digitalWrite(D8, LOW);
    }

  // Print Local IP Address
  Serial.println(WiFi.localIP());
  
  //Calibrierungswerte Setzen
  float calibrationValue_1 = 13278.8; // uncomment this if you want to set this value in the sketch
  float calibrationValue_2 = 14250.6; // uncomment this if you want to set this value in the sketch
  float calibrationValue_3 = 13079.0; // uncomment this if you want to set this value in the sketch
  float calibrationValue_4 = 14278.3; // uncomment this if you want to set this value in the sketch

  // wägezellen starten
  LoadCell_1.begin();
  LoadCell_2.begin();
  LoadCell_3.begin();
  LoadCell_4.begin();
    
  //LoadCell_1.setReverseOutput();
  //LoadCell_2.setReverseOutput();
  
  unsigned long stabilizingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  
  byte loadcell_1_rdy = 0;
  byte loadcell_2_rdy = 0;
  byte loadcell_3_rdy = 0;
  byte loadcell_4_rdy = 0;
  
  while ((loadcell_1_rdy + loadcell_2_rdy + loadcell_3_rdy + loadcell_4_rdy) < 4) { 
    //run startup, stabilization and tare, all modules simultaniously
    if (!loadcell_1_rdy) loadcell_1_rdy = LoadCell_1.startMultiple(stabilizingtime, _tare);
    if (!loadcell_2_rdy) loadcell_2_rdy = LoadCell_2.startMultiple(stabilizingtime, _tare);
    if (!loadcell_3_rdy) loadcell_3_rdy = LoadCell_3.startMultiple(stabilizingtime, _tare);
    if (!loadcell_4_rdy) loadcell_4_rdy = LoadCell_4.startMultiple(stabilizingtime, _tare);
  }
  if (LoadCell_1.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 no.1 wiring and pin designations");
  }
  if (LoadCell_2.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 no.2 wiring and pin designations");
  }
  if (LoadCell_3.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 no.3 wiring and pin designations");
  }
  if (LoadCell_4.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 no.4 wiring and pin designations");
  }

  LoadCell_1.setCalFactor(calibrationValue_1); // user set calibration value (float)
  LoadCell_2.setCalFactor(calibrationValue_2); // user set calibration value (float)
  LoadCell_3.setCalFactor(calibrationValue_3); // user set calibration value (float)
  LoadCell_4.setCalFactor(calibrationValue_4); // user set calibration value (float)
  Serial.println("Wägezellen sind bereit");
  digitalWrite(D8, HIGH);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  // Route for index.html web page
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  // Route to load style.css file
  server.on("/main.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/main.css", "text/css");
  });
  // Route to load main.js file
  server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/main.js", "application/javascript");
  });
  // Route to load image file
  server.on("/logo_vita.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/logo_vita.png", "image/png");
  });
   // Route to load json file
  server.on("/coords.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", holeErgebnisse());
    offTime = millis() + 120000;
  });
  // Tara
  server.on("/tara.html", HTTP_GET, [](AsyncWebServerRequest *request){
    taraAlle();
    request->redirect("/index.html");
  });
  // Ausschalten
  server.on("/off.html", HTTP_GET, [](AsyncWebServerRequest *request){
     request->send(200, "text/plain", "Ich schalte jetzt ab...");
     shutNow = true;
  });
  // Start server
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 1000; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell_1.update()) newDataReady = true;
  LoadCell_2.update();
  LoadCell_3.update();
  LoadCell_4.update();
  
  //get smoothed value from data set
  if ((newDataReady)) {
    if (millis() > t + serialPrintInterval) {
      float a = LoadCell_1.getData();
      float b = LoadCell_2.getData();
      float c = LoadCell_3.getData();
      float d = LoadCell_4.getData();
      Serial.print("rechts vorne (1); ");
      Serial.print(a);
      Serial.print(";    links vorne (2); ");
      Serial.print(b);
      Serial.print(";    rechts hinten (4); ");
      Serial.print(d);
      Serial.print(";    links hinten (3); ");
      Serial.println(c);
      newDataReady = 0;
      t = millis();
    }
  }

  // receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') taraAlle();
  }

  //check if last tare operation is complete
  if (LoadCell_1.getTareStatus() == true) {
    Serial.println("Tare load cell 1 complete");
  }
  if (LoadCell_2.getTareStatus() == true) {
    Serial.println("Tare load cell 2 complete");
  }
  if (LoadCell_3.getTareStatus() == true) {
    Serial.println("Tare load cell 3 complete");
  }
  if (LoadCell_4.getTareStatus() == true) {
    Serial.println("Tare load cell 4 complete");
  }
  if (t > offTime || shutNow == true) {
    delay(100);
    server.reset();
    Serial.println("Ich schalte ab");
    ESP.deepSleep(0);
    }
  yield();
}
