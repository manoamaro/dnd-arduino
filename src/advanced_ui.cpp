//
// Created by Manoel Amaro on 01.02.24.
//

#include "advanced_ui.h"
#include "dice.h"
#include "print.h"
#include "op.h"
#include "op_result.h"
#include "math_op.h"
#include "const_op.h"
#include "roll_op.h"

#define CENTER_TEXT(len) ((128 - ((len) * 6)) / 2)
#define CENTER_2X_TEXT(len) ((128 - ((len) * 12)) / 2)
#define UI_INPUT_Y 56

AdvancedUI::AdvancedUI() : UI(ADVANCED)
{
    this->state = UI_CONST_DICE_STATE;
    this->uiOpType = UI_OP_CONST;
    this->expressionTree = new ExpressionTree();
}

AdvancedUI::~AdvancedUI()
{
    delete this->expressionTree;
}

void AdvancedUI::printOp(Op *op, Display *display, uint8_t depth = 0)
{
    if (!op)
    {
        return;
    }

    if (op->type == OP_MATH)
    {
        printOp(((MathOp*)op)->left.get(), display, depth + 1);
    }
    if (op->type == OP_CONST)
    {
        display->print(((ConstOp*)op)->value);
    }
    else if (op->type == OP_MATH)
    {
        auto mathOp = (MathOp *)op;
        if (mathOp->left)
        {
            display->print(F(" "));
            if (mathOp->exp == MATH_EXP_PLUS)
            {
                display->print(F("+"));
            }
            else if (mathOp->exp == MATH_EXP_MINUS)
            {
                display->print(F("-"));
            }
            display->print(F(" "));
        }
    }
    else if (op->type == OP_DICE)
    {
        auto rollOp = (RollOp *)op;
        display->print(rollOp->amount);
        display->print(F("d"));
        display->print(rollOp->sides());
        if (rollOp->minimum > 1)
        {
            display->print(F("m"));
            display->print(rollOp->minimum);
        }
        if (rollOp->keepHighest > 0)
        {
            display->print(F("kh"));
            display->print(rollOp->keepHighest);
        }
        if (rollOp->reRollLowerThan > 0)
        {
            display->print(F("ro"));
            display->print(rollOp->reRollLowerThan);
        }
    }
    if (op->type == OP_MATH)
    {
        printOp(((MathOp*)op)->right.get(), display, depth + 1);
    }
}

void AdvancedUI::render(Display *display)
{
    display->clearDisplay();
    if (state == UI_SHOW_RESULT_STATE)
    {
        display->setCursor(CENTER_2X_TEXT(7), 0);
        display->print(F("Results"));
    }
    else
    {
        display->setTextSize(2);
        display->setCursor(CENTER_2X_TEXT(8), 0);
        display->print(F("Advanced"));
    }
    display->drawHLine(0, 18, 128);
    display->setTextSize(1);

    if (state == UI_SHOW_RESULT_STATE)
    {
    }
    else
    {
        display->setCursor(0, 24);
        this->printOp(this->expressionTree->root.get(), display);
    }

    if (state == UI_CONST_DICE_STATE)
    {
        if (uiOpType == UI_OP_CONST)
        {
            display->setCursor(CENTER_TEXT(3), UI_INPUT_Y);
            display->print(F("[X]"));
        }
        else if (uiOpType == UI_OP_DICE)
        {
            display->setCursor(CENTER_TEXT(5), UI_INPUT_Y);
            display->print(F("[XdY]"));
        }
    }
    else if (state == UI_INPUT_CONST_STATE)
    {
        display->setCursor(CENTER_TEXT(2), UI_INPUT_Y);
        display->print(this->constOp->value);
    }
    else if (state == UI_INPUT_DICE_X_STATE)
    {
        display->setCursor(CENTER_TEXT(5), UI_INPUT_Y);
        display->print(F("["));
        display->print(this->rollOp->amount);
        display->print(F("]"));
        display->print(F("d"));
        display->print(this->rollOp->sides());
    }
    else if (state == UI_INPUT_DICE_Y_STATE)
    {
        display->setCursor(CENTER_TEXT(5), UI_INPUT_Y);
        display->print(this->rollOp->amount);
        display->print(F("d"));
        display->print(F("["));
        display->print(this->rollOp->sides());
        display->print(F("]"));
    }
    else if (state == UI_INPUT_DICE_MIN_STATE)
    {
        display->setCursor(CENTER_TEXT(14), UI_INPUT_Y);
        display->print(F("["));
        display->print(F("minimum of "));
        display->print(this->rollOp->minimum);
        display->print(F("]"));
    }
    else if (state == UI_INPUT_DICE_KH_STATE)
    {
        display->setCursor(CENTER_TEXT(16), UI_INPUT_Y);
        display->print(F("["));
        display->print(F("keep highest "));
        display->print(this->rollOp->keepHighest);
        display->print(F("]"));
    }
    else if (state == UI_INPUT_DICE_RO_STATE)
    {
        display->setCursor(CENTER_TEXT(15), UI_INPUT_Y);
        display->print(F("["));
        display->print(F("re-roll lte "));
        display->print(this->rollOp->reRollLowerThan);
        display->print(F("]"));
    }
    else if (state == UI_INPUT_MATH_STATE)
    {
        display->setCursor(CENTER_TEXT(1), UI_INPUT_Y);
        if (this->mathOp->exp == MATH_EXP_PLUS)
        {
            display->print(F("+"));
        }
        else if (this->mathOp->exp == MATH_EXP_MINUS)
        {
            display->print(F("-"));
        }
    }
    else if (state == UI_SHOW_RESULT_STATE)
    {
        display->setCursor(CENTER_2X_TEXT(3), 37);
        display->print(this->result.result);
    }

    display->display();
}

void AdvancedUI::up(bool isLong)
{
    if (this->state == UI_CONST_DICE_STATE)
    {
        uiOpType = (UIOpType)_constrain(uiOpType + 1, 0, 1);
    }
    else if (this->state == UI_INPUT_CONST_STATE)
    {
        this->constOp->value = _constrain(this->constOp->value+ 1, 1, 25);
    }
    else if (this->state == UI_INPUT_DICE_X_STATE)
    {
        this->rollOp->amount = _constrain(this->rollOp->amount + 1, 1, 10);
    }
    else if (this->state == UI_INPUT_DICE_Y_STATE)
    {
        this->rollOp->incSides();
    }
    else if (this->state == UI_INPUT_DICE_MIN_STATE)
    {
        this->rollOp->minimum = _constrain(this->rollOp->minimum + 1, 1, this->rollOp->sides());
    }
    else if (this->state == UI_INPUT_DICE_KH_STATE)
    {
        this->rollOp->keepHighest = _constrain(this->rollOp->keepHighest + 1, 0, this->rollOp->amount);
    }
    else if (this->state == UI_INPUT_DICE_RO_STATE)
    {
        this->rollOp->reRollLowerThan = _constrain(this->rollOp->reRollLowerThan + 1, 0, this->rollOp->sides());
    }
    else if (this->state == UI_INPUT_MATH_STATE)
    {
        this->mathOp->exp = (this->mathOp->exp + 1) % 2;
    }
}

void AdvancedUI::down(bool longPress)
{
    if (this->state == UI_CONST_DICE_STATE)
    {
        uiOpType = (UIOpType)_constrain(uiOpType - 1, 0, 1);
    }
    else if (this->state == UI_INPUT_CONST_STATE)
    {
        this->constOp->value = _constrain(this->constOp->value - 1, 1, 25);
    }
    else if (this->state == UI_INPUT_DICE_X_STATE)
    {
        this->rollOp->amount = _constrain(this->rollOp->amount - 1, 1, 10);
    }
    else if (this->state == UI_INPUT_DICE_Y_STATE)
    {
        // TODO: add a way to change the dice sides
        this->rollOp->decSides();
    }
    else if (this->state == UI_INPUT_DICE_MIN_STATE)
    {
        this->rollOp->minimum = _constrain(this->rollOp->minimum - 1, 1, this->rollOp->sides());
    }
    else if (this->state == UI_INPUT_DICE_KH_STATE)
    {
        this->rollOp->keepHighest = _constrain(this->rollOp->keepHighest - 1, 0, this->rollOp->amount);
    }
    else if (this->state == UI_INPUT_DICE_RO_STATE)
    {
        this->rollOp->reRollLowerThan = _constrain(this->rollOp->reRollLowerThan - 1, 0, this->rollOp->sides());
    }
    else if (this->state == UI_INPUT_MATH_STATE)
    {
        this->mathOp->exp = (this->mathOp->exp + 1) % 2;
    }
}

void AdvancedUI::right(bool longPress)
{
    if (longPress)
    {
        this->expressionTree->evaluateExpression(&result);
        this->state = UI_SHOW_RESULT_STATE;
    }
    else
    {
        if (this->state == UI_CONST_DICE_STATE)
        {
            if (this->uiOpType == UI_OP_CONST)
            {
                this->constOp = std::make_shared<ConstOp>(1);
                this->expressionTree->addNode(this->constOp);
                this->state = UI_INPUT_CONST_STATE;
            }
            else if (this->uiOpType == UI_OP_DICE)
            {
                this->rollOp = std::make_shared<RollOp>(random());
                this->expressionTree->addNode(this->rollOp);
                this->state = UI_INPUT_DICE_X_STATE;
            }
        }
        else if (this->state == UI_INPUT_DICE_X_STATE)
        {
            this->state = UI_INPUT_DICE_Y_STATE;
        }
        else if (this->state == UI_INPUT_DICE_Y_STATE)
        {
            this->state = UI_INPUT_DICE_MIN_STATE;
        }
        else if (this->state == UI_INPUT_DICE_MIN_STATE)
        {
            if (this->rollOp->amount > 1)
            {
                this->state = UI_INPUT_DICE_KH_STATE;
            }
            else
            {
                this->state = UI_INPUT_DICE_RO_STATE;
            }
        }
        else if (this->state == UI_INPUT_DICE_KH_STATE)
        {
            this->state = UI_INPUT_DICE_RO_STATE;
        }
        else if (this->state == UI_INPUT_CONST_STATE || this->state == UI_INPUT_DICE_RO_STATE)
        {
            this->mathOp = std::make_shared<MathOp>(MATH_EXP_PLUS);
            this->expressionTree->addNode(this->mathOp);
            this->state = UI_INPUT_MATH_STATE;
        }
        else if (this->state == UI_INPUT_MATH_STATE)
        {
            this->state = UI_CONST_DICE_STATE;
            this->uiOpType = UI_OP_CONST;
        }
        else if (this->state == UI_SHOW_RESULT_STATE)
        {
            this->left(true);
        }
    }
}

void AdvancedUI::left(bool longPress)
{
    if (longPress)
    {
        delete this->expressionTree;
        this->mathOp = nullptr;
        this->rollOp = nullptr;
        this->constOp = nullptr;
        result.result = 0;
        this->state = UI_CONST_DICE_STATE;
        this->uiOpType = UI_OP_CONST;
        this->expressionTree = new ExpressionTree();
    }
}
