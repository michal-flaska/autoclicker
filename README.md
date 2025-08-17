# Autoclicker built with C++ & WinAPI

## Overview

A minimal autoclicker built in C++ using the Windows API.
It reads configuration from `config.ini` and simulates key presses or mouse clicks while a trigger key is held.
No GUI, no dependencies beyond Windows itself.

## Features

* Configurable trigger key (hex VK codes).
* Configurable action key or mouse click.
* Adjustable initial delay (before first action after trigger).
* Adjustable repeat delay (between subsequent actions).
* Uses `SendInput` for reliable injection.
* Runs silently in background.

## Project Setup

1. Open **Visual Studio**.
2. Create a new **Empty Project** (C++).
3. Set:
   * Platform: **x64**.
   * Language standard: **C++20** or higher.
   * Character set: **Multi-Byte**.
4. Add a `main.cpp` file with the provided source code.
5. Place `config.ini` in the same folder as the compiled `.exe`.

## Config File

Edit the `config.ini` file.

## Usage

1. Run the compiled `Autoclicker.exe`.
2. Hold the **TriggerKey**.
3. The program automatically simulates the configured key or mouse click.
4. Release the trigger to stop.

## Notes

* Virtual-key codes reference: https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
* `MouseClick=1` bypasses `ActionKey` and always sends a left-click.
* For debugging, you may run with a console visible. For stealth, build with `WinMain` instead of `main`.
* Compiling in **Release x64** is recommended.
