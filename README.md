
# Smart Walking Aid for Visually Impaired (Arduino Uno)

This project is designed to assist visually impaired individuals by detecting nearby obstacles, moisture, and floor level changes using sensors. The system gives audio alerts through a buzzer to help guide the user safely.

## Features
### Obstacle Detection: Uses ultrasonic sensors to detect objects in front, left, and right.

### Moisture Detection: Alerts if water is detected on the floor.

### Pit Detection: Warns the user about floor drops or holes.

### Directional Audio: Plays different sound patterns based on the situation.

## Components Used

Arduino Uno

Ultrasonic Sensors (2x)

Moisture Sensor

Servo Motor

Buzzer

Wires and Breadboard

##How to Use

Connect the components as per the pin configuration in the code.

Upload the sketch to your Arduino Uno using the Arduino IDE.

Open the Serial Monitor at 9600 baud rate to see debug info.

When powered, the device will scan surroundings and play sounds as alerts.


## Dependencies
```
Servo.h (built-in Arduino library)

pitches.h (custom tone definitions, must be included in the same folder)

Pin Configuration (Quick Ref)
Moisture Sensor → A0

Front Ultrasonic Sensor → Trig: 9, Echo: 10

Floor Ultrasonic Sensor → Trig: 4, Echo: 5

Servo Motor → Pin 11

Buzzer → Pin 8

Audio Output LEDs → Pins 12, 13
```