#include <Bluetooth.h>
#include <Keybull.h>
#include <Joystick.h>
#include <Led.h>

#define loopTime 80

SoftwareSerial Serial1(2, 3);

Bluetooth bluetooth(&Serial1);
Keybull keybull(4);
Joystick leftJoystick(A0, A1, A2);
Joystick rightJoystick(A3, A4, A5);
Button switcher(A6);
Led led1(11);
Led led2(12);
Led builtin(LED_BUILTIN);

void setup ()
{
  // Serial setup //
  {
    Serial.begin(9600);
    Serial.println("Serial communication's on...");
    Serial1.begin(9600);
  }
}

void loop ()
{
  // Fetch data to json and send it //
  {
    bluetooth.json["switcher"] = switcher.getValue();
    //bluetooth.json["keybull"] = keybull.getValue();
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
  }
  //led1.on();
  //led2.off();
  //led3.on();
  delay(loopTime);
}
