#include <Bluetooth.h>
#include <Joystick.h>
#include <Led.h>
#include <Keypad.h>

#define loopTime 20
#define DEBUG false

//                    RX  TX
SoftwareSerial Serial1(2, 4);
Bluetooth bluetooth(&Serial1);
Joystick leftJoystick(A1, A0, A2, true, false, 512, 512, 50, 50);
Joystick rightJoystick(A4, A3, A5, true, false, 512, 512, 50, 50);
Button switcher(A6);
Led blueLed(12);
Led whiteLed(11);
Led redLed(13);

int fromPin = 4;
char keys[4][3] = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } };
byte rowPins[4] = { 5, 10, 9, 7 };
byte colPins[3] = { 6, 4, 8 };
byte rows = 4;
byte cols = 3;
Keypad keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

int estimation = -1;

void setup ()
{
  // Serial setup //
  {
    Serial.begin(9600);
    Serial1.begin(9600);
#if DEBUG
    Serial.println("Serial communication's on...");
    Serial.println("Bluetooth communication's on...");
    Serial.println("Debug mode's on...");
#endif
    blueLed.off();
    whiteLed.off();
    redLed.off();
  }
}

void loop ()
{
  // Fetch data to json and send it via bluetooth //
  {
    bluetooth.json["switch"] = switcher.getAnalogValue() > 512;
    if (bluetooth.json["switch"].as<bool>()) {
      whiteLed.on();
    }
    else {
      whiteLed.off();
    }
    const int keypadValue = keypad.getKey();
    bluetooth.json["keypad"] = keypadValue;
    if (keypadValue == 12) {
      redLed.on();
      bluetooth.json["keypad"] = 0;
      int key = keypad.getKey();
      estimation = 0;
      while (key != 12 && ((int)log10(estimation) + 1) < 3) {
        if (key >= 1 && key <= 9) {
          estimation = estimation * 10 + key;

        }
        if (key == 11) {
          estimation *= 10;
        }
        key = keypad.getKey();
      }
      redLed.off();
    }
    bluetooth.json["estimation"] = estimation;
    {
      {
        bluetooth.json["joysticks"]["left"]["x"] = leftJoystick.x.getValue();
        bluetooth.json["joysticks"]["left"]["y"] = leftJoystick.y.getValue();
        bluetooth.json["joysticks"]["left"]["clck"] = leftJoystick.clck.getValue();
      }
      {
        bluetooth.json["joysticks"]["right"]["x"] = rightJoystick.x.getValue();
        bluetooth.json["joysticks"]["right"]["y"] = rightJoystick.y.getValue();
        bluetooth.json["joysticks"]["right"]["clck"] = rightJoystick.clck.getValue();
      }
    }
    bluetooth.send();
#if DEBUG
    serializeJsonPretty(bluetooth.json, Serial);
#endif
    blueLed.on();
  }
  delay(loopTime);
}
