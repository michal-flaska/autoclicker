#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers to speed compilation
#include <windows.h>
#include <fstream>              // File input/output
#include <string>
#include <sstream>              // std::stringstream for parsing hex numbers
#include <iostream>

// Converts a string containing a hex number (like "0x20") into an unsigned int
static UINT hexToUInt(const std::string& s) {
        unsigned int x = 0;             // Variable to store numeric value
        std::stringstream ss;           // Create stringstream for parsing
        ss << std::hex << s;            // Treat string as hexadecimal
        ss >> x;                        // Extract numeric value
        return static_cast<UINT>(x);    // Return as UINT
}

void welcomeMessage() {

        std::string configFile = "config.ini";
        std::string readmeLink = "https://github.com/michal-flaska/autoclicker/blob/main/README.md";
        std::string authorName = "Mike";

        /* Doesnt look as good as expected in the console, so im leaving it commented ;-;

        std::cout << "    _         _             _ _      _             " << '\n';
        std::cout << "   / \  _   _| |_ ___   ___| (_) ___| | _____ _ __ " << '\n';
        std::cout << "  / _ \| | | | __/ _ \ / __| | |/ __| |/ / _ \ '__|" << '\n';
        std::cout << " / ___ \ |_| | || (_) | (__| | | (__|   <  __/ |   " << '\n';
        std::cout << "/_/   \_\__,_|\__\___/ \___|_|_|\___|_|\_\___|_|   " << '\n';

        */

        std::cout << "Hello there," << '\n';
        std::cout << "Make sure you edited settings in the 'config.ini' file!" << '\n';
        std::cout << '\n';
        std::cout << "If you need help, make sure to check out the README at:" << '\n';
        std::cout << readmeLink << '\n';
        std::cout << '\n';
        std::cout << "Have fun!" << '\n';
        std::cout << "--" << authorName << '\n';

}

int main() {

        welcomeMessage();

        // Variables to store configuration values
        UINT trigger = 0;               // Virtual-key code for the key to hold
        UINT action = 0;                // Virtual-key code for the key to simulate
        DWORD initialDelay = 0;         // Delay after trigger is first pressed (ms)
        DWORD repeatDelay = 0;          // Delay between repeated actions while holding trigger
        bool mouseClick = false;        // Whether to simulate a left mouse click instead of a key press

        std::ifstream file("config.ini");       // Open config file in read mode
        if (!file.is_open()) return 1;          // Exit if config cannot be opened

        std::string line; // Buffer to hold each line from the file
        while (std::getline(file, line)) { // Read file line by line
                if (line.find('=') == std::string::npos) continue;      // Skip lines without '='
                auto pos = line.find('=');                              // Find position of '=' character
                std::string key = line.substr(0, pos);                  // Left of '=' is the key
                std::string val = line.substr(pos + 1);                 // Right of '=' is the value

                // Match keys and assign corresponding variables
                if (key == "TriggerKey") trigger = hexToUInt(val);              // Convert hex string to UINT
                else if (key == "ActionKey") action = hexToUInt(val);           // Convert hex string to UINT
                else if (key == "InitialDelay") initialDelay = std::stoul(val); // Convert string to DWORD
                else if (key == "RepeatDelay") repeatDelay = std::stoul(val);   // Convert string to DWORD
                else if (key == "MouseClick") mouseClick = (val == "1");        // Convert string "1"/"0" to bool
        }
        file.close(); // Done reading config file

        // Main loop: runs forever
        for (;;) {
                // Check if the trigger key is currently held down
                if (GetAsyncKeyState(trigger) & 0x8000) {       // 0x8000 = high-order bit indicates key down
                        Sleep(initialDelay);                    // Wait initial delay after first pressing trigger

                        // Inner loop: continue while the trigger key is still held
                        while (GetAsyncKeyState(trigger) & 0x8000) {
                                if (mouseClick) {
                                        // Create array of INPUT structures for mouse down/up
                                        INPUT inputs[2] = {};
                                        inputs[0].type = INPUT_MOUSE;                   // First input is mouse event
                                        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;    // Press left mouse button
                                        inputs[1].type = INPUT_MOUSE;                   // Second input
                                        inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;      // Release left mouse button
                                        SendInput(2, inputs, sizeof(INPUT));            // Send both events to Windows
                                }
                                else {
                                        // Create array of INPUT structures for key press/release
                                        INPUT inputs[2] = {};
                                        inputs[0].type = INPUT_KEYBOARD;                // First input: key down
                                        inputs[0].ki.wVk = static_cast<WORD>(action);   // Set virtual key code
                                        inputs[1].type = INPUT_KEYBOARD;                // Second input: key up
                                        inputs[1].ki.wVk = static_cast<WORD>(action);   // Set the virtual-key code for the key to release (same as action key)
                                        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;         // Mark as key release
                                        SendInput(2, inputs, sizeof(INPUT));            // Send both inputs
                                }

                                Sleep(repeatDelay); // Wait between repeated actions
                        }
                }
                Sleep(10); // Short sleep to reduce CPU usage while polling
        }

        return 0; // Never reached, but standard for main ;-;
}
