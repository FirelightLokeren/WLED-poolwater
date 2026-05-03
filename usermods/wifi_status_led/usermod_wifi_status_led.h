#pragma once
#include "wled.h"

class WifiStatusLedUsermod : public Usermod {
private:
  static const uint8_t  STATUS_SEG          = 1;
  static const uint16_t CONNECTING_BLINK_MS = 500;
  static const uint16_t UPDATE_INTERVAL_MS  = 2000;

  uint32_t lastUpdate = 0;
  uint32_t lastBlink  = 0;
  bool     blinkState = false;

  void setSegmentColor(uint32_t color) {
    if (STATUS_SEG >= strip.getSegmentsNum()) return;
    Segment& seg = strip.getSegment(STATUS_SEG);
    seg.setColor(0, color);
    seg.setOption(SEG_OPTION_ON, true);
    seg.mode       = FX_MODE_STATIC;
    stateChanged   = true;
    colorUpdated(CALL_MODE_DIRECT_CHANGE);
  }

public:
  void setup() override {}

  void loop() override {
    uint32_t now = millis();

    if (WiFi.status() != WL_CONNECTED && !apActive) {
      if (now - lastBlink > CONNECTING_BLINK_MS) {
        lastBlink  = now;
        blinkState = !blinkState;
        setSegmentColor(blinkState ? 0xFFAA00 : 0x000000);
      }
      return;
    }

    if (now - lastUpdate < UPDATE_INTERVAL_MS) return;
    lastUpdate = now;

    if (WiFi.status() == WL_CONNECTED) {
      setSegmentColor(0x002200); // groen = verbonden
    } else if (apActive) {
      setSegmentColor(0x220000); // rood = AP-modus
    }
  }

  uint16_t getId() override { return USERMOD_ID_UNSPECIFIED; }
};

#ifdef USERMOD_WIFI_STATUS_LED
static WifiStatusLedUsermod wifi_status_led_mod;
REGISTER_USERMOD(wifi_status_led_mod);
#endif
