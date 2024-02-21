//
// Created by Manoel Amaro on 01.02.24.
//

#ifndef DND_ADVANCEDUI_H
#define DND_ADVANCEDUI_H

#include "Adafruit_SSD1306.h"
#include "op_result.h"
#include "const_op.h"
#include "roll_op.h"
#include "math_op.h"
#include "expression_tree.h"
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
    UiState state = UI_CONST_DICE_STATE;
    UIOpType uiOpType = UI_OP_CONST;
    ExpressionTree expressionTree;
    ConstOp *currConstOp = nullptr;
    RollOp *currRollOp = nullptr;
    MathOp *currMathOp = nullptr;
    OpResult result;

    void printOp(Op *op, Adafruit_SSD1306 *display, uint8_t depth);
    void printDetails(OpResultDetail *detail, Adafruit_SSD1306 *display);

public:
    explicit AdvancedUI();
    ~AdvancedUI() override;

    void render(Adafruit_SSD1306 *display) override;
    void up(bool longPress) override;
    void down(bool longPress) override;
    void left(bool longPress) override;
    void right(bool longPress) override;
};

#endif // DND_ADVANCEDUI_H
