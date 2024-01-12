#include <SoftwareSerial.h>
#include <Bluetooth.h>
#include <Joystick.h>
#include <Led.h>
#include <Keypad.h>

#define DEBUG false

#define SWITCH 0
#define KEYPAD 1
#define ESTIMATION 2
#define JOYSTICK_LEFT_X 3
#define JOYSTICK_LEFT_Y 4
#define JOYSTICK_LEFT_CLCK 5
#define JOYSTICK_RIGHT_X 6
#define JOYSTICK_RIGHT_Y 7
#define JOYSTICK_RIGHT_CLCK 8

#define NUM_VALUES 9

//                    RX  TX
SoftwareSerial Serial1(2, 3);

int sizes[NUM_VALUES] = { 1, 4, 8, 9, 9, 1, 9, 9, 1 };
Bluetooth bluetooth(&Serial1, sizes, NUM_VALUES, '@');
Joystick leftJoystick(A1, A0, A2, true, false, 512, 512, 50, 50, 0, 512);
Joystick rightJoystick(A4, A3, A5, true, false, 512, 512, 50, 50, 0, 512);
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
    Serial1.begin(9600);
    Serial.begin(9600);
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
    bluetooth.message.set(SWITCH, switcher.getAnalogValue() > 512);
    if (bluetooth.message.get(SWITCH) == 1) {
      whiteLed.on();
    }
    else {
      whiteLed.off();
    }
    bluetooth.message.set(KEYPAD, keypad.getKey());
    if (bluetooth.message.get(KEYPAD) == 12) {
      redLed.on();
      bluetooth.message.set(KEYPAD, 0);
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
    bluetooth.message.set(ESTIMATION, estimation);
    {
      {
        bluetooth.message.set(JOYSTICK_LEFT_X, leftJoystick.x.getValue());
        bluetooth.message.set(JOYSTICK_LEFT_Y, leftJoystick.y.getValue());
        bluetooth.message.set(JOYSTICK_LEFT_CLCK, leftJoystick.clck.getValue());
      }
      {
        bluetooth.message.set(JOYSTICK_RIGHT_X, rightJoystick.x.getValue());
        bluetooth.message.set(JOYSTICK_RIGHT_Y, rightJoystick.y.getValue());
        bluetooth.message.set(JOYSTICK_RIGHT_CLCK, rightJoystick.clck.getValue());
      }
    }
    bluetooth.send();
    blueLed.on();
  }
  // delay(LOOP_TIME);
}
