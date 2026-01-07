#ifndef CAMERA_MODULE_H
#define CAMERA_MODULE_H

#include "esp_camera.h"
#include "Arduino.h"
#include "camera_pins.h"

esp_err_t init_camera();
camera_fb_t *capture_photo();
void stop_camera();
#endif