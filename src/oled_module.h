#ifndef OLED_MODULE_H
#define OLED_MODULE_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_SDA 8
#define OLED_SCL 9

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

void init_oled();
void show_status(String title, String status);
void clear_screen();

#endif