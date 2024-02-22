//
// Created by Manoel Amaro on 03.02.24.
//

#ifndef DND_UI_H
#define DND_UI_H

#include "lcdgfx.h"
#include <inttypes.h>


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

    virtual void render(DisplaySSD1306_128x64_I2C *display) = 0;
    virtual void up(bool longPress) = 0;
    virtual void down(bool longPress) = 0;
    virtual void left(bool longPress) = 0;
    virtual void right(bool longPress) = 0;

    virtual uint8_t pos();

    virtual ~UI();
};


#endif //DND_UI_H
