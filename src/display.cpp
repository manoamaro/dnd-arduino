//
// Created by Manoel Amaro on 01.03.24.
//

#include "display.h"

Display::Display(Adafruit_SSD1306 *display) {
    _display = display;
}

void Display::setup() {
    if (!_display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    Serial.println(F("Display initialized"));
    _display->clearDisplay();
    _display->setTextSize(1);
    _display->setTextColor(DISPLAY_COLOR_WHITE);
    _display->display();
}

void Display::drawRect(int x, int y, int w, int h) {
    _display->drawRect(x, y, w, h, DISPLAY_COLOR_WHITE);
}

void Display::drawBitmap(int x, int y, const uint8_t *bitmap, int w, int h, int color) {
    _display->drawBitmap(x, y, bitmap, w, h, color);
}

void Display::setTextSize(int size) {
    _display->setTextSize(size);
}

size_t Display::write(uint8_t c) {
    return _display->write(c);
}

void Display::setCursor(int x, int y) {
    _display->setCursor(x, y);
}

void Display::display() {
    _display->display();
}

void Display::clearDisplay() {
    _display->clearDisplay();
}

void Display::setSerifFont() {
    _display->setFont(&FreeSerif9pt7b);
}

void Display::drawHLine(int x, int y, int w) {
    _display->drawFastHLine(x, y, w, DISPLAY_COLOR_WHITE);
}

void Display::setStandardFont() {
    _display->setFont();
}

void Display::drawRoundRect(int x, int y, int w, int h, int r) {
    _display->drawRoundRect(x, y, w, h, r, DISPLAY_COLOR_WHITE);
}

int Display::width() {
    return DISPLAY_WIDTH;
}

int Display::height() {
    return DISPLAY_HEIGHT;
}
