#include <Bluetooth.h>
#include <Keybull.h>
#include <Joystick.h>
#include <Led.h>
#include <Report.h>

#define loopTime 40
#define debugMode true // Pass to false for production

//                    RX  TX
SoftwareSerial Serial1(2, 3);
Bluetooth bluetooth(&Serial1);
Keybull keybull(4);
Joystick leftJoystick(A0, A1, 0);
Joystick rightJoystick(A2, A3, 0, true);
Button switcher(A6);
Led led1(11);
Led led2(12);
Led builtin(LED_BUILTIN);
Report report(&Serial, debugMode, 100);

void setup ()
{
  // Serial setup //
  {
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial.println("Serial communication's on...");
    Serial.println("Bluetooth communication's on...");
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
        report.ok++;
      }
      else
      {
        report.inv++;
        bluetooth.empty();
      }
      }
      else
      {
      report.ntr++;
      }
      report.print();
  }*/
  // Fetch data to json and send it //
  {
    bluetooth.json["switcher"] = switcher.getValue();
    //bluetooth.json["keybull"] = keybull.getValue(); // Uncomment when keypad is plugged in
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
    serializeJsonPretty(bluetooth.json, Serial);  // This line is for debug only. Due to bug with the arduino nano, you can uncomment this line and it will work.
  }
  delay(loopTime);
}
