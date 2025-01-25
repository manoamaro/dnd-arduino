//
// Created by Manoel Amaro on 01.03.24.
//

#ifndef DND_DISPLAY_H
#define DND_DISPLAY_H
#include <Arduino.h>
#include <Print.h>
#include "inttypes.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Fonts/FreeSerif9pt7b.h"
#include "Fonts/FreeMono9pt7b.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_COLOR_WHITE 1
#define DISPLAY_COLOR_BLACK 2

class Display: public Print {
public:
    Adafruit_SSD1306 *_display;

    explicit Display(Adafruit_SSD1306 *display);

    void setup();
    void drawRect(int x, int y, int w, int h);
    void drawBitmap(int x, int y, const uint8_t *bitmap, int w, int h, int color);
    void setTextSize(int size);
    void setCursor(int x, int y);
    void display();

    void clearDisplay();

    void setSerifFont();

    size_t write(uint8_t c) override;

    void drawHLine(int x, int y, int w);

    void setStandardFont();

    void drawRoundRect(int x, int y, int w, int h, int r);

    static int width();

    static int height();
};

#endif //DND_DISPLAY_H
