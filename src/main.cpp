#include <Arduino.h>
#include <Wire.h>
#include "lcdgfx.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include "MainMenu.h"
#include "StandardUI.h"
#include "advanced_ui.h"
#include "button.cpp"
#include <MemoryUsage.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

#define gbi(by, bi) (by & (1 << bi))
#define cbi(by, bi) (by &= ~(1 << bi))
#define sbi(by, bi) (by |= (1 << bi))

#define ROTARY_A 2
#define ROTARY_B 3

#define BUTTON_1 4
#define BUTTON_UP 5
#define BUTTON_DOWN 6
#define BUTTON_2 7
#define OLED_VCC 13

#define SLEEP_TIME 20000

DisplaySSD1306_128x64_I2C display(-1); // or (-1,{busId, addr, scl, sda, frequency})

UI_TYPE currScreen = MAIN_MENU;
MainMenu mainMenu;
StandardUI standardUI;
AdvancedUI advancedUI;

Button leftButton(BUTTON_1);
Button upButton(BUTTON_UP);
Button downButton(BUTTON_DOWN);
Button rightButton(BUTTON_2);

volatile uint32_t lastInteractionTime = 0;

void setupDisplay() {
    delay(500);
    display.begin();
    display.fill(0x00);
    display.setFixedFont(ssd1306xled_font6x8);
    display.printFixed(0, 8, "Snake", STYLE_NORMAL);
    lcd_delay(1000);
}

void sleep() {
    digitalWrite(OLED_VCC, LOW);
    delay(500);
    sleep_enable(); // Enable sleep mode
    power_adc_disable(); // Disable ADC to save power
    sei(); // Enable interrupts
    sleep_mode(); // Go to sleep

    // The program resumes here after wake up
    sleep_disable(); // Disable sleep mode
    power_adc_enable(); // Re-enable ADC
    digitalWrite(OLED_VCC, HIGH);
    setupDisplay();
}

UI *currentUI() {
    switch (currScreen) {
        case MAIN_MENU:
            return &mainMenu;
        case STANDARD:
            return &standardUI;
        case ADVANCED:
            return &advancedUI;
        case HISTORY:
            break;
    }
    return nullptr;
}

void onButtonLeftPress() {
    currentUI()->left(false);
}

void onButtonLeftLongPress() {
    currentUI()->left(true);
    if (currScreen != MAIN_MENU) {
        currScreen = MAIN_MENU;
    }
}

void onButtonUpPress() {
    currentUI()->up(false);
}

void onButtonUpLongPress() {
    currentUI()->up(true);
}

void onButtonDownPress() {
    currentUI()->down(false);
}

void onButtonDownLongPress() {
    currentUI()->down(true);
}

void onButtonRightPress() {
    currentUI()->right(false);
    if (currScreen == MAIN_MENU) {
        if (currentUI()->pos() == 0) {
            currScreen = STANDARD;
        } else if (currentUI()->pos() == 1) {
            currScreen = ADVANCED;
        } else if (currentUI()->pos() == 2) {
            currScreen = HISTORY;
        }
    }
}

void onButtonRightLongPress() {
    currentUI()->right(true);
}

void onEncoderCW() {
    currentUI()->right(false);
}

void onEncoderCCW() {
    currentUI()->left(false);
}

void encoderISR() {
    static const int8_t encStates[] PROGMEM = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
    static volatile uint8_t lastAb = 3;
    static volatile int8_t encVal = 0;
    static volatile uint32_t lastInterruptTime = 0;

    uint32_t interruptTime = millis();

    lastAb <<= 2; // Remember previous state
    uint8_t a = digitalRead(ROTARY_A);
    uint8_t b = digitalRead(ROTARY_B);
    lastAb |= (a << 1) | b; // Add current state
    encVal += encStates[(lastAb & 0x0f)];
    if (encVal > 3 || encVal < -3) {
        int8_t changeValue = (encVal > 0) - (encVal < 0);
        if (millis() - lastInterruptTime < 10) {
            changeValue *= 10;
        }
        lastInterruptTime = millis();
        //encoderPos += changeValue;
        if (changeValue > 0) {
            onEncoderCW();
        } else {
            onEncoderCCW();
        }
        encVal = 0;
    }

    lastInteractionTime = interruptTime;
}

ISR(PCINT2_vect) {
    // This ISR will be executed when pin D2 changes state
    // Note: The ISR is required to allow wake-up from sleep, but doesn't need to do anything.
    lastInteractionTime = millis();
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    MEMORY_PRINT_START
    MEMORY_PRINT_HEAPSTART
    MEMORY_PRINT_HEAPEND
    MEMORY_PRINT_STACKSTART
    MEMORY_PRINT_END
    MEMORY_PRINT_HEAPSIZE
    FREERAM_PRINT

    pinMode(OLED_VCC, OUTPUT);
    digitalWrite(OLED_VCC, HIGH);

    setupDisplay();

    PCICR |= (1 << PCIE2);  // Enable PCINT2 (covers D0 to D7, including D2)

    PCMSK2 |= (1 << PCINT20);
    leftButton.onPress(onButtonLeftPress);
    leftButton.onLongPress(onButtonLeftLongPress);
    leftButton.begin();

    PCMSK2 |= (1 << PCINT21);
    upButton.onPress(onButtonUpPress);
    upButton.onLongPress(onButtonUpLongPress);
    upButton.begin();

    PCMSK2 |= (1 << PCINT22);
    downButton.onPress(onButtonDownPress);
    downButton.onLongPress(onButtonDownLongPress);
    downButton.begin();

    PCMSK2 |= (1 << PCINT23);
    rightButton.onPress(onButtonRightPress);
    rightButton.onLongPress(onButtonRightLongPress);
    rightButton.begin();


    pinMode(ROTARY_A, INPUT_PULLUP);
    pinMode(ROTARY_B, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ROTARY_A), encoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTARY_B), encoderISR, CHANGE);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void loop() {

    if (currentUI()) {
        currentUI()->render(&display);
    }

    leftButton.read();
    upButton.read();
    downButton.read();
    rightButton.read();

    if (lastInteractionTime + SLEEP_TIME < millis()) {
        sleep();
    }
    FREERAM_PRINT;
}