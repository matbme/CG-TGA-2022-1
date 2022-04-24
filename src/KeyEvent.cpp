#include "KeyEvent.h"
#include <iostream>

#include <GLFW/glfw3.h>

void KeyEvent::updateKeyStatus (int key, int action) {
    switch (KeyEvent::keys[key]) {
        case UNPRESSED:
            if (action == GLFW_PRESS) KeyEvent::keys[key] = PRESSED;
            break;
        case PRESSED:
        case LOCKED:
            if (action == GLFW_RELEASE) KeyEvent::keys[key] = UNPRESSED;
            break;
        case TEMP_LOCKED:
            if (action == GLFW_RELEASE) {
                KeyEvent::keys[key] = UNPRESSED;
            }
            else if (glfwGetTime () >= KeyEvent::key_temp_lock[key]) {
                KeyEvent::keys[key] = PRESSED;
            } 
            break;
    }
}

void KeyEvent::tempLockKey(int key, double secs) {
    KeyEvent::keys[key] = TEMP_LOCKED;
    KeyEvent::key_temp_lock[key] = glfwGetTime () + secs;
}

std::array <KeyEvent::KeyStatus, 1024> KeyEvent::keys = { KeyEvent::KeyStatus::UNPRESSED };
std::array <double, 1024> KeyEvent::key_temp_lock = { 0.0f };
