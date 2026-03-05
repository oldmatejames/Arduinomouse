# Arduinomouse
A short Arduino sketch that turns a USB-capable Arduino into a remote-controlled HID (IE: Mouse/Keyboard)

The loop() function continuously listens for newline-terminated strings on the serial port. It then uses a pretty basic if/else-if chain to turn these into HID inputs.
The Arduino responds "OK\n" on success, and "ERR\n" on an unrecognised command.

To test functionality you can initial the connection (for example using Platformio "pio device monitor") and send the "KR\n" input and should receive an "OK" in response.
```
pio device monitor
--- Terminal on COM5 | 9600 8-N-1
--- Available filters and text transformations: debug, default, direct, hexlify, log2file, nocontrol, printable, send_on_enter, time
--- More details at https://bit.ly/pio-monitor-filters
--- Quit: Ctrl+C | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H
OK
```

Commands:
```
//   MM <dx> <dy>    — Mouse move relative (each component -127..127)
//   MC <L|R|M>      — Mouse click
//   MD <L|R|M>      — Mouse button down
//   MU <L|R|M>      — Mouse button up
//   MW <delta>      — Mouse wheel scroll (+/-)
//   KP <code>       — Key press + release (decimal HID/ASCII code)
//   KD <code>       — Key down
//   KU <code>       — Key up
//   KW <text>       — Type a string
//   KR              — Release all keys and mouse buttons
```
