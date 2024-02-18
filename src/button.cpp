#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>

class Button {
  uint8_t _pin;
  uint16_t _debounce_time;
  uint32_t _long_press_time;
  uint32_t _last_time;
  bool _last_status;
  bool _curr_status;
  bool _long_pressed;
  void (*_on_press)();
  void (*_on_long_press)();

  bool _readPin() const {
    return digitalRead(_pin) == LOW;
  }

public:
  Button(uint8_t pin, uint16_t debounce = 100, uint32_t long_press_time = 2000) {
    _pin = pin;
    _debounce_time = debounce;
    _long_press_time = long_press_time;
  }

  void begin() {
    pinMode(_pin, INPUT_PULLUP);
    _last_time = millis();
    _last_status = _readPin();
    _curr_status = _last_status;
    _long_pressed = false;
  }

  void onPress(void (*on_press)()) {
    this->_on_press = on_press;
  }

  void onLongPress(void (*on_long_press)()) {
    this->_on_long_press = on_long_press;
  }

  void read() {
    bool pinVal = _readPin();
    if (millis() > _last_time + _debounce_time) {
      _curr_status = pinVal;
      if (_curr_status != _last_status) {
        // CHANGED
        _last_status = _curr_status;
        if (!_curr_status) {
          // RELEASED
          if (!_long_pressed) {
            if (_on_press) {
              _on_press();
            }
          } else {
            // PRESSED
            _long_pressed = false;
          }
        } else {
          _last_time = millis();
        }
      } else if (!_long_pressed && _curr_status) {
        if (millis() - _last_time > _long_press_time) {
        // LONG_PRESSED
          if (_on_long_press) {
            _on_long_press();
            _last_time = millis();
            _long_pressed = true;
          }
        }
      }
    }
  }
};

#endif //BUTTON_H