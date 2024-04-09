#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"

#define DHTPIN D2         // Pin Sunergible
#define DHTTYPE DHT11     // Pin DTH11
#define ONE_WIRE_BUS D1   // Pin to connect DS18B20 sensor (GPIO number)

DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const char* ssid = "Alumnos";
const char* password = "";
const char* server = "192.168.1.42"; // Dirección IP de tu servidor local
const int serverPort = 9080; // Puerto del servidor

void setup() {
  Serial.begin(9600);
  dht.begin();
  sensors.begin();

  WiFi.begin(ssid, password);          // Inicialización de conexión WiFi
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  delay(10);

  float tc = dht.readTemperature(false);  // Temperatura DTH11

  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0); // Temperatura Sumergible

  // Crea JSON bajo string llamada payload
  String payload = "{\"room_temperature\":" + String(tc) +
                   ",\"liquid_temperature\":" + String(temperatureC) +
                    "\"}";

  // Envío de solicitud HTTP POST 
  HTTPClient http;
  WiFiClient client;
  String url = "http://" + String(server) + ":" + String(serverPort) + "/postplain/"; // Ruta de tu archivo PHP
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(payload);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("Error in HTTP POST request: ");
    Serial.println(httpResponseCode);
  }

  http.end();

  delay(5000);
}
