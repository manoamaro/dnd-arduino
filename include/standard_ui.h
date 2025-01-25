//
// Created by Manoel Amaro on 01.02.24.
//

#ifndef DND_STANDARDUI_H
#define DND_STANDARDUI_H

#include "UI.h"
#include "dice.h"

#define MAX_COUNT_PER_DICE 5
#define INIT_DICE_COUNT 0


class StandardUI: public UI {
private:
    uint8_t _curr_die = 0;
    uint8_t _dice_count[DICE_COUNT] = {INIT_DICE_COUNT};
    uint8_t _dice_results[DICE_COUNT][MAX_COUNT_PER_DICE]{};
    uint8_t _sum = 0;
    bool _show_rolls = false;
public:
    explicit StandardUI();
    void render(Display *display) override;
    void up(bool longPress) override;
    void down(bool longPress) override;
    void left(bool longPress) override;
    void right(bool longPress) override;
};


#endif //DND_STANDARDUI_H
