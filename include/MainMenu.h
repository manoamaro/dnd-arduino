//
// Created by Manoel Amaro on 01.02.24.
//

#ifndef DND_MAINMENU_H
#define DND_MAINMENU_H

#include "Adafruit_SSD1306.h"
#include "UI.h"

class MainMenu: public UI {
private:
    uint8_t _curr = 0;
public:
    explicit MainMenu();
    void render(Adafruit_SSD1306 *display) override;
    void up(bool longPress) override;
    void down(bool longPress) override;
    void left(bool longPress) override;
    void right(bool longPress) override;
    uint8_t pos() override;
};

#endif //DND_MAINMENU_H
