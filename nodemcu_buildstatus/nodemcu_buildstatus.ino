#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ESP8266_D1_PIN_ORDER
#include <ESP8266WiFi.h>
#include "FastLED.h"

const char* ssid     = "AltGuest";
const char* password = "Welcome@Altran";
const char* host = "www.kfk4ever.com";
const char* url = "http://www.kfk4ever.com/~griffon26/buildlightstatus.txt";

///////////////////////////////////////////////////////////////////////////////////////////
//
// Move a white dot along the strip of leds.  This program simply shows how to configure the leds,
// and then how to turn a single pixel white and then off, moving down the line of pixels.
//

// How many leds are in the strip?
#define NUM_LEDS 12

// Data pin that led data will be written out over
#define DATA_PIN D1

// Clock pin only needed for SPI based chipsets when not using hardware SPI
//#define CLOCK_PIN 8

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

char buildStatus;

// This function sets up the ledsand tells the controller about them
void setup() {
  delay(2000);
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(115200);
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  setBlue();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  setOrange();
}

// This function runs over and over, and is where you do the magic to light
// your leds.
void loop() {
  delay(5000);
  getBuildStatus(buildStatus);
  Serial.println("Status is: " + String(buildStatus));

  if (buildStatus == 'G') {
    Serial.println("Green");
    setGreen();
  }
  else if(buildStatus == 'F') {
    Serial.println("Error");
    setPurple();
  }
  else {
    Serial.println("Red");
    setRed();
  }
}

void getBuildStatus(char &result) {
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    result = 'F';
    return;
  }

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      result = 'F';
      return;
    }
  }

  String data = "";
  while (client.available()) {
    data = client.readStringUntil('\r');
    result = data.charAt(1);
  }
}


void setBlue() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;
  }
  FastLED.show();
}

void setGreen() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Green;
  }
  FastLED.show();
}

void setRed() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
  }
  FastLED.show();
}

void setOrange() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Orange;
  }
  FastLED.show();
}

void setPurple() {
    for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(100, 0, 100);
  }
  FastLED.show();
}
