//
// Created by Manoel Amaro on 01.02.24.
//

#include "StandardUI.h"
#include "images.h"
#include "TrueRandom.h"

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

void StandardUI::render(DisplaySSD1306_128x64_I2C *display) {
    if (!_show_rolls) {
        display->clear();
        display->drawBitmap1(-4, 20, ICON_WIDTH, ICON_HEIGHT, icon_allArray[rotateNumber(_curr_die - 2, DICE_COUNT)]);
        display->drawBitmap1(22, 10, ICON_WIDTH, ICON_HEIGHT, icon_allArray[rotateNumber(_curr_die - 1, DICE_COUNT)]);
        display->drawBitmap1(48, 5, ICON_WIDTH, ICON_HEIGHT, image_allArray[_curr_die]);
        display->drawBitmap1(86, 10, ICON_WIDTH, ICON_HEIGHT, icon_allArray[rotateNumber(_curr_die + 1, DICE_COUNT)]);
        display->drawBitmap1(112, 20, ICON_WIDTH, ICON_HEIGHT, icon_allArray[rotateNumber(_curr_die + 2, DICE_COUNT)]);
        display->setTextCursor(40, 45);
        display->print(_dice_count[_curr_die]);
        utils::print(display, F("d"));
        display->print(Dice::sideOf(_curr_die));
        display->drawRect(0, 0, 128, 64);
    } else {
        display->clear();
        for (uint8_t i = 0; i < DICE_COUNT; i++) {
            display->setTextCursor(0, 4 + i * 10);
            utils::print(display, F("d"));
            display->print(Dice::sideOf(i));
            for (uint8_t j = 0; j < MAX_COUNT_PER_DICE; j++) {
                display->setTextCursor(32 + j * 20, 4 + i * 10);
                if (_dice_results[i][j] > 0) {
                    display->print(_dice_results[i][j]);
                } else {
                    utils::print(display, F("*"));
                }
            }
        }
        display->drawRect(20, 0, display->width() - 20, display->height());
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
                this->_dice_results[i][j] = Dice::roll(1, Dice::sideOf(i), TrueRandom.random());
                this->_sum += this->_dice_results[i][j];
            }
        }
        this->_show_rolls = true;
    }
}
