#ifndef KEYEVENT_H
#define KEYEVENT_H

#include <array>

class KeyEvent {
public:
    enum KeyStatus {
        UNPRESSED, PRESSED, LOCKED, TEMP_LOCKED
    };

    static void updateKeyStatus (int key, int action);

    // Trava a tecla `key` por `secs` segundos, impedindo ela repetir tao rapido
    static void tempLockKey (int key, double secs);

    static std::array <KeyStatus, 1024> keys;
    static std::array <double, 1024> key_temp_lock;
};

#endif
