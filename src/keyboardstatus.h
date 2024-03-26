#ifndef KEYBOARDSTATUS_H
#define KEYBOARDSTATUS_H

#include <unordered_map>

/*
 * Keeps track of the status of keyboard presses (whether it is being held down or not
 */
class KeyboardStatus
{
public:
    enum class KEY_STATUS {
        DOWN,
        UP
    };
private:
    std::unordered_map<int, KEY_STATUS> keyStatuses;
public:
    /*
     * Update the status of the keyboard key.
     */
    void updateStatus(int key, KEY_STATUS status);

    /*
     * Returns whether a key is currently down (i.e: pressed)
     */
    bool isDown(char key);
};

#endif // KEYBOARDSTATUS_H
