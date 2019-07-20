#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "index.h"

#define DHTPIN A0 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
#define LED_BUILTIN 2
#define LED_BUTTON 13
#define SSID "NET_2GDB14C2"
#define PASSWORD "4BDB14C2"

// Conecte pino 1 do sensor (esquerda) ao +5V (3V no NodeMCU)
// Conecte pino 2 do sensor ao pino de dados definido em seu Arduino
// Conecte pino 4 do sensor ao GND
// Conecte o resistor de 10K entre pin 2 (dados) 
// e ao pino 1 (VCC) do sensor
DHT dht(DHTPIN, DHTTYPE);
float t,h;
ESP8266WebServer server(80);

long previousLEDMillis = 0;
byte ledState;
String ledButton = "OFF";

void setup() 
{
  Serial.begin(115200);
  Serial.println(" --- Inicializando a aplicação ESP8266 --- ");
  setupWifi();
  initConnectionsLED();
  dht.begin();
}

void loop() 
{
  server.handleClient();
}

void setupWifi() {
  Serial.print("Connecting to: ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 

  if(WiFi.status() == WL_CONNECTED) //If WiFi connected to hot spot then start mDNS
  {
    if (MDNS.begin("shineca.local")) {  //Start mDNS with name esp8266
      Serial.println("MDNS started");
    }
  }
  
  server.on("/", handleRoot);  //Associate handler function to path
  server.on("/setLED", handleLED);
  server.on("/readTemp", handleTemp);
  server.on("/readHum", handleHum);
  server.begin();
  Serial.println("HTTP server started");
}

void handleTemp() { 
 readDHTSensor();
 server.send(200, "text/plane", String(t));
}

void handleHum() {
 readDHTSensor();
 server.send(200, "text/plane", String(h));
}
 
void handleLED() {
  String ledState = "OFF";
  String t_state = server.arg("ledState");
  if(t_state == "1") {
    digitalWrite(LED_BUILTIN,LOW); //LED ON
    ledState = "ON"; //Feedback parameter
  } else {
    digitalWrite(LED_BUILTIN,HIGH); //LED OFF
    ledState = "OFF"; //Feedback parameter  
  }
  server.send(200, "text/plane", ledState); //Send web page
}

void handleRoot() {
  String page = MAIN_page;
  server.send(200, "text/html", page);
}

void readDHTSensor() {
  // A leitura da temperatura e umidade pode levar 250ms!
  // O atraso do sensor pode chegar a 2 segundos.
  h = dht.readHumidity();
  t = dht.readTemperature();
  // testa se retorno é valido, caso contrário algo está errado.
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Umidade: ");
    Serial.print(h);
    Serial.print(" %t");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
  }
  delay(500);  
}

void blinkLed(boolean flg) {
  if(flg) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousLEDMillis >= 500) {
      previousLEDMillis = currentMillis;  
      ledState = !ledState;
      digitalWrite(LED_BUILTIN, ledState);
    }
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void initConnectionsLED() {
  ledState = HIGH;
  pinMode(LED_BUILTIN, OUTPUT);   
  digitalWrite(LED_BUILTIN, ledState);
}
