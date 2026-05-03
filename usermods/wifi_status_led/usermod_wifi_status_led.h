#pragma once
#include "wled.h"

class WifiStatusLedUsermod : public Usermod {
private:
  static const uint8_t  LED_PIN           = 48;
  static const uint16_t CONNECTING_BLINK_MS = 500;
  static const uint16_t UPDATE_INTERVAL_MS  = 2000;
  
  static const uint8_t  TOUCH_PIN         = 1;
  static const uint16_t MY_TOUCH_THRESHOLD   = 40000; // aanpassen indien nodig
  static const uint16_t TOUCH_DEBOUNCE_MS = 500;


  uint32_t lastUpdate = 0;
  uint32_t lastBlink  = 0;
  bool     blinkState = false;
  uint32_t lastTouch  = 0;
  bool     wifiEnabled = true;

  void setLedColor(uint8_t r, uint8_t g, uint8_t b) {
    neopixelWrite(LED_PIN, r, g, b);
  }

  void checkTouch() {
    uint32_t now = millis();
    if (now - lastTouch < TOUCH_DEBOUNCE_MS) return;
    
    uint32_t touchValue = touchRead(TOUCH_PIN);
    if (touchValue < MY_TOUCH_THRESHOLD) {
      lastTouch = now;
      wifiEnabled = !wifiEnabled;
      if (wifiEnabled) {
        WiFi.begin();
      } else {
        WiFi.disconnect(true);
      }
    }
  }


public:
  void setup() override {
    // LED pin initialiseren
    pinMode(LED_PIN, OUTPUT);
  }

  void loop() override {
    checkTouch();
    uint32_t now = millis();

    if (WiFi.status() != WL_CONNECTED && !apActive) {
      if (now - lastBlink > CONNECTING_BLINK_MS) {
        lastBlink  = now;
        blinkState = !blinkState;
        if (blinkState) setLedColor(50, 30, 0);  // geel
        else setLedColor(0, 0, 0);
      }
      return;
    }

    if (now - lastUpdate < UPDATE_INTERVAL_MS) return;
    lastUpdate = now;

    if (WiFi.status() == WL_CONNECTED) {
      setLedColor(0, 20, 0);  // groen
    } else if (apActive) {
      setLedColor( 20, 0, 0);  // rood
    }
  }

  uint16_t getId() override { return USERMOD_ID_UNSPECIFIED; }
};

static WifiStatusLedUsermod wifi_status_led_mod;
REGISTER_USERMOD(wifi_status_led_mod);
