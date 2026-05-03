#pragma once
#include "wled.h"

class WifiStatusLedUsermod : public Usermod {
private:
  static const uint8_t  LED_PIN           = 48;
  static const uint16_t CONNECTING_BLINK_MS = 500;
  static const uint16_t UPDATE_INTERVAL_MS  = 2000;

  uint32_t lastUpdate = 0;
  uint32_t lastBlink  = 0;
  bool     blinkState = false;

  void setLedColor(uint8_t r, uint8_t g, uint8_t b) {
    neopixelWrite(LED_PIN, r, g, b);
  }

public:
  void setup() override {
    // LED pin initialiseren
    pinMode(LED_PIN, OUTPUT);
  }

  void loop() override {
    uint32_t now = millis();

    if (WiFi.status() != WL_CONNECTED && !apActive) {
      if (now - lastBlink > CONNECTING_BLINK_MS) {
        lastBlink  = now;
        blinkState = !blinkState;
        if (blinkState) setLedColor(5, 3, 0);  // geel
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
