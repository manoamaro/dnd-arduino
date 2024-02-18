//
// Created by Manoel Amaro on 03.02.24.
//

#ifndef DND_UI_H
#define DND_UI_H

#include <inttypes.h>
#include <Adafruit_SSD1306.h>

enum UI_TYPE {
    MAIN_MENU,
    STANDARD,
    ADVANCED,
    HISTORY,
};

class UI {
public:
    UI_TYPE type;

    explicit UI(UI_TYPE type);

    virtual void render(Adafruit_SSD1306 *display) = 0;
    virtual void up(bool longPress) = 0;
    virtual void down(bool longPress) = 0;
    virtual void left(bool longPress) = 0;
    virtual void right(bool longPress) = 0;

    virtual uint8_t pos();

    virtual ~UI();
};


#endif //DND_UI_H