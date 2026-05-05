#pragma once
#include "wled.h"

class WifiStatusLedUsermod : public Usermod {
private:
  static const uint8_t  LED_PIN             = 48;
  static const uint16_t CONNECTING_BLINK_MS = 500;
  static const uint16_t UPDATE_INTERVAL_MS  = 2000;
  static const uint8_t  TOUCH_PIN           = 1;
  static const uint16_t MY_TOUCH_THRESHOLD  = 40000;
  static const uint16_t TOUCH_DEBOUNCE_MS   = 500;

  uint32_t lastUpdate  = 0;
  uint32_t lastBlink   = 0;
  uint32_t lastTouch   = 0;
  bool     blinkState  = false;
  bool     wifiEnabled = true;

  void setLedColor(uint8_t r, uint8_t g, uint8_t b) {
    neopixelWrite(LED_PIN, r, g, b);
  }

  void checkTouch() {
    uint32_t now = millis();
    if (now - lastTouch < TOUCH_DEBOUNCE_MS) return;
    uint32_t touchValue = touchRead(TOUCH_PIN);
    if (touchValue < MY_TOUCH_THRESHOLD) {
      lastTouch   = now;
      wifiEnabled = !wifiEnabled;
      if (wifiEnabled) {
        ESP.restart();
      } else {
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
      }
    }
  }

public:
  void setup() override {
    pinMode(LED_PIN, OUTPUT);
  }

  void loop() override {
    if (millis() < 5000) return;
    checkTouch();

    uint32_t now = millis();

    if (!wifiEnabled) {
      if (now - lastBlink > CONNECTING_BLINK_MS) {
        lastBlink  = now;
        blinkState = !blinkState;
      }
      return;
    }

    if (WiFi.status() != WL_CONNECTED && !apActive) {
      if (now - lastBlink > CONNECTING_BLINK_MS) {
        lastBlink  = now;
        blinkState = !blinkState;
      }
      return;
    }

    if (now - lastUpdate < UPDATE_INTERVAL_MS) return;
    lastUpdate = now;
  }

  uint16_t getId() override { return USERMOD_ID_UNSPECIFIED; }
};

static WifiStatusLedUsermod wifi_status_led_mod;
REGISTER_USERMO
