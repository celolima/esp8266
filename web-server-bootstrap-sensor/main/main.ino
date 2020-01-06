#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ESP8266_D1_PIN_ORDER

#include <DHT.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <FastLED.h>

#include "index.h"

#define DHTPIN A0 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
#define LED_BUILTIN 2
#define LED_BUTTON 13
#define SSID "NET_2GDB14C2"
#define PASSWORD "4BDB14C2"

#define LED_PIN     5
#define NUM_LEDS    4
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;

// Conecte pino 1 do sensor (esquerda) ao +5V (3V no NodeMCU)
// Conecte pino 2 do sensor ao pino de dados definido em seu Arduino
// Conecte pino 4 do sensor ao GND
// Conecte o resistor de 10K entre pin 2 (dados) e ao pino 1 (VCC) do sensor

DHT dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80);

float t = 0,h = 0;

byte ledState;
String ledButton = "OFF";

boolean isPalette = true;
int colorChoosen = 0;

void setup() {
  Serial.begin(115200);
  Serial.println(" --- Inicializando a aplicação ESP8266 --- ");
  setupLEDStrip();
  setupWifi();
  initConnectionsLED();
  dht.begin();
}

void loop() {
  MDNS.update();
  server.handleClient();
  //showLED();
}

/*------------------------------------------------------HTTP------------------------------------------------------*/

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

  if(WiFi.status() == WL_CONNECTED) { //If WiFi connected to hot spot then start mDNS
    if (!MDNS.begin("neneka")) {
      Serial.println("Error setting up MDNS responder!");
      while (1) {
        delay(1000);
      }
    }
  }
  
  server.on("/", handleRoot);  //Associate handler function to path
  server.on("/setLED", handleLED);
  server.on("/readTemp", handleTemp);
  server.on("/readHum", handleHum);
  server.on("/changePalette", handleChangePalette);
  server.begin();
  Serial.println("HTTP server started");

  MDNS.addService("http", "tcp", 80);
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

/*------------------------------------------------------FIM - HTTP------------------------------------------------------*/

/*------------------------------------------------------SENSOR TEMP-----------------------------------------------------*/

void handleTemp() { 
 //readDHTSensor();
 server.send(200, "text/plane", String(t));
}

void handleHum() {
 //readDHTSensor();
 server.send(200, "text/plane", String(h));
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
/*------------------------------------------------------FIM - SENSOR TEMP-------------------------------------------------*/

void initConnectionsLED() {
  ledState = HIGH;
  pinMode(LED_BUILTIN, OUTPUT);   
  digitalWrite(LED_BUILTIN, ledState);
}

/*------------------------------------------------------FastLED------------------------------------------------------*/
void setupLEDStrip() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    fill_solid( currentPalette, 16, CRGB::Black);
}

void showLED() {
  uint8_t colorIndex = 1;
  for( int i = 0; i < NUM_LEDS; i++) {
    if(isPalette) {
      leds[i] = ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS, LINEARBLEND);
      colorIndex += 4;
    } else {
      leds[i] = colorChoosen;
    }    
  }
  FastLED.show();
}

void handleChangePalette() {      
  String choosenPalette = server.arg("palette");
  
  unsigned long currentMillis = 0, previousLEDMillis = 0;
  isPalette = true;
  colorChoosen = 0;

  if(choosenPalette ==  "cloud") {
      currentPalette = CloudColors_p;
  } else if(choosenPalette ==  "lava") {
      currentPalette = LavaColors_p;
  } else if(choosenPalette ==  "ocean") {
      currentPalette = OceanColors_p;
  } else if(choosenPalette ==  "forest") {
      currentPalette = ForestColors_p;
  } else if(choosenPalette ==  "rainbow") {
      currentPalette = RainbowColors_p;
  } else if(choosenPalette ==  "rainbowStriped") {
      currentPalette = RainbowStripeColors_p;
  } else if(choosenPalette ==  "party") {
      currentPalette = PartyColors_p;
  } else if(choosenPalette ==  "heat") {
      currentPalette = HeatColors_p;
  } else if(choosenPalette ==  "blink") {    
    for( int i = 0; i < 200; i++) {
      fill_solid( currentPalette, NUM_LEDS, CRGB::Red);      
      showLED();
      
      delay(700);
      
      fill_solid( currentPalette, NUM_LEDS, CRGB::Black);
      showLED();

      delay(700);
    }
  } else if(choosenPalette ==  "random") {
      for( int i = 0; i < NUM_LEDS; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
      }
  } else if(choosenPalette ==  "shutdown") {
      fill_solid( currentPalette, NUM_LEDS, CRGB::Black);
  } else {
      isPalette = false;
      colorChoosen = (int)strtol(choosenPalette.c_str(), NULL, 16);
  }
  showLED();
  server.send(200, "text/plane", choosenPalette);
}

