//
// Created by Manoel Amaro on 01.02.24.
//

#include "main_menu.h"
#include "images.h"
#include "Fonts/FreeSerif9pt7b.h"
#include "Adafruit_SSD1306.h"

MainMenu::MainMenu() : UI(MAIN_MENU) {
}

MainMenu::~MainMenu()
{
    //delete this->menu;
}

void MainMenu::render(Display *display)
{
    display->clearDisplay();
    display->setSerifFont();
    display->setTextSize(1);
    display->setCursor(22, 13);
    display->print(F("Dice Roller"));
    display->drawHLine(0, 15, 128);
    display->setStandardFont();
    display->setCursor(40, 24);
    display->print(F("Standard"));
    display->setCursor(40, 38);
    display->print(F("Advanced"));
    display->setCursor(44, 52);
    display->print(F("History"));

    display->drawRoundRect(34, 20 + (_curr * 14), 60, 16, 4);

    display->drawBitmap(0, 23, image_allArray[_curr], IMAGE_WIDTH, IMAGE_HEIGHT, WHITE);
    display->drawBitmap(96, 23, image_allArray[5 - _curr], IMAGE_WIDTH, IMAGE_HEIGHT, WHITE);
    display->display();
}

void MainMenu::up(bool longPress)
{
    if (_curr > 0) {
        _curr--;
    } else {
        _curr = 2;
    }
}

void MainMenu::down(bool longPress)
{
    _curr = (_curr + 1) % 3;
}

void MainMenu::left(bool longPress)
{
}

void MainMenu::right(bool longPress)
{
}

uint8_t MainMenu::pos()
{
    return _curr;
}
