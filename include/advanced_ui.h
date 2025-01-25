//
// Created by Manoel Amaro on 01.02.24.
//

#ifndef DND_ADVANCEDUI_H
#define DND_ADVANCEDUI_H

#include "expression_tree.h"
#include "op_result.h"
#include "op.h"
#include "const_op.h"
#include "math_op.h"
#include "roll_op.h"
#include "UI.h"
#include <memory>

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
    ExpressionTree *expressionTree;
    OpResult result;
    std::shared_ptr<ConstOp> constOp;
    std::shared_ptr<RollOp> rollOp;
    std::shared_ptr<MathOp> mathOp;

    void printOp(Op *op, Display *display, uint8_t depth);

public:
    explicit AdvancedUI();
    ~AdvancedUI() override;

    void render(Display *display) override;
    void up(bool longPress) override;
    void down(bool longPress) override;
    void left(bool longPress) override;
    void right(bool longPress) override;
};

#endif // DND_ADVANCEDUI_H
