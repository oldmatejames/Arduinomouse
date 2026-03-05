#include <Arduino.h>
#include <Keyboard.h>
#include <Mouse.h>

// Serial protocol: newline-terminated commands at 9600 baud.
// Arduino responds "OK\n" on success, "ERR\n" on unrecognised command.
//
// Commands:
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

static uint8_t parseButton(const String& arg) {
    if (arg == "R") return MOUSE_RIGHT;
    if (arg == "M") return MOUSE_MIDDLE;
    return MOUSE_LEFT;
}

void handleCommand(const String& cmd) {
    if (cmd.length() < 2) { Serial.println("ERR"); return; }

    String type = cmd.substring(0, 2);
    String args = (cmd.length() > 3) ? cmd.substring(3) : String("");

    if (type == "MM") {
        int sp = args.indexOf(' ');
        if (sp < 0) { Serial.println("ERR"); return; }
        int dx = constrain(args.substring(0, sp).toInt(), -127, 127);
        int dy = constrain(args.substring(sp + 1).toInt(), -127, 127);
        Mouse.move(dx, dy, 0);

    } else if (type == "MC") {
        Mouse.click(parseButton(args));

    } else if (type == "MD") {
        Mouse.press(parseButton(args));

    } else if (type == "MU") {
        Mouse.release(parseButton(args));

    } else if (type == "MW") {
        Mouse.move(0, 0, constrain(args.toInt(), -127, 127));

    } else if (type == "KP") {
        uint8_t code = (uint8_t)args.toInt();
        Keyboard.press(code);
        delay(8);
        Keyboard.release(code);

    } else if (type == "KD") {
        Keyboard.press((uint8_t)args.toInt());

    } else if (type == "KU") {
        Keyboard.release((uint8_t)args.toInt());

    } else if (type == "KW") {
        Keyboard.print(args);

    } else if (type == "KR") {
        Keyboard.releaseAll();
        Mouse.release(MOUSE_LEFT);
        Mouse.release(MOUSE_RIGHT);
        Mouse.release(MOUSE_MIDDLE);

    } else {
        Serial.println("ERR");
        return;
    }

    Serial.println("OK");
}

void setup() {
    Serial.begin(9600);
    Keyboard.begin();
    Mouse.begin();
}

void loop() {
    if (Serial.available() > 0) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd.length() > 0) {
            handleCommand(cmd);
        }
    }
}
