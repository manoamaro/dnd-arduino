//
// Created by Manoel Amaro on 01.02.24.
//

#include <Arduino.h>
#include "standard_ui.h"
#include "images.h"

static int rotateNumber(int input, int max) {
    if (input >= 0 && input < max) {
        return input;
    } else if (input < 0) {
        return max + input; // Rotates the negative number within the range
    } else {
        return input % max; // Rotates the number back into the range for inputs >= max
    }
}

StandardUI::StandardUI() : UI(STANDARD) {
}

void StandardUI::render(Display *display) {
    if (!_show_rolls) {
        display->clearDisplay();
        display->drawBitmap(-4, 20, icon_allArray[rotateNumber(_curr_die - 2, DICE_COUNT)], ICON_WIDTH, ICON_HEIGHT, DISPLAY_COLOR_WHITE);
        display->drawBitmap(22, 10, icon_allArray[rotateNumber(_curr_die - 1, DICE_COUNT)], ICON_WIDTH, ICON_HEIGHT, DISPLAY_COLOR_WHITE);
        display->drawBitmap(48, 5, image_allArray[_curr_die], IMAGE_WIDTH, IMAGE_HEIGHT, DISPLAY_COLOR_WHITE);
        display->drawBitmap(86, 10, icon_allArray[rotateNumber(_curr_die + 1, DICE_COUNT)], ICON_WIDTH, ICON_HEIGHT, DISPLAY_COLOR_WHITE);
        display->drawBitmap(112, 20, icon_allArray[rotateNumber(_curr_die + 2, DICE_COUNT)], ICON_WIDTH, ICON_HEIGHT, DISPLAY_COLOR_WHITE);
        display->setTextSize(2);
        //display->setTextColor(WHITE);
        display->setCursor(40, 45);
        display->print(_dice_count[_curr_die]);
        display->print(F("d"));
        display->print(Dice::sideOf(_curr_die));
        display->drawRoundRect(0, 0, 128, 64, 4);
        display->display();
    } else {
        display->clearDisplay();
        display->setTextSize(1);
        for (uint8_t i = 0; i < DICE_COUNT; i++) {
            display->setCursor(0, 4 + i * 10);
            display->print(F("d"));
            display->print(Dice::sideOf(i));
            for (uint8_t j = 0; j < MAX_COUNT_PER_DICE; j++) {
                display->setCursor(32 + j * 20, 4 + i * 10);
                if (_dice_results[i][j] > 0) {
                    display->print(_dice_results[i][j]);
                } else {
                    display->print(F("*"));
                }
            }
        }
        display->drawRoundRect(20, 0, display->width() - 20, display->height(), 4);
        display->display();
    }
}

void StandardUI::up(bool longPress) {
    this->_dice_count[this->_curr_die] = min(this->_dice_count[this->_curr_die] + 1, MAX_COUNT_PER_DICE);
    this->_show_rolls = false;
}

void StandardUI::down(bool longPress) {
    this->_dice_count[this->_curr_die] = max(this->_dice_count[this->_curr_die] - 1, 0);
    this->_show_rolls = false;
}

void StandardUI::left(bool longPress) {
    if (longPress) {
        this->_curr_die = 0;
        for (int i = 0; i < MAX_COUNT_PER_DICE; i++) {
            this->_dice_count[i] = INIT_DICE_COUNT;
        }
        this->_show_rolls = false;
    } else {
        this->_curr_die = (this->_curr_die + 1) % DICE_COUNT;
        this->_show_rolls = false;
    }
}

void StandardUI::right(bool longPress) {
    if (longPress) {

    } else {
        this->_sum = 0;
        for (uint8_t i = 0; i < DICE_COUNT; i++) {
            for (uint8_t j = 0; j < MAX_COUNT_PER_DICE; j++) {
                this->_dice_results[i][j] = 0;
            }
        }

        for (uint8_t i = 0; i < DICE_COUNT; i++) {
            for (uint8_t j = 0; j < this->_dice_count[i]; j++) {
                this->_dice_results[i][j] = Dice::roll(1, Dice::sideOf(i), random());
                this->_sum += this->_dice_results[i][j];
            }
        }
        this->_show_rolls = true;
    }
}
