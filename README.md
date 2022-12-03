# Bull

This is the remote control code from the Ingenium team.

The INO file depends of the following list of libraries:
- [Bluetooth](github.com/IngeniumTeam/Bluetooth)
- [Keybull](github.com/IngeniumTeam/Keybull)
- [Joystick](github.com/IngeniumTeam/Joystick)
- [Led](github.com/IngeniumTeam/Led)
- [Report](github.com/IngeniumTeam/Report)

The hardware consists of an Arduino Nano board, two double-axes potentiometers, three LEDs, a back/foward switch, a on/off switch and a 4x3 matrix keypad. Communication is ensured by an HC-05 bluetooth module. All messages are encoded in JSON.

JSON template format:

`{
  "switch": true,
  "joysticks": {
    "left": {
      "x": 512,
      "y": 512,
      "clck": true
    },
    "right": {
      "x": 512,
      "y": 512,
      "clck": false
    }
  }
}`