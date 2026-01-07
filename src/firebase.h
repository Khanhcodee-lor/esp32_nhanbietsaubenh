#ifndef FIREBASE_H
#define FIREBASE_H

#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include "camera_module.h"

// Cấu hình firebase
#define DATABASE_URL "https://aiot-caytrong-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define DATABASE_SECRET "kmwVpgH0L64u6ZkE2frN7wJVLpNQQ0XOXCKL5Sxr"
#define STORAGE_BUCKET_ID "aiot-caytrong.firebasestorage.app"

extern FirebaseData fbdo;
extern FirebaseAuth auth;
extern FirebaseConfig config;

void init_firebase();
void send_photo_to_firebase(camera_fb_t *fb);
bool check_capture_command();
#endif