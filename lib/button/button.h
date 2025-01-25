#pragma once

#include <Arduino.h>

#define BUTTON_STATUS_BIT_LAST_STATUS 0
#define BUTTON_STATUS_BIT_CURR_STATUS 1
#define BUTTON_STATUS_BIT_LONG_PRESSED 2

static inline void set_bit_to(uint8_t *byte, uint8_t bit, bool value)
{
    if (value)
    {
        *byte |= 1 << bit;
    }
    else
    {
        *byte &= ~(1 << bit);
    }
}

static inline bool get_bit(uint8_t byte, uint8_t bit)
{
    return (byte >> bit) & 1;
}

template <size_t N>
class Button
{
private:
    std::array<uint8_t, N> pins;
    std::array<uint8_t, N> statuses;
    uint16_t _debounce_time;
    uint32_t _long_press_time;
    uint32_t _last_time;

    void (*_on_press)(uint8_t pin, bool long_press);

    bool _readPin(uint8_t pin) const
    {
        return digitalRead(pin) == LOW;
    }

public:
    Button(std::initializer_list<uint8_t> init) : _debounce_time(100), _long_press_time(2000)
    {
        if (init.size() != N)
        {
            return;
        }
        std::copy(init.begin(), init.end(), pins.begin());
        statuses.fill(0);
    }

    void begin()
    {
        for (uint8_t i = 0; i < N; i++)
        {
            auto _pin = pins[i];
            auto status = &statuses[i];
            pinMode(_pin, INPUT_PULLUP);
            set_bit_to(status, BUTTON_STATUS_BIT_LAST_STATUS, _readPin(_pin));
            set_bit_to(status, BUTTON_STATUS_BIT_CURR_STATUS, get_bit(*status, 0));
            set_bit_to(status, BUTTON_STATUS_BIT_LONG_PRESSED, true);
        }
        _last_time = millis();
    }

    void onPress(void (*on_press)(uint8_t pin, bool long_press))
    {
        this->_on_press = on_press;
    }

    void read()
    {
        for (uint8_t i = 0; i < N; i++)
        {
            auto pin = pins[i];
            auto status = &statuses[i];
            bool pinVal = _readPin(pin);
            if (millis() > _last_time + _debounce_time)
            {
                set_bit_to(status, BUTTON_STATUS_BIT_CURR_STATUS, pinVal);
                if (get_bit(*status, BUTTON_STATUS_BIT_CURR_STATUS) != get_bit(*status, BUTTON_STATUS_BIT_LAST_STATUS))
                {
                    // CHANGED
                    set_bit_to(status, BUTTON_STATUS_BIT_LAST_STATUS, get_bit(*status, BUTTON_STATUS_BIT_CURR_STATUS));
                    if (!get_bit(*status, BUTTON_STATUS_BIT_CURR_STATUS))
                    {
                        // RELEASED
                        if (!get_bit(*status, BUTTON_STATUS_BIT_LONG_PRESSED))
                        {
                            if (_on_press)
                            {
                                _on_press(pin, false);
                            }
                        }
                        else
                        {
                            // PRESSED
                            set_bit_to(status, BUTTON_STATUS_BIT_LONG_PRESSED, false);
                        }
                    }
                    else
                    {
                        _last_time = millis();
                    }
                }
                else if (!get_bit(*status, BUTTON_STATUS_BIT_LONG_PRESSED) && get_bit(*status, BUTTON_STATUS_BIT_CURR_STATUS))
                {
                    if (millis() - _last_time > _long_press_time)
                    {
                        // LONG_PRESSED
                        if (_on_press)
                        {
                            _on_press(pin, true);
                            _last_time = millis();
                            set_bit_to(status, BUTTON_STATUS_BIT_LONG_PRESSED, true);
                        }
                    }
                }
            }
        }
    }
};
