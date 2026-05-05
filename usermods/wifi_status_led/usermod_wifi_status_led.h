#pragma once
#include "wled.h"

static volatile bool _touchDetected = false;

void IRAM_ATTR _touchISR() {
  _touchDetected = true;
}

class WifiStatusLedUsermod : public Usermod {
private:
  static const uint8_t  LED_PIN            = 48;
  static const uint8_t  TOUCH_PIN          = 1;
  static const uint16_t MY_TOUCH_THRESHOLD = 80000;
  static const uint16_t TOUCH_DEBOUNCE_MS  = 1000;

  uint32_t lastTouch = 0;

public:
  void setup() override {
    pinMode(LED_PIN, OUTPUT);
    touchAttachInterrupt(TOUCH_PIN, _touchISR, MY_TOUCH_THRESHOLD);
  }

  void loop() override {
    if (millis() < 5000) return;

    uint32_t now = millis();
    if (_touchDetected && (now - lastTouch > TOUCH_DEBOUNCE_MS)) {
      _touchDetected = false;
      lastTouch = now;
      if (strip.getSegmentsNum() > 0) {
        Segment& seg = strip.getSegment(0);
        seg.setOption(SEG_OPTION_ON, !seg.getOption(SEG_OPTION_ON));
        stateChanged = true;
        colorUpdated(CALL_MODE_DIRECT_CHANGE);
      }
    }
  }

  uint16_t getId() override { return USERMOD_ID_UNSPECIFIED; }
};

static WifiStatusLedUsermod wifi_status_led_mod;
REGISTER_USERMOD(wifi_status_led_mod);
