//
// Created by Manoel Amaro on 01.02.24.
//

#include "MainMenu.h"
#include <Fonts/FreeSerif9pt7b.h>
#include "images.h"
#include <Adafruit_SSD1306.h>

MainMenu::MainMenu() : UI(MAIN_MENU) {}

void MainMenu::render(Adafruit_SSD1306 *display) {
    display->clearDisplay();
    display->setFont(&FreeSerif9pt7b);
    display->setTextSize(1);
    display->setCursor(22, 13);
    display->print(F("Dice Roller"));
    display->drawFastHLine(0, 15, 128, SSD1306_WHITE);
    display->setFont();
    display->setCursor(40, 24);
    display->print(F("Standard"));
    display->setCursor(40, 38);
    display->print(F("Advanced"));
    display->setCursor(44, 52);
    display->print(F("History"));

    display->drawRoundRect(34, 20 + (_curr * 14), 60, 16, 4, SSD1306_WHITE);

    display->drawBitmap(0, 23, image_allArray[_curr], IMAGE_WIDTH, IMAGE_HEIGHT, WHITE);
    display->drawBitmap(96, 23, image_allArray[5 - _curr], IMAGE_WIDTH, IMAGE_HEIGHT, WHITE);

    display->display();
}

void MainMenu::up(bool longPress) {
    if (_curr > 0) {
        _curr--;
    } else {
        _curr = 2;
    }
}

void MainMenu::down(bool longPress) {
    _curr = (_curr + 1) % 3;
}

void MainMenu::left(bool longPress) {
}

void MainMenu::right(bool longPress) {
}

uint8_t MainMenu::pos() {
    return _curr;
}
