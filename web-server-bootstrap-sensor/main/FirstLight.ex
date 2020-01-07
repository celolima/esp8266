#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ESP8266_D1_PIN_ORDER

#include <FastLED.h>

#define NUM_LEDS 120
#define DATA_PIN 5
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

void setup() {
    Serial.begin(115200);
    Serial.println(" --- Inicializando a app FirstLight --- ");
    delay(2000);    
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
}

void loop() {
    for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed++) {
        //Serial.println(whiteLed);
        if(whiteLed % 2 == 0) {
          leds[whiteLed] = CRGB::Red;  
        } else {
          leds[whiteLed] = CRGB::Yellow;  
        }
        
        FastLED.show();
        delay(20);
        leds[whiteLed] = CRGB::Black;        
   }
}