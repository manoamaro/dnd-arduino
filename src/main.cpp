#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include "./images.h"
#include "./button.cpp"
#include <TrueRandom.h>
#include "./Op.h"

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

#define gbi(by, bi) (by & (1 << bi))
#define cbi(by, bi) (by &= ~(1 << bi))
#define sbi(by, bi) (by |= (1 << bi))

#define DICE_COUNT 6
#define MAX_COUNT_PER_DICE 5
#define INIT_DICE_COUNT 0

#define BUTTON_DIE 2
#define BUTTON_PLUS 4
#define BUTTON_MINUS 5
#define BUTTON_ROLL 6
#define OLED_VCC 13

#define SLEEP_TIME 20000

enum Screen {
    DICE,
    ROLLS,
    SUM
};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Button diceButton(BUTTON_DIE);
Button plusButton(BUTTON_PLUS);
Button minusButton(BUTTON_MINUS);
Button rollButton(BUTTON_ROLL);

int curr_die = 0;
uint8_t dice_value[DICE_COUNT] = {4, 6, 8, 10, 12, 20};
uint8_t dice_count[DICE_COUNT] = {INIT_DICE_COUNT};
uint8_t dice_results[DICE_COUNT][MAX_COUNT_PER_DICE];
uint16_t sum = 0;
volatile uint32_t last_interaction_time = 0;

Screen screen = DICE;

void rotate() {
    curr_die = (curr_die + 1) % DICE_COUNT;
    screen = DICE;
    last_interaction_time = millis();
}

void reset() {
    curr_die = 0;
    for (int i = 0; i < MAX_COUNT_PER_DICE; i++) {
        dice_count[i] = INIT_DICE_COUNT;
    }
    screen = DICE;
    last_interaction_time = millis();
}

void inc() {
    dice_count[curr_die] = min(dice_count[curr_die] + 1, MAX_COUNT_PER_DICE);
    screen = DICE;
    last_interaction_time = millis();
}

void dec() {
    dice_count[curr_die] = max(dice_count[curr_die] - 1, 0);
    screen = DICE;
    last_interaction_time = millis();
}

void roll() {
    if (screen != SUM) {
        sum = 0;
        for (uint8_t i = 0; i < DICE_COUNT; i++) {
            for (uint8_t j = 0; j < MAX_COUNT_PER_DICE; j++) {
                dice_results[i][j] = 0;
            }
        }

        for (uint8_t i = 0; i < DICE_COUNT; i++) {
            for (uint8_t j = 0; j < dice_count
            [i]; j++) {
                dice_results[i][j] = TrueRandom.random(1, dice_value[i] + 1);
                sum += dice_results[i][j];
            }
        }
    }
    screen = ROLLS;
    last_interaction_time = millis();
}

void showSum() {
    if (screen == ROLLS) {
        screen = SUM;
    } else {
        screen = ROLLS;
    }
    last_interaction_time = millis();
}

void wake() {
    last_interaction_time = millis();
}


int rotateNumber(int input, int max) {
    if (input >= 0 && input < max) {
        return input;
    } else if (input < 0) {
        return max + input; // Rotates the negative number within the range
    } else {
        return input % max; // Rotates the number back into the range for inputs >= max
    }
}

void splash() {
    display.clearDisplay();
    display.drawBitmap(0, 0, image_d20, IMAGE_WIDTH, IMAGE_HEIGHT, WHITE);
    display.display();
    delay(1000);
}

void render() {
    char buffer[10];
    if (screen == DICE) {
        display.clearDisplay();
        display.drawBitmap(-4, 20, icon_allArray[rotateNumber(curr_die - 2, DICE_COUNT)], ICON_WIDTH, ICON_HEIGHT,
                           WHITE);
        display.drawBitmap(22, 10, icon_allArray[rotateNumber(curr_die - 1, DICE_COUNT)], ICON_WIDTH, ICON_HEIGHT,
                           WHITE);
        display.drawBitmap(48, 5, image_allArray[curr_die], IMAGE_WIDTH, IMAGE_HEIGHT, WHITE);
        display.drawBitmap(86, 10, icon_allArray[rotateNumber(curr_die + 1, DICE_COUNT)], ICON_WIDTH, ICON_HEIGHT,
                           WHITE);
        display.drawBitmap(112, 20, icon_allArray[rotateNumber(curr_die + 2, DICE_COUNT)], ICON_WIDTH, ICON_HEIGHT,
                           WHITE);
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(40, 45);
        sprintf(buffer, "%dxd%d", dice_count[curr_die], dice_value[curr_die]);
        display.write(buffer);
        display.fillRoundRect(0, 0, 128, 64, 4, SSD1306_INVERSE);
        display.display();
    } else if (screen == ROLLS) {
        display.clearDisplay();
        display.setTextSize(1);
        for (uint8_t i = 0; i < DICE_COUNT; i++) {
            sprintf(buffer, "d%d", dice_value[i]);
            display.setCursor(0, 4 + i * 10);
            display.write(buffer);
            for (uint8_t j = 0; j < MAX_COUNT_PER_DICE; j++) {
                if (dice_results[i][j] > 0) {
                    sprintf(buffer, "%d", dice_results[i][j]);
                } else {
                    sprintf(buffer, "*");
                }
                display.setCursor(32 + j * 20, 4 + i * 10);
                display.write(buffer);
            }
        }
        display.fillRoundRect(20, 0, display.width() - 20, display.height(), 4, SSD1306_INVERSE);
        display.display();
    } else if (screen == SUM) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(35, 10);
        display.write("Total");
        display.setCursor(45, 35);
        sprintf(buffer, "%03d", sum);
        display.write(buffer);
        display.display();
    }
}

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
    attachInterrupt(0, wake, CHANGE);
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

void setup() {
    // put your setup code here, to run once:
    pinMode(OLED_VCC, OUTPUT);
    digitalWrite(OLED_VCC, HIGH);

    setupDisplay();

    diceButton.onPress(rotate);
    diceButton.onLongPress(reset);
    diceButton.begin();

    plusButton.onPress(inc);
    plusButton.begin();

    minusButton.onPress(dec);
    minusButton.begin();

    rollButton.onPress(roll);
    rollButton.onLongPress(showSum);
    rollButton.begin();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    splash();
}

uint32_t last_rendered_time = 0;

void loop() {
    if (last_rendered_time + 30 < millis()) {
        render();
        last_rendered_time = millis();
    }

    diceButton.read();
    plusButton.read();
    minusButton.read();
    rollButton.read();

    if (last_interaction_time + SLEEP_TIME < millis()) {
        sleep();
    }
}