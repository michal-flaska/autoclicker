#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fstream>
#include <string>
#include <sstream>

static UINT hexToUInt(const std::string& s) {
        unsigned int x = 0;
        std::stringstream ss;
        ss << std::hex << s;
        ss >> x;
        return static_cast<UINT>(x);
}

int main() {
        UINT trigger = 0, action = 0;
        DWORD initialDelay = 0, repeatDelay = 0;
        bool mouseClick = false;

        std::ifstream file("config.ini");
        if (!file.is_open()) return 1;

        std::string line;
        while (std::getline(file, line)) {
                if (line.find('=') == std::string::npos) continue;
                auto pos = line.find('=');
                std::string key = line.substr(0, pos), val = line.substr(pos + 1);
                if (key == "TriggerKey") trigger = hexToUInt(val);
                else if (key == "ActionKey") action = hexToUInt(val);
                else if (key == "InitialDelay") initialDelay = std::stoul(val);
                else if (key == "RepeatDelay") repeatDelay = std::stoul(val);
                else if (key == "MouseClick") mouseClick = (val == "1");
        }
        file.close();

        for (;;) {
                if (GetAsyncKeyState(trigger) & 0x8000) {
                        Sleep(initialDelay);
                        while (GetAsyncKeyState(trigger) & 0x8000) {
                                if (mouseClick) {
                                        INPUT inputs[2] = {};
                                        inputs[0].type = INPUT_MOUSE;
                                        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                                        inputs[1].type = INPUT_MOUSE;
                                        inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
                                        SendInput(2, inputs, sizeof(INPUT));
                                }
                                else {
                                        INPUT inputs[2] = {};
                                        inputs[0].type = INPUT_KEYBOARD;
                                        inputs[0].ki.wVk = static_cast<WORD>(action);
                                        inputs[1].type = INPUT_KEYBOARD;
                                        inputs[1].ki.wVk = static_cast<WORD>(action);
                                        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
                                        SendInput(2, inputs, sizeof(INPUT));
                                }
                                Sleep(repeatDelay);
                        }
                }
                Sleep(10);
        }

        return 0;
}
