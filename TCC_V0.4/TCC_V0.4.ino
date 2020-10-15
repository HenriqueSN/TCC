//Funciona Sensor de humidade, temperatura e temperatura do solo, PH LDR e humidade do solo
//Inclui bibliotecas
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>


//Seta pin do DHT
#define DHTPIN 4

//Seta pin DS18B20
const int DS18B20PIN = 5; 

//Seta pin LDR
const int LDRPIN = 34;

//Seta pin PH
const int PHPIN = 35;

//Seta pin HUMIDADE
const int HUMIDADEPIN = 2;

//Variável para gravar o valor do LDR
int ldrValue = 0;

//Variável para gravar o valor do PH
int phValue = 0;

//Variável para gravar o valor do HUMIDADE
int humidadeValue = 0;

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

  //lê o valor de LDR
   ldrValue = analogRead(LDRPIN);

  //lê o valor do PH 
  phValue = analogRead(PHPIN);

  //lê o valor do PH 
  humidadeValue = analogRead(HUMIDADEPIN);

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
  Serial.println(" Luminosidade:");
  Serial.println(ldrValue);
  Serial.println(" PH:");
  Serial.println(phValue);
    Serial.println(" Humidade do solo:");
  Serial.println(humidadeValue);
  Serial.println();
 

}
