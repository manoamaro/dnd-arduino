//
// Created by Manoel Amaro on 01.02.24.
//

#include "AdvancedUI.h"
#include "Dice.h"
#include "TrueRandom.h"

#define CENTER_TEXT(len) ((128 - ((len) * 6)) / 2)
#define CENTER_2X_TEXT(len) ((128 - ((len) * 12)) / 2)
#define UI_INPUT_Y 56

AdvancedUI::AdvancedUI() : UI(ADVANCED) {
    this->state = UI_CONST_DICE_STATE;
    this->uiOpType = UI_OP_CONST;
    this->expressionTree = new ExpressionTree();
}

void AdvancedUI::printOp(Op *op, Adafruit_SSD1306 *display, uint8_t depth = 0) {
    if (!op) {
        return;
    }

    if (op->type == OP_MATH) {
        printOp(((MathOp *) op)->left, display, depth + 1);
    }
    if (op->type == OP_CONST) {
        display->print(((ConstOp *) op)->value, DEC);
    } else if (op->type == OP_MATH) {
        auto mathOp = (MathOp *) op;
        if (mathOp->left) {
            display->print(F(" "));
            if (mathOp->exp == MATH_EXP_PLUS) {
                display->print(F("+"));
            } else if (mathOp->exp == MATH_EXP_MINUS) {
                display->print(F("-"));
            }
            display->print(F(" "));
        }
    } else if (op->type == OP_DICE) {
        auto diceOp = (RollOp *) op;
        display->print(diceOp->amount);
        display->print(F("d"));
        display->print(diceOp->sides());
        if (diceOp->minimum > 1) {
            display->print(F("m"));
            display->print(diceOp->minimum);
        }
        if (diceOp->keepHighest > 0) {
            display->print(F("kh"));
            display->print(diceOp->keepHighest);
        }
        if (diceOp->reRollLowerThan > 0) {
            display->print(F("ro"));
            display->print(diceOp->reRollLowerThan);
        }
    }
    if (op->type == OP_MATH) {
        printOp(((MathOp *) op)->right, display, depth + 1);
    }
}

void AdvancedUI::render(Adafruit_SSD1306 *display) {
    display->clearDisplay();
    display->setTextSize(2);
    display->setTextColor(WHITE);
    if (state == UI_SHOW_RESULT_STATE) {
        display->setCursor(CENTER_2X_TEXT(7), 0);
        display->print(F("Results"));
    } else {
        display->setCursor(CENTER_2X_TEXT(8), 0);
        display->print(F("Advanced"));
    }
    display->setTextSize(1);
    display->drawFastHLine(0, 18, 128, SSD1306_WHITE);
    if (state == UI_SHOW_RESULT_STATE) {
    } else {
        display->drawFastHLine(0, 52, 128, SSD1306_WHITE);
    }

    if (state == UI_SHOW_RESULT_STATE) {
    } else {
        if (this->expressionTree) {
            display->setCursor(0, 24);
            this->printOp(this->expressionTree->root, display);
        }
    }

    if (state == UI_CONST_DICE_STATE) {
        if (uiOpType == UI_OP_CONST) {
            display->setCursor(CENTER_TEXT(3), UI_INPUT_Y);
            display->print(F("[X]"));
        } else if (uiOpType == UI_OP_DICE) {
            display->setCursor(CENTER_TEXT(5), UI_INPUT_Y);
            display->print(F("[XdY]"));
        }
    } else if (state == UI_INPUT_CONST_STATE) {
        display->setCursor(CENTER_TEXT(2), UI_INPUT_Y);
        display->print(this->currConstOp->value, DEC);
    } else if (state == UI_INPUT_DICE_X_STATE) {
        display->setCursor(CENTER_TEXT(5), UI_INPUT_Y);
        display->print(F("["));
        display->print(this->currRollOp->amount);
        display->print(F("]"));
        display->print(F("d"));
        display->print(this->currRollOp->sides());
    } else if (state == UI_INPUT_DICE_Y_STATE) {
        display->setCursor(CENTER_TEXT(5), UI_INPUT_Y);
        display->print(this->currRollOp->amount);
        display->print(F("d"));
        display->print(F("["));
        display->print(this->currRollOp->sides());
        display->print(F("]"));
    } else if (state == UI_INPUT_MATH_STATE) {
        display->setCursor(CENTER_TEXT(1), UI_INPUT_Y);
        if (this->currMathOp->exp == MATH_EXP_PLUS) {
            display->print(F("+"));
        } else if (this->currMathOp->exp == MATH_EXP_MINUS) {
            display->print(F("-"));
        }
    } else if (state == UI_INPUT_DICE_MIN_STATE) {
        display->setCursor(CENTER_TEXT(14), UI_INPUT_Y);
        display->print(F("["));
        display->print(F("minimum of "));
        display->print(this->currRollOp->minimum);
        display->print(F("]"));
    } else if (state == UI_INPUT_DICE_KH_STATE) {
        display->setCursor(CENTER_TEXT(16), UI_INPUT_Y);
        display->print(F("["));
        display->print(F("keep highest "));
        display->print(this->currRollOp->keepHighest);
        display->print(F("]"));
    } else if (state == UI_INPUT_DICE_RO_STATE) {
        display->setCursor(CENTER_TEXT(15), UI_INPUT_Y);
        display->print(F("["));
        display->print(F("re-roll lte "));
        display->print(this->currRollOp->reRollLowerThan);
        display->print(F("]"));
    } else if (state == UI_SHOW_RESULT_STATE) {
        display->setTextSize(2);
        display->setCursor(CENTER_2X_TEXT(3), 37);
        display->print(this->result);
    }
    display->display();
}

void AdvancedUI::up(bool isLong) {
    if (this->state == UI_CONST_DICE_STATE) {
        uiOpType = (UIOpType) _constrain(uiOpType + 1, 0, 1);
    } else if (this->state == UI_INPUT_CONST_STATE) {
        this->currConstOp->value = _constrain(this->currConstOp->value + 1, 1, 25);
    } else if (this->state == UI_INPUT_DICE_X_STATE) {
        this->currRollOp->amount = _constrain(this->currRollOp->amount + 1, 1, 10);
    } else if (this->state == UI_INPUT_DICE_Y_STATE) {
        this->currRollOp->incSides();
    } else if (this->state == UI_INPUT_DICE_MIN_STATE) {
        this->currRollOp->minimum = _constrain(this->currRollOp->minimum + 1, 1, this->currRollOp->sides());
    } else if (this->state == UI_INPUT_DICE_KH_STATE) {
        this->currRollOp->keepHighest = _constrain(this->currRollOp->keepHighest + 1, 0, this->currRollOp->amount);
    } else if (this->state == UI_INPUT_DICE_RO_STATE) {
        this->currRollOp->reRollLowerThan = _constrain(this->currRollOp->reRollLowerThan + 1, 0, this->currRollOp->sides());
    } else if (this->state == UI_INPUT_MATH_STATE) {
        this->currMathOp->exp = (this->currMathOp->exp + 1) % 2;
    }
}

void AdvancedUI::down(bool longPress) {
    if (this->state == UI_CONST_DICE_STATE) {
        uiOpType = (UIOpType) _constrain(uiOpType - 1, 0, 1);
    } else if (this->state == UI_INPUT_CONST_STATE) {
        this->currConstOp->value = _constrain(this->currConstOp->value - 1, 1, 25);
    } else if (this->state == UI_INPUT_DICE_X_STATE) {
        this->currRollOp->amount = _constrain(this->currRollOp->amount - 1, 1, 10);
    } else if (this->state == UI_INPUT_DICE_Y_STATE) {
        this->currRollOp->decSides();
    } else if (this->state == UI_INPUT_DICE_MIN_STATE) {
        this->currRollOp->minimum = _constrain(this->currRollOp->minimum - 1, 1, this->currRollOp->sides());
    } else if (this->state == UI_INPUT_DICE_KH_STATE) {
        this->currRollOp->keepHighest = _constrain(this->currRollOp->keepHighest - 1, 0, this->currRollOp->amount);
    } else if (this->state == UI_INPUT_DICE_RO_STATE) {
        this->currRollOp->reRollLowerThan = _constrain(this->currRollOp->reRollLowerThan - 1, 0, this->currRollOp->sides());
    } else if (this->state == UI_INPUT_MATH_STATE) {
        this->currMathOp->exp = (this->currMathOp->exp + 1) % 2;
    }
}

void AdvancedUI::right(bool longPress) {
    if (longPress) {
        this->result = this->expressionTree->evaluateExpression();
        this->state = UI_SHOW_RESULT_STATE;
    } else {
        if (this->state == UI_CONST_DICE_STATE) {
            if (this->uiOpType == UI_OP_CONST) {
                // CONST selected
                this->currConstOp = new ConstOp(0);
                this->expressionTree->addNode(this->currConstOp);
                this->state = UI_INPUT_CONST_STATE;
            } else if (this->uiOpType == UI_OP_DICE) {
                this->currRollOp = new RollOp(TrueRandom.rand());
                this->expressionTree->addNode(this->currRollOp);
                this->state = UI_INPUT_DICE_X_STATE;
            }
        } else if (this->state == UI_INPUT_DICE_X_STATE) {
            this->state = UI_INPUT_DICE_Y_STATE;
        } else if (this->state == UI_INPUT_DICE_Y_STATE) {
            this->state = UI_INPUT_DICE_MIN_STATE;
        } else if (this->state == UI_INPUT_DICE_MIN_STATE) {
            if (this->currRollOp->amount > 1) {
                this->state = UI_INPUT_DICE_KH_STATE;
            } else {
                this->state = UI_INPUT_DICE_RO_STATE;
            }
        } else if (this->state == UI_INPUT_DICE_KH_STATE) {
            this->state = UI_INPUT_DICE_RO_STATE;
        } else if (this->state == UI_INPUT_CONST_STATE || this->state == UI_INPUT_DICE_RO_STATE) {
            this->currMathOp = new MathOp(MATH_EXP_PLUS);
            this->expressionTree->addNode(this->currMathOp);
            this->state = UI_INPUT_MATH_STATE;
        } else if (this->state == UI_INPUT_MATH_STATE) {
            this->state = UI_CONST_DICE_STATE;
            this->uiOpType = UI_OP_CONST;
        } else if (this->state == UI_SHOW_RESULT_STATE) {
            this->left(true);
        }
    }
}

void AdvancedUI::left(bool longPress) {
    if (longPress) {
        delete this->expressionTree;
        this->expressionTree = new ExpressionTree();
        this->state = UI_CONST_DICE_STATE;
    }
}

AdvancedUI::~AdvancedUI() {
    delete this->expressionTree;
    delete this->currConstOp;
    delete this->currRollOp;
    delete this->currMathOp;
}
