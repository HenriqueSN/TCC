//Inclui bibliotecas
#include "DHT.h"

//Seta pin do DHT
#define DHTPIN 4

//Seleciona tipo do DHT
#define DHTTYPE DHT11

//Inicia sensor DHT
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {

  //lê os valores a cada 1 segundo
  delay(1000);

  float h = dht.readHumidity();
  
  float t = dht.readTemperature();
 

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
  Serial.println();
 

}
