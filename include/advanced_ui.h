//
// Created by Manoel Amaro on 01.02.24.
//

#ifndef DND_ADVANCEDUI_H
#define DND_ADVANCEDUI_H

#include "expression_tree.h"
#include "op_result.h"
#include "op2.h"
#include "UI.h"

enum UiState
{
    UI_CONST_DICE_STATE,     // Select between constant value or sequence generator
    UI_INPUT_CONST_STATE,    // x
    UI_INPUT_DICE_X_STATE,   // xdy
    UI_INPUT_DICE_Y_STATE,   // xdy
    UI_INPUT_DICE_MIN_STATE, // [minX]
    UI_INPUT_DICE_KH_STATE,  // [khX]
    UI_INPUT_DICE_RO_STATE,  // [roX]
    UI_INPUT_MATH_STATE,     // [+,-]
    UI_SHOW_RESULT_STATE
};

enum UIOpType
{
    UI_OP_CONST,
    UI_OP_DICE,
};

class AdvancedUI : public UI
{
private:
    uint8_t state = UI_CONST_DICE_STATE;
    uint8_t uiOpType = UI_OP_CONST;
    ExpressionTree expressionTree;
    op::Op2 *currOp = nullptr;
    OpResult result;

    void printOp(op::Op2 *op, DisplaySSD1306_128x64_I2C *display, uint8_t depth);
    void printDetails(OpResultDetail *detail, DisplaySSD1306_128x64_I2C *display);

public:
    explicit AdvancedUI();
    ~AdvancedUI() override;

    void render(DisplaySSD1306_128x64_I2C *display) override;
    void up(bool longPress) override;
    void down(bool longPress) override;
    void left(bool longPress) override;
    void right(bool longPress) override;
};

#endif // DND_ADVANCEDUI_H
