//
// Created by Manoel Amaro on 01.02.24.
//

#include "advanced_ui.h"
#include "dice.h"
#include "TrueRandom.h"
#include <MemoryUsage.h>

#define CENTER_TEXT(len) ((128 - ((len) * 6)) / 2)
#define CENTER_2X_TEXT(len) ((128 - ((len) * 12)) / 2)
#define UI_INPUT_Y 56

AdvancedUI::AdvancedUI() : UI(ADVANCED) {
    this->state = UI_CONST_DICE_STATE;
    this->uiOpType = UI_OP_CONST;
    this->expressionTree.reset();
}

void AdvancedUI::printDetails(OpResultDetail *details, DisplaySSD1306_128x64_I2C *display) {
    for (int i = 0; i < OP_RESULT_MAX_DETAILS; i++) {
        auto detail = details[i];
        switch (detail.getOpType()) {
            case OP_RESULT_DETAIL_TYPE_CONST:
                display->print(detail.getValue());
                break;
            case OP_RESULT_DETAIL_TYPE_MATH_PLUS:
                utils::print(display, F("+"));
                break;
            case OP_RESULT_DETAIL_TYPE_MATH_MINUS:
                utils::print(display, F("-"));
                break;
            case OP_RESULT_DETAIL_TYPE_ROLL:
                utils::print(display, F("("));
                display->print(detail.getValue());
                utils::print(display, F(")"));
                break;
            case OP_RESULT_DETAIL_TYPE_ROLL_CRITICAL_SUCCESS:
                utils::print(display, F("**"));
                display->print(detail.getValue());
                utils::print(display, F("**"));
                break;
            case OP_RESULT_DETAIL_TYPE_ROLL_CRITICAL_FAILURE:
                utils::print(display, F("--"));
                display->print(detail.getValue());
                utils::print(display, F("--"));
                break;
            case OP_RESULT_DETAIL_TYPE_ROLL_DROP:
                utils::print(display, F("~"));
                display->print(detail.getValue());
                utils::print(display, F("~"));
                break;
            default:
                break;
        }
    }
}

void _debug_op(op::Op2 *op) {
    if (!op) {
        return;
    }
    if (op->type == OP_MATH) {
        _debug_op(op->math.left);
    }

    if (op->type == OP_CONST) {
        Serial.print(op->constant.value);
    } else if (op->type == OP_MATH) {
        if (op->math.exp == MATH_EXP_PLUS) {
            Serial.print("+");
        } else if (op->math.exp == MATH_EXP_MINUS) {
            Serial.print("-");
        }
    } else if (op->type == OP_DICE) {
        Serial.print(op->roll.amount);
        Serial.print("d");
        Serial.print(op->roll.sides);
        if (op->roll.minimum > 1) {
            Serial.print("m");
            Serial.print(op->roll.minimum);
        }
        if (op->roll.keepHighest > 0) {
            Serial.print("kh");
            Serial.print(op->roll.keepHighest);
        }
        if (op->roll.reRollLowerThan > 0) {
            Serial.print("ro");
            Serial.print(op->roll.reRollLowerThan);
        }
    }
    
    if (op->type == OP_MATH) {
        _debug_op(op->math.right);
    }
}

void AdvancedUI::printOp(op::Op2 *op, DisplaySSD1306_128x64_I2C *display, uint8_t depth = 0) {
    if (!op) {
        return;
    }

    if (op->type == OP_MATH) {
        printOp(op->math.left, display, depth + 1);
    }
    if (op->type == OP_CONST) {
        display->print(op->constant.value);
    } else if (op->type == OP_MATH) {
        if (op->math.left) {
            utils::print(display, F(" "));
            if (op->math.exp == MATH_EXP_PLUS) {
                utils::print(display, F("+"));
            } else if (op->math.exp == MATH_EXP_MINUS) {
                utils::print(display, F("-"));
            }
            utils::print(display, F(" "));
        }
    } else if (op->type == OP_DICE) {
        display->print(op->roll.amount);
        utils::print(display, F("d"));
        display->print(op->roll.sides);
        if (op->roll.minimum > 1) {
            utils::print(display, F("m"));
            display->print(op->roll.minimum);
        }
        if (op->roll.keepHighest > 0) {
            utils::print(display, F("kh"));
            display->print(op->roll.keepHighest);
        }
        if (op->roll.reRollLowerThan > 0) {
            utils::print(display, F("ro"));
            display->print(op->roll.reRollLowerThan);
        }
    }
    if (op->type == OP_MATH) {
        printOp(op->math.right, display, depth + 1);
    }
}

void AdvancedUI::render(DisplaySSD1306_128x64_I2C *display) {
    display->clear();
    if (state == UI_SHOW_RESULT_STATE) {
        display->setTextCursor(CENTER_2X_TEXT(7), 0);
        utils::print(display, F("Results"));
    } else {
        display->setTextCursor(CENTER_2X_TEXT(8), 0);
        utils::print(display, F("Advanced"));
    }
    display->drawHLine(0, 18, 128);
    if (state == UI_SHOW_RESULT_STATE) {
    } else {
        display->drawHLine(0, 52, 128);
    }

    if (state == UI_SHOW_RESULT_STATE) {
    } else {
        display->setTextCursor(0, 24);
        //this->printOp(this->expressionTree.root, display);
        _debug_op(this->expressionTree.root);
    }

    if (state == UI_CONST_DICE_STATE) {
        if (uiOpType == UI_OP_CONST) {
            display->setTextCursor(CENTER_TEXT(3), UI_INPUT_Y);
            utils::print(display, F("[X]"));
        } else if (uiOpType == UI_OP_DICE) {
            display->setTextCursor(CENTER_TEXT(5), UI_INPUT_Y);
            utils::print(display, F("[XdY]"));
        }
    } else if (state == UI_INPUT_CONST_STATE) {
        display->setTextCursor(CENTER_TEXT(2), UI_INPUT_Y);
        display->print(this->currOp->constant.value);
    } else if (state == UI_INPUT_DICE_X_STATE) {
        display->setTextCursor(CENTER_TEXT(5), UI_INPUT_Y);
        utils::print(display, F("["));
        display->print(this->currOp->roll.amount);
        utils::print(display, F("]"));
        utils::print(display, F("d"));
        display->print(this->currOp->roll.sides);
    } else if (state == UI_INPUT_DICE_Y_STATE) {
        display->setTextCursor(CENTER_TEXT(5), UI_INPUT_Y);
        display->print(this->currOp->roll.amount);
        utils::print(display, F("d"));
        utils::print(display, F("["));
        display->print(this->currOp->roll.sides);
        utils::print(display, F("]"));
    } else if (state == UI_INPUT_DICE_MIN_STATE) {
        display->setTextCursor(CENTER_TEXT(14), UI_INPUT_Y);
        utils::print(display, F("["));
        utils::print(display, F("minimum of "));
        display->print(this->currOp->roll.minimum);
        utils::print(display, F("]"));
    } else if (state == UI_INPUT_DICE_KH_STATE) {
        display->setTextCursor(CENTER_TEXT(16), UI_INPUT_Y);
        utils::print(display, F("["));
        utils::print(display, F("keep highest "));
        display->print(this->currOp->roll.keepHighest);
        utils::print(display, F("]"));
    } else if (state == UI_INPUT_DICE_RO_STATE) {
        display->setTextCursor(CENTER_TEXT(15), UI_INPUT_Y);
        utils::print(display, F("["));
        utils::print(display, F("re-roll lte "));
        display->print(this->currOp->roll.reRollLowerThan);
        utils::print(display, F("]"));
    } else if (state == UI_INPUT_MATH_STATE) {
        display->setTextCursor(CENTER_TEXT(1), UI_INPUT_Y);
        if (this->currOp->math.exp == MATH_EXP_PLUS) {
            utils::print(display, F("+"));
        } else if (this->currOp->math.exp == MATH_EXP_MINUS) {
            utils::print(display, F("-"));
        }
    } else if (state == UI_SHOW_RESULT_STATE) {
        display->setTextCursor(CENTER_2X_TEXT(3), 37);
        display->print(this->result.getResult());
    }
}

void AdvancedUI::up(bool isLong) {
    if (this->state == UI_CONST_DICE_STATE) {
        uiOpType = (UIOpType) _constrain(uiOpType + 1, 0, 1);
    } else if (this->state == UI_INPUT_CONST_STATE) {
        this->currOp->constant.value = _constrain(this->currOp->constant.value + 1, 1, 25);
    } else if (this->state == UI_INPUT_DICE_X_STATE) {
        this->currOp->roll.amount = _constrain(this->currOp->roll.amount + 1, 1, 10);
    } else if (this->state == UI_INPUT_DICE_Y_STATE) {
        // TODO: add a way to change the dice sides
        this->currOp->roll.sides;
    } else if (this->state == UI_INPUT_DICE_MIN_STATE) {
        this->currOp->roll.minimum = _constrain(this->currOp->roll.minimum + 1, 1, this->currOp->roll.sides);
    } else if (this->state == UI_INPUT_DICE_KH_STATE) {
        this->currOp->roll.keepHighest = _constrain(this->currOp->roll.keepHighest + 1, 0, this->currOp->roll.amount);
    } else if (this->state == UI_INPUT_DICE_RO_STATE) {
        this->currOp->roll.reRollLowerThan = _constrain(this->currOp->roll.reRollLowerThan + 1, 0,
                                                        this->currOp->roll.sides);
    } else if (this->state == UI_INPUT_MATH_STATE) {
        this->currOp->math.exp = (this->currOp->math.exp + 1) % 2;
    }
}

void AdvancedUI::down(bool longPress) {
    if (this->state == UI_CONST_DICE_STATE) {
        uiOpType = (UIOpType) _constrain(uiOpType - 1, 0, 1);
    } else if (this->state == UI_INPUT_CONST_STATE) {
        this->currOp->constant.value = _constrain(this->currOp->constant.value - 1, 1, 25);
    } else if (this->state == UI_INPUT_DICE_X_STATE) {
        this->currOp->roll.amount = _constrain(this->currOp->roll.amount - 1, 1, 10);
    } else if (this->state == UI_INPUT_DICE_Y_STATE) {
        // TODO: add a way to change the dice sides
        this->currOp->roll.sides;
    } else if (this->state == UI_INPUT_DICE_MIN_STATE) {
        this->currOp->roll.minimum = _constrain(this->currOp->roll.minimum - 1, 1, this->currOp->roll.sides);
    } else if (this->state == UI_INPUT_DICE_KH_STATE) {
        this->currOp->roll.keepHighest = _constrain(this->currOp->roll.keepHighest - 1, 0, this->currOp->roll.amount);
    } else if (this->state == UI_INPUT_DICE_RO_STATE) {
        this->currOp->roll.reRollLowerThan = _constrain(this->currOp->roll.reRollLowerThan - 1, 0,
                                                        this->currOp->roll.sides);
    } else if (this->state == UI_INPUT_MATH_STATE) {
        this->currOp->math.exp = (this->currOp->math.exp + 1) % 2;
    }
}

void AdvancedUI::right(bool longPress) {
    if (longPress) {
        this->expressionTree.evaluateExpression(&this->result);
        this->state = UI_SHOW_RESULT_STATE;
    } else {
        if (this->state == UI_CONST_DICE_STATE) {
            if (this->uiOpType == UI_OP_CONST) {
                this->currOp = op::Op2_new_const(1);
                this->expressionTree.addNode(this->currOp);
                this->state = UI_INPUT_CONST_STATE;
            } else if (this->uiOpType == UI_OP_DICE) {
                this->currOp = op::Op2_new_roll(TrueRandom.random());
                this->expressionTree.addNode(this->currOp);
                this->state = UI_INPUT_DICE_X_STATE;
            }
        } else if (this->state == UI_INPUT_DICE_X_STATE) {
            this->state = UI_INPUT_DICE_Y_STATE;
        } else if (this->state == UI_INPUT_DICE_Y_STATE) {
            this->state = UI_INPUT_DICE_MIN_STATE;
        } else if (this->state == UI_INPUT_DICE_MIN_STATE) {
            if (this->currOp->roll.amount > 1) {
                this->state = UI_INPUT_DICE_KH_STATE;
            } else {
                this->state = UI_INPUT_DICE_RO_STATE;
            }
        } else if (this->state == UI_INPUT_DICE_KH_STATE) {
            this->state = UI_INPUT_DICE_RO_STATE;
        } else if (this->state == UI_INPUT_CONST_STATE || this->state == UI_INPUT_DICE_RO_STATE) {
            this->currOp = op::Op2_new_math(MATH_EXP_PLUS);
            this->expressionTree.addNode(this->currOp);
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
        delete this->currOp;
        this->expressionTree.reset();
        this->state = UI_CONST_DICE_STATE;
    }
}

AdvancedUI::~AdvancedUI() {
    this->expressionTree.reset();
    op::Op2_free(this->currOp);
}
