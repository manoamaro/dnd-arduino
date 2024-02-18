#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include "MainMenu.h"
#include "button.cpp"
#include "StandardUI.h"
#include "AdvancedUI.h"

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

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

UI *currentUI;

Button leftButton(BUTTON_1);
Button upButton(BUTTON_UP);
Button downButton(BUTTON_DOWN);
Button rightButton(BUTTON_2);

volatile uint32_t lastInteractionTime = 0;

void setupDisplay() {
    delay(500);
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        for (;;);  // Don't proceed, loop forever
    }

    display.setTextColor(WHITE);
    display.clearDisplay();
    display.display();
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

void onButtonLeftPress() {
    currentUI->left(false);
}

void onButtonLeftLongPress() {
    currentUI->left(true);
    if (currentUI->type != MAIN_MENU) {
        delete currentUI;
        currentUI = new MainMenu();
    }
}

void onButtonUpPress() {
    currentUI->up(false);
}

void onButtonUpLongPress() {
    currentUI->up(true);
}

void onButtonDownPress() {
    currentUI->down(false);
}

void onButtonDownLongPress() {
    currentUI->down(true);
}

void onButtonRightPress() {
    currentUI->right(false);
    if (currentUI->type == MAIN_MENU) {
        if (currentUI->pos() == 0) {
            delete currentUI;
            currentUI = new StandardUI();
        } else if (currentUI->pos() == 1) {
            delete currentUI;
            currentUI = new AdvancedUI();
        } else if (currentUI->pos() == 2) {
        }
    }
}

void onButtonRightLongPress() {
    currentUI->right(true);
}

void onEncoderCW() {
    currentUI->right(false);
}

void onEncoderCCW() {
    currentUI->left(false);
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

    currentUI = new MainMenu();
}

void loop() {

    if (currentUI) {
        currentUI->render(&display);
    }

    leftButton.read();
    upButton.read();
    downButton.read();
    rightButton.read();

    if (lastInteractionTime + SLEEP_TIME < millis()) {
        sleep();
    }
}