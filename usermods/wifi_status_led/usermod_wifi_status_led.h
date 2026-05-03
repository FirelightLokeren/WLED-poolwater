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
    seg.setOpti
