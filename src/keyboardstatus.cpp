#include "keyboardstatus.h"

void KeyboardStatus::updateStatus(int key, KEY_STATUS status) {
    keyStatuses[key] = status;
}

bool KeyboardStatus::isDown(char key) {
    if (keyStatuses.find(key) == keyStatuses.end()) {
        // Initialize key to up, if status has not been set.
        keyStatuses[key] = KEY_STATUS::UP;
    }

    return keyStatuses[key] == KEY_STATUS::DOWN;
}
