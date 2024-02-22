//
// Created by Manoel Amaro on 01.02.24.
//

#include "MainMenu.h"
#include "images.h"

MainMenu::MainMenu() : UI(MAIN_MENU) {}

void MainMenu::render(DisplaySSD1306_128x64_I2C *display) {
    display->clear();
    //display->setFont(&FreeSerif9pt7b);
    display->printFixed(22, 13, "Dice Roller", STYLE_NORMAL);
    display->drawHLine(0, 15, 128);
    display->printFixed(40, 24, "Standard", STYLE_NORMAL);
    display->printFixed(40, 38, "Advanced", STYLE_NORMAL);
    display->printFixed(44, 52, "History", STYLE_NORMAL);

    display->drawRect(34, 20 + (_curr * 14), 60, 16);

    display->drawBitmap1(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, image_allArray[0]);
    display->drawBitmap1(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, image_allArray[5-_curr]);
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
