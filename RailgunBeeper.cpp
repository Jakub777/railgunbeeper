#include <SFML/Audio.hpp>
#include <iostream>
#include <thread>
#include <windows.h>

#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33

int beeper() {
    // sampling coicidence protecion (probably useless after adding railgun mode - consider deleting in future)
    for (int i = 0; i < 10; ++i) {
        if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
            return 0;
        Sleep(99);
    }
    // 1s
    for (int i = 0; i < 4; ++i) {
        if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
            return 0;
        Beep(500, 250);
    }
    // 2s
    for (int i = 0; i < 4; ++i) {
        if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
            return 0;
        Beep(500, 100);
    }
    // 2,5s
    for (int i = 0; i < 1; ++i) {
        if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
            return 0;
        Beep(2000, 400);
    }
    // 2,9s
    return 0;
}

bool isKeyReleased(int vKey) {
    return !(GetAsyncKeyState(vKey) & 0x8000);
}

void mouseListener() {
    static bool olderPressCheck = false;
    static bool youngerPressCheck = false; 
    bool RailgunMode = true;
    std::cout << "Railgun mode ON\n";
    std::cout << "Hold LMB for 3s to hear beep\n\n";
    std::cout << "Press key '1' or '2' to quit Railgun mode\n";
    std::cout << "Press key '3' two times in a row to enter Railgun mode\n";

    sf::SoundBuffer buffer_on, buffer_off;
    if (!buffer_on.loadFromFile("mode_activated.mp3") || !buffer_off.loadFromFile("mode_off.mp3")) {
        std::cerr << "cant load a sound!" << std::endl;
        return;
    }
    sf::Sound sound_on(buffer_on), sound_off(buffer_off);

    while (true) {
        if (RailgunMode && (GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
            // play beeps
            beeper();
        }

        else if ( !RailgunMode && GetAsyncKeyState(VK_3) ){
            // variables storage
            youngerPressCheck = olderPressCheck;
            olderPressCheck = true;
            // debouncing protection
            while (true) {
                if (isKeyReleased(0x33))
                    break;
                else
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            // enter railgun mode
            if (olderPressCheck && youngerPressCheck)
            {
                olderPressCheck = false;
                youngerPressCheck = false;
                RailgunMode = true;
                sound_on.play();
            }
        }

        else if (GetAsyncKeyState(VK_1) & 0x8000 || GetAsyncKeyState(VK_2) & 0x8000)  {
            // variables reset
            olderPressCheck = false;
            youngerPressCheck = false;
            // quit railgun mode
            if (RailgunMode == true) {
                sound_off.play();
                RailgunMode = false;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main() {
    std::thread listenerThread(mouseListener);
    listenerThread.detach(); 

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "\nPress Enter to leave...\n" << std::endl;
    std::cin.get();
    return 0;
}