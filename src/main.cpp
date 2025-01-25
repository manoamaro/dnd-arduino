#include <Arduino.h>

#ifdef AVR
#include <avr/sleep.h>
#include <avr/power.h>
#endif


#include "main_menu.h"
#include "standard_ui.h"
#include "advanced_ui.h"
#include "button.h"
#include "display.h"

#ifdef AVR
#define BUTTON_1 4
#define BUTTON_UP 5
#define BUTTON_DOWN 6
#define BUTTON_2 7
#define OLED_VCC 13
#else
#define BUTTON_1 12
#define BUTTON_UP 13
#define BUTTON_DOWN 14
#define BUTTON_2 15
#define OLED_VCC 27
#endif

#define SLEEP_TIME 20000

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET (-1) // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Display display(&_display);

UI *currentUI = nullptr;

Button<4> button({BUTTON_1, BUTTON_UP, BUTTON_DOWN, BUTTON_2});

volatile uint32_t lastInteractionTime = 0;

void onButtonLeftPress(bool longPress) {
    if (longPress) {
        currentUI->left(true);
        if (currentUI->type != MAIN_MENU) {
            delete currentUI;
            currentUI = new MainMenu();
        }
    } else {
        currentUI->left(false);
    }
}

void onButtonUpPress(bool longPress) {
    currentUI->up(longPress);
}

void onButtonDownPress(bool longPress) {
    currentUI->down(longPress);
}

void onButtonRightPress(bool longPress) {
    currentUI->right(longPress);
    if (!longPress && currentUI->type == MAIN_MENU) {
        switch (currentUI->pos()) {
            case 0:
                delete currentUI;
                currentUI = new StandardUI();
                break;
            case 1:
                delete currentUI;
                currentUI = new AdvancedUI();
                break;
            case 2:
                delete currentUI;
                currentUI = new MainMenu();
                break;
        }
    }
}

void onButtonPress(uint8_t button, bool longPress) {
    switch (button) {
        case BUTTON_1:
            onButtonLeftPress(longPress);
            break;
        case BUTTON_UP:
            onButtonUpPress(longPress);
            break;
        case BUTTON_DOWN:
            onButtonDownPress(longPress);
            break;
        case BUTTON_2:
            onButtonRightPress(longPress);
            break;
    }
}

#ifdef AVR
ISR(PCINT2_vect) {
    // This ISR will be executed when pin D2 changes state
    // Note: The ISR is required to allow wake-up from sleep, but doesn't need to do anything.
    lastInteractionTime = millis();
}
#endif

bool render() {
    if (currentUI) {
        currentUI->render(&display);
    }
    return true;
}

void setupDisplay() {
    display.setup();
}

void _sleep() {
    #ifdef AVR
    digitalWrite(OLED_VCC, LOW);
    delay(500);
    sleep_enable();      // Enable sleep mode
    power_adc_disable(); // Disable ADC to save power
    sei();               // Enable interrupts
    sleep_mode();        // Go to sleep

    // The program resumes here after wake up
    sleep_disable();    // Disable sleep mode
    power_adc_enable(); // Re-enable ADC
    digitalWrite(OLED_VCC, HIGH);
    setupDisplay();
    #endif
}

void setupInterrupts() {
    #ifdef AVR
    PCICR |= (1 << PCIE2); // Enable PCINT2 (covers D0 to D7, including D2)
    PCMSK2 |= (1 << PCINT20);
    PCMSK2 |= (1 << PCINT21);
    PCMSK2 |= (1 << PCINT22);
    PCMSK2 |= (1 << PCINT23);
    #endif
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    Serial.println("Starting...");

    pinMode(OLED_VCC, OUTPUT);
    digitalWrite(OLED_VCC, HIGH);

    setupDisplay();

    button.onPress(onButtonPress);
    button.begin();

    setupInterrupts();
    currentUI = new MainMenu();
    Serial.println("Started");
}

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef ESP32
    return esp_get_free_heap_size();
#elif defined(__arm__)
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

void loop() {
    // put your main code here, to run repeatedly:
    render();
    button.read();
    if (lastInteractionTime + SLEEP_TIME < millis()) {
        //_sleep();
    }

    Serial.print("Free memory: ");
    Serial.println(freeMemory()); 
}