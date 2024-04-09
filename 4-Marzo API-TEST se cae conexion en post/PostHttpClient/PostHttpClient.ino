#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"

#define DHTPIN D2         // Pin Sunergible
#define DHTTYPE DHT11     // Pin DTH11
#define ONE_WIRE_BUS D1   // Pin to connect DS18B20 sensor (GPIO number)
#define SERVER_IP "192.168.137.173"// IPCONFIG 

#ifndef STASSID
#define STASSID "Arduino" // ACTUALIZA RED
#define STAPSK "12346578" // ACTUALIZA CONTRASEÑA
#endif

DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  dht.begin();
  sensors.begin();

  WiFi.begin(STASSID, STAPSK);          // Inicialización de conexión WiFi
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  delay(10);

  float tc = dht.readTemperature(false);  // Temperatura DTH11

  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0); // Temperatura Sumergible

  // Crea JSON bajo string llamada payload
  String payload = "{\"room_temperature\":" + String(tc) +
                   ",\"liquid_temperature\":" + String(temperatureC) +
                   "}";
  Serial.println(payload);

  // Imprimir la URL antes de llamar a http.begin()
  Serial.print("Connecting to URL: ");
  Serial.print("http://");
  Serial.print(SERVER_IP);
  Serial.println("/PHP-API/insertData.php");

  // Envío de solicitud HTTP POST 
  HTTPClient http;
  WiFiClient client;

  http.begin(client, "http://" + String(SERVER_IP) + "/PHP-API/insertData.php");  // HTTP
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(payload);

  if (httpCode > 0) {
      // Se ha enviado el encabezado HTTP y se ha manejado la respuesta del servidor
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // Archivo encontrado en el servidor
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      } else {
        Serial.println("No se encontró el archivo en el servidor.");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

  http.end();

  delay(600000);
}

