#include <Bluetooth.h>
#include <Joystick.h>
#include <Led.h>
#include <Report.h>
#include <Keypad.h>

#define loopTime 10
#define debugMode false // Pass to false for production

//                    RX  TX
SoftwareSerial Serial1(2, 3);
Bluetooth bluetooth(&Serial1);
Joystick leftJoystick(A0, A1, A2, false, false, 512, 512, 50, 50);
Joystick rightJoystick(A3, A4, A5, false, false, 512, 512, 50, 50);
Button switcher(A6);
Led led1(11);
Led led2(12);
Led builtin(LED_BUILTIN);
//Report report(&Serial, debugMode, 100);

int fromPin = 4;
char keys[4][3] = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } };
byte rowPins[4] = { 5, 10, 9, 7 };
byte colPins[3] = { 6, 4, 8 };
byte rows = 4;
byte cols = 3;
Keypad keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

void setup ()
{
  // Serial setup //
  {
    Serial.begin(9600);
    Serial1.begin(9600);
#if debugMode
    Serial.println("Serial communication's on...");
    Serial.println("Bluetooth communication's on...");
    Serial.println("Debug mode's on...");
#endif
  }
}

void loop ()
{
  // Receive data //
  /*{
    if (bluetooth.receive())
    {
      if (bluetooth.lastError == DeserializationError::Ok)
      {
        //report.ok++;
      }
      else
      {
        //report.inv++;
        bluetooth.empty();
      }
    }
    else
    {
      //report.ntr++;
    }
    //report.print();
    }*/
  // Fetch data to json and send it via bluetooth //
  {
    bluetooth.json["switch"] = switcher.getValue();
    bluetooth.json["keypad"] = keypad.getKey();
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
#if debugMode
    serializeJsonPretty(bluetooth.json, Serial);
#endif
  }
  delay(loopTime);
}
