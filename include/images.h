#ifndef DND_IMAGES_H
#define DND_IMAGES_H

#include <Arduino.h>

#define IMAGE_HEIGHT 32
#define IMAGE_WIDTH 32
#define ICON_HEIGHT 20
#define ICON_WIDTH 20

extern const unsigned char image_dragon[] PROGMEM;
extern const unsigned char* image_allArray[6];
extern const unsigned char* icon_allArray[6];

#endif //DND_IMAGES_H

