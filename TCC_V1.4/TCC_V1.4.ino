#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>


//Helix IP Address 
const char* orionAddressPath = "143.107.145.24:1026/v2";

//Device ID (example: urn:ngsi-ld:entity:001) 
const char* deviceID = "urn:ngsi-ld:entity:001";

//Wi-Fi Credentials
const char* ssid = "LIVE TIM_FB66_2G"; 
const char* password = "pjfed8jx8d";

//Seta pin LDR
const int LDRPIN = 34;

//Seta pin PH
const int PHPIN = 35;

//Seta pin HUMIDADE
const int UMPIN = 25; 

#define DHTPIN 4

//Define Rele

int rele1 = 23;
int rele2 = 22;

const int DS18B20PIN = 5;


  
String guardat;
String guardah;
String guardats;
String guardal;
String guardaph;
String guardahs;

//Variável para gravar o valor do LDR
int ldrValue = 0;

//Variável para gravar o valor do PH
int phValue = 0;

//Variável para gravar o valor do HUMIDADE
int umValue = 0;
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

  //pinMode(rele1, OUTPUT);
  //pinMode(rele2, OUTPUT);
  
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
  
}
 
void loop(){

  //delay(1000);

  //lê o valor da Humidade
                             
  umValue = analogRead(UMPIN);

  //lê o valor de LDR
  ldrValue = analogRead(LDRPIN);

  //lê o valor do PH 
  phValue = analogRead(PHPIN);

  

  float h = dht.readHumidity();
  
  float t = dht.readTemperature();

  sensors.requestTemperatures();
  float ts = sensors.getTempCByIndex(0);

  if( t > 23){
  pinMode(rele1, OUTPUT);
    //digitalWrite(rele1, HIGH);
  }
   
   if( t < 23){
    //digitalWrite(rele1, LOW);
    pinMode(rele1, INPUT);
  }

  //if(umValue >3999){
   // digitalWrite(rele2, HIGH);
  //}

  //if(umValue <3999){
  //  digitalWrite(rele2, LOW);
  //}

  
  Serial.print(F("Humidade: "));
  Serial.print(h);
  Serial.print(F("%  Temperatura: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(" Temperatura do solo: ");
  Serial.print(ts);
  Serial.println("ºC");
  Serial.println(" Luminosidade:");
  Serial.println(ldrValue);
  Serial.println(" PH:");
  Serial.println(phValue);
  Serial.println(" Umidade:");
  Serial.println(umValue);
  

  guardat = String(t);
  guardah = String(h);
  guardats = String(ts);
  guardal = String(ldrValue);
  guardaph = String(phValue);
  guardahs = String(umValue);

  orionUpdate(deviceID, guardat, guardah, guardats, guardal, guardaph, guardahs);
}

//update 
  void orionUpdate(String entitieID, String temperature, String humidity, String soilTemperature, String light, String ph, String soilHumidity){
  
    String bodyRequest = "{ \"pH\": { \"type\": \"float\", \"value\": "+ph+" }, \"luminosidade\": { \"type\": \"float\", \"value\": "+light+" }, \"tempSolo\": { \"type\": \"float\", \"value\": "+soilTemperature+" }, \"tempAmbiente\": { \"type\": \"float\", \"value\": " + temperature + " }, \"humidSolo\": { \"type\": \"float\", \"value\": "+soilHumidity+" }, \"humidAmbiente\": { \"type\": \"float\", \"value\": " + humidity + " } } ";
    String pathRequest = "/entities/" + entitieID + "/attrs?options=forcedUpdate";

    String fullAddress = "http://" + String(orionAddressPath) + pathRequest;
    http.begin(fullAddress);
    Serial.println("Orion URI request: " + fullAddress);

    http.addHeader("Content-Type", "application/json"); 
    http.addHeader("Accept", "application/json"); 

    http.addHeader("fiware-service", "helixiot"); 
    http.addHeader("fiware-servicepath", "/"); 

    Serial.println(bodyRequest);
    int httpCode = http.POST(bodyRequest);
    //String response =  http.getString();

    Serial.println("HTTP CODE");
    Serial.println(httpCode);
    //digitalWrite(rele1, HIGH);
  
    http.end();

    
    
}
