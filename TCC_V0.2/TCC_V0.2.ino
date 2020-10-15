//Funciona Sensor de humidade, temperatura e temperatura do solo
//Inclui bibliotecas
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>


//Seta pin do DHT
#define DHTPIN 4

//Seta pin DS18B20
const int DS18B20PIN = 5; 

//Seleciona tipo do DHT
#define DHTTYPE DHT11

//Inicia sensor DHT
DHT dht(DHTPIN, DHTTYPE);

//Inicia o DS18B20
OneWire oneWire(DS18B20PIN);

//passa a referencia do onewire para a biblioteca dallas
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  dht.begin();
  sensors.begin();
}

void loop() {

  //lê os valores a cada 1 segundo
  delay(1000);

   
  //lê humidade
  float h = dht.readHumidity();
  //lê temperatura
  float t = dht.readTemperature();

  //lê temperatura do solo
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);

  //verifica se a leitura do DHT funciona, caso não, dá um erro
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Falha ao ler sensor DHT"));
    return;
  }



  Serial.print(F("Humidade: "));
  Serial.print(h);
  Serial.print(F("%  Temperatura: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(" Temperatura do solo: ");
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.println();
 

}
