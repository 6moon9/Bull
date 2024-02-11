#include <Bluetooth.h>
#include <SoftwareSerial.h>
#include <Joystick.h>
#include <Led.h>
#include <Keypad.h>

#define LOOP_TIME 20
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
Bluetooth bluetooth(&Serial1, sizes, NUM_VALUES, '.');
Joystick leftJoystick(A1, A0, A2, true, false, 512, 512, 50, 50, 0, 511);
Joystick rightJoystick(A4, A3, A5, true, false, 512, 512, 50, 50, 0, 511);
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

int estimation = 0;
int key = 0;

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
  //int start = millis();
  bluetooth.message.set(SWITCH, switcher.getAnalogValue() > 512);
  if (bluetooth.message.get(SWITCH)) {
    whiteLed.on();
  }
  else {
    whiteLed.off();
  }
  int currentKey = keypad.getKey();
  if (currentKey == 12) {
    redLed.on();
    estimation = 0;
    currentKey = keypad.getKey();
    while (currentKey != 12 && ((int)log10(estimation) + 1) < 3) {
      if (currentKey >= 1 && currentKey <= 9) {
        estimation = estimation * 10 + currentKey;
      }
      if (currentKey == 11) {
        estimation *= 10;
      }
      currentKey = keypad.getKey();
    }
    redLed.off();
  }
  else if (currentKey != 0) {
    key = currentKey;
  }
  bluetooth.message.set(KEYPAD, key);
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
  //Serial.println(millis() - start);
  delay(LOOP_TIME);
#if DEBUG
  Serial.print(bluetooth.message.get(SWITCH)); Serial.print(" ");
  Serial.print(bluetooth.message.get(KEYPAD)); Serial.print(" ");
  Serial.print(bluetooth.message.get(ESTIMATION)); Serial.print(" ");
  Serial.print(bluetooth.message.get(JOYSTICK_LEFT_X)); Serial.print(" ");
  Serial.print(bluetooth.message.get(JOYSTICK_LEFT_Y)); Serial.print(" ");
  Serial.print(bluetooth.message.get(JOYSTICK_LEFT_CLCK)); Serial.print("   ");
  Serial.print(bluetooth.message.get(JOYSTICK_RIGHT_X)); Serial.print(" ");
  Serial.print(bluetooth.message.get(JOYSTICK_RIGHT_Y)); Serial.print(" ");
  Serial.print(bluetooth.message.get(JOYSTICK_RIGHT_CLCK)); Serial.print(" ");
  Serial.println();
#endif
}
