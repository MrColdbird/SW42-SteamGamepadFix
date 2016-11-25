# steamy dinput8.dll proxy
This is a simple dinput8.dll proxy library that hooks direct input, as well as Steam's own direct input hook (hookception!).

After hooking said functions, it then identifies a set of known controller GUIDs against a re-mapping list, allowing one to remap controls in games that don't support proper remapping themselves.

In addition to that, it can simulate keyboard key presses.

This project was initially made to fix the horrible button mapping that KOEI bitch-slapped us with in the PC version of Samurai Warriors 4-2, which rendered pretty much every XInput-based gamepad in existence broken (including the official Xbox360 controllers).
The code also contains a keyboard button mapping for the gamepad's start button, linking it to the space key, so that one can skip cutscenes without having to have a physical keyboard available at all times.

This code is however, flexible enough, to act as a generic direct input button remapper for pretty much every game that uses it.
It also serves as a proof of concept for XInput <-> Direct Input button mapping scheme modification, and thus, is to be seen as the other side of tools like Xbox 360 Controller Emulator, which does the opposite.

# How to use it?
Place the dinput8.dll from the binaries subdirectory in your 32bit-based game's application folder, and then copy your operating systems 32bit dinput8.dll, renamed as dinput8base.dll, into your game's application folder.
You can find your operating system's original dinput8.dll in either the C:\Windows\System32 folder (on a 32bit operating system) or in the C:\Windows\SysWoW64 folder (on a 64bit operating system).