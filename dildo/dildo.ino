#include <DHT.h>
#include <DHT_U.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define Type DHT11
#define dhtPin 2

DHT HT(dhtPin, Type);
OneWire ourWire(dhtPin); // Se establece el pin 2 como bus OneWire
DallasTemperature sensors(&ourWire); // Se declara una variable u objeto para nuestro sensor

int humidity;
float tempC;
float tempF;
int dt = 1000;

void setup() {
  Serial.begin(9600);
  HT.begin();
  sensors.begin(); // Se inicia el sensor
  delay(1000); // Esperar un segundo antes de comenzar
}

void loop() {
  // Lecturas del sensor DHT11
  humidity = HT.readHumidity();
  tempC = HT.readTemperature();
  tempF = HT.readTemperature(true);

  Serial.print("DHT11 - Humedad: ");
  Serial.print(humidity);
  Serial.print("% - Temperatura: ");
  Serial.print(tempC);
  Serial.println("°C");

  // Lectura del sensor DS18B20
  sensors.requestTemperatures(); // Se envía el comando para leer la temperatura
  float temp = sensors.getTempCByIndex(0); // Se obtiene la temperatura en ºC

  Serial.print("DS18B20 - Temperatura: ");
  Serial.print(temp);
  Serial.println("°C");

  delay(dt);
}
