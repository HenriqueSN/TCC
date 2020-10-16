#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>


//Helix IP Address 
const char* orionAddressPath = "104.215.116.161:1026/v2";

//Device ID (example: urn:ngsi-ld:entity:001) 
const char* deviceID = "urn:ngsi-ld:entity:002";

//Wi-Fi Credentials
const char* ssid = "LIVE TIM_FB66_2G"; 
const char* password = "pjfed8jx8d";

#define DHTPIN 4

const int DS18B20PIN = 5; 
  
String guardat;
String guardah;
String guardats;

//Seleciona tipo do DHT
#define DHTTYPE DHT11

//Inicia sensor DHT
DHT dht(DHTPIN, DHTTYPE);

//Inicia o DS18B20
OneWire oneWire(DS18B20PIN);

//passa a referencia do onewire para a biblioteca dallas
DallasTemperature sensors(&oneWire);
 
WiFiClient espClient;
HTTPClient http;


void setup() {
  //setup
  
  Serial.begin(115200);
  
  //start sensor DHT11
  dht.begin();
  sensors.begin();
  
  //Wi-Fi access
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected WiFi network!");
  delay(2000);

  //Serial.println("Creating " + String(deviceID) + " entitie...");
  delay(2000);
  //creating the device in the Helix Sandbox (plug&play) 
  //orionCreateEntitie(deviceID);
  
}
 
void loop(){

   delay(1000);

  float h = dht.readHumidity();
  
  float t = dht.readTemperature();

  sensors.requestTemperatures();
  float ts = sensors.getTempCByIndex(0);

  guardat = String(t);
  guardah = String(h);
  guardats = String(ts);

  /*char msgHumidity[10];
  char msgTemperature[10]; 
  sprintf(msgHumidity,"%d",h);
  sprintf(msgTemperature,"%d",t);*/



  //update 
  Serial.println("Updating data in orion...");
  orionUpdate(deviceID, guardat, guardah, guardats);

  Serial.println("Finished updating data in orion...");

  Serial.print(F("Humidade: "));
  Serial.print(h);
  Serial.print(F("%  Temperatura: "));
  Serial.print(t);
  Serial.print(F("°C "));
   Serial.print(" Temperatura do solo: ");
  Serial.print(ts);
  Serial.println("ºC");
  Serial.println();
}

//plug&play
/*void orionCreateEntitie(String entitieName) {
  
    String bodyRequest = "{\"id\": \"" + entitieName + "\", \"type\": \"sensor\", \"pH\": { \"type\": \"float\", \"value\": \"0\" }, \"luminosidade\": { \"type\": \"float\", \"value\": \"0\" }, \"tempSolo\": { \"type\": \"float\", \"value\": \"0\" }, \"tempAmbiente\": { \"type\": \"float\", \"value\": \"0\" }, \"humidSolo\": { \"type\": \"float\", \"value\": \"0\" }, \"humidAmbiente\": { \"type\": \"float\", \"value\": \"0\" } } ";
    //String bodyRequest = "{\"id\": \"" + entitieName + "\", \"type\": \"iot\", \"temperature\": { \"value\": \"0\", \"type\": \"integer\"},\"humidity\": { \"value\": \"0\", \"type\": \"integer\"}}";
    httpRequest("/entities", bodyRequest);
}*/

//update 
void orionUpdate(String entitieID, String temperature, String humidity, String soilTemperature){
  
     String bodyRequest = "{ \"pH\": { \"type\": \"float\", \"value\": 20 }, \"luminosidade\": { \"type\": \"float\", \"value\": 20 }, \"tempSolo\": { \"type\": \"float\", \"value\": "+soilTemperature+" }, \"tempAmbiente\": { \"type\": \"float\", \"value\": " + temperature + " }, \"humidSolo\": { \"type\": \"float\", \"value\": 20 }, \"humidAmbiente\": { \"type\": \"float\", \"value\": " + humidity + " } } ";
    //String bodyRequest = "{\"temperature\": { \"value\": \""+ temperature + "\", \"type\": \"float\"}, \"humidity\": { \"value\": \""+ humidity +"\", \"type\": \"float\"}}";
    String pathRequest = "/entities/" + entitieID + "/attrs?options=forcedUpdate";
    httpRequest(pathRequest, bodyRequest);
}

//request
void httpRequest(String path, String data)
{
  String payload = makeRequest(path, data);

  if (!payload) {
    return;
  }

  Serial.println("##[RESULT]## ==> " + payload);

}

//request
String makeRequest(String path, String bodyRequest)
{
  String fullAddress = "http://" + String(orionAddressPath) + path;
  http.begin(fullAddress);
  Serial.println("Orion URI request: " + fullAddress);

  http.addHeader("Content-Type", "application/json"); 
  http.addHeader("Accept", "application/json"); 
  http.addHeader("fiware-service", "helixiot"); 
  http.addHeader("fiware-servicepath", "/"); 

Serial.println(bodyRequest);
  int httpCode = http.POST(bodyRequest);

  String response =  http.getString();

  Serial.println("HTTP CODE");
  Serial.println(httpCode);
  
  if (httpCode < 0) {
    Serial.println("request error - " + httpCode);
    return "";
  }

  if (httpCode != HTTP_CODE_OK) {
    return "";
  }

  http.end();

  return response;
}
