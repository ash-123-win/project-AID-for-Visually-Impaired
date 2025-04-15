#include "pitches.h"
#include <Servo.h>

#define NOTE_REST 0

const int Moisture_Sens_Pin = 0;//Analog pin 0
const int Allowed_Moisture_Value = 900;// If moisture content is detected the analog values decrease from 1023

const int trigPin_ObjSensor = 9;  // Trigger pin of the Ultrasonic sensor, which detects the Object distance
const int echoPin_ObjSensor = 10; // Echo pin 

const int trigPin_FloorSensor = 4; // Trigger Pin of the Ultrasonic sensor, which detects the Floor distance
const int echoPin_FloorSensor = 5;

const int Zero_deg_Audio = 12;
const int Ninty_deg_Audio = 0;
const int Pi_deg_Audio = 13;

float object_distance_cm = 0.0;

Servo servo_motor;  // create servo object to control a servo
// twelve servo objects can be created on most boards

// Notes in the danger alert melody:
int dangerMelody[] = {
  NOTE_A5, NOTE_REST, NOTE_A5, NOTE_REST, NOTE_A5, NOTE_REST,
  NOTE_A5, NOTE_REST, NOTE_A5, NOTE_REST, NOTE_A5
};

// Note durations for the danger alert melody:
int dangerNoteDurations[] = {
  8, 8, 8, 8, 8,
  8, 8, 8, 8, 8
};

// Notes in the vibration sound melody. Used as alert for floor distance sensing
int vibrationMelody[] = {
  NOTE_A4, NOTE_E4, NOTE_A4, NOTE_E4,
  NOTE_A4, NOTE_E4, NOTE_A4, NOTE_E4,
  NOTE_A4, NOTE_E4, NOTE_A4, NOTE_E4,
  NOTE_A4, NOTE_E4, NOTE_A4, NOTE_E4
};

// Note durations for the vibration sound melody:
int vibrationNoteDurations[] = {
  8, 8, 8, 8,
  8, 8, 8, 8,
  8, 8, 8, 8,
  8, 8, 8, 8
};

// Notes in the warning 1 melody, Used as alert for moisture sensing
int warning1Melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5,
  NOTE_G5, NOTE_REST, NOTE_REST, NOTE_REST
};

// Note durations for Warning 1 melody:
int warning1NoteDurations[] = {
  4, 4, 4, 4,
  4, 8, 8, 8
};

// New melody for Warning 2:
int warning2Melody[] = {
  NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4,
  NOTE_A4, NOTE_REST, NOTE_REST, NOTE_REST
};

// Note durations for Warning 2 melody:
int warning2NoteDurations[] = {
  4, 4, 4, 4,
  4, 8, 8, 8
};


void setup() {
  
  
  // Play the vibration sound melody:
  Serial.begin(9600);

  pinMode(trigPin_ObjSensor, OUTPUT);
  pinMode(echoPin_ObjSensor, INPUT);

  pinMode(trigPin_FloorSensor, OUTPUT);
  pinMode(echoPin_FloorSensor, INPUT);

  servo_motor.attach(11);  // attaches the servo on pin 11 to the servo object
}


void loop() {

Check_FloorDistance_And_Alert();
Check_Moisture_And_Alert();
Check_Surroundings();
 
}

//++++++++++++ Function 1 for using the buzzer ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void playMelody(int melody[], int noteDurations[], int melodySize) {
  // Iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < melodySize; thisNote++) {
    // Calculate the note duration, take one second divided by the note type.
    // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];

    // Play the note using the tone function with the specified frequency and duration:
    tone(8, melody[thisNote], noteDuration);

    // To distinguish the notes, set a minimum time between them.
    // The note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // Stop the tone playing:
    noTone(8);
  }
}

//++++++++++++ Function 2 for measuing pits on the floor ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Check_FloorDistance_And_Alert (void) {

  // Start measuring distance
  digitalWrite(trigPin_FloorSensor, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_FloorSensor, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_FloorSensor, LOW);
  long duration_FloorSensor = pulseIn(echoPin_FloorSensor, HIGH);
  float floor_distance_cm = duration_FloorSensor * 0.034 / 2; 
  
  // Only for testing purpose
  Serial.print("Floor Distance: ");
  Serial.print(floor_distance_cm);
  Serial.println(" cm");

  if (floor_distance_cm >= 50) {
    playMelody(vibrationMelody, vibrationNoteDurations, sizeof(vibrationMelody) / sizeof(vibrationMelody[0]));
  }

}


//+++++++++++++ Function 3 Checking for moisture content on the floor ++++++++++++++++++++++++++++++++++++++++++++++++++

void Check_Moisture_And_Alert (void) {

  // Reading the analog value from the moisture sensor
  int moisture_Content = analogRead(Moisture_Sens_Pin);
  Serial.print("Moisture Analog Value:");
  Serial.println(moisture_Content);
  if(moisture_Content < Allowed_Moisture_Value){
       
       playMelody(warning1Melody, warning1NoteDurations, sizeof(warning1Melody) / sizeof(warning1Melody[0]));
       
  } else {
       
  }
}


//+++++++++++++ Functions 4 Checking for objects in the surroundings ++++++++++++++++++++++++++++

void Check_Surroundings(void) {

  Check_Front(90, Zero_deg_Audio); // Checking for objects directly in the front

} 

//+++++++++++++ Functions 5 Checking objects directly in the front ++++++++++++++++++++++++++++

void Check_Front(int angle, int audio_pin) {

  servo_motor.write(angle); // Servo facing front 90 degrees (front)
  delay(1500);

  // Start measuring distance of the object in the front
  digitalWrite(trigPin_ObjSensor, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_ObjSensor, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_ObjSensor, LOW);
  long duration_ObjSensor = pulseIn(echoPin_ObjSensor, HIGH);

  object_distance_cm = duration_ObjSensor * 0.034 / 2;  // Calculating the distance in centimeters

  Serial.print("Object Distance: ");
  Serial.print(object_distance_cm);
  Serial.println(" cm");
 
  if ((angle == 90) && (object_distance_cm <= 50)) {
     // // Something detected, Play the danger alert melody
     playMelody(dangerMelody, dangerNoteDurations, sizeof(dangerMelody) / sizeof(dangerMelody[0]));
     Check_Right(0, Zero_deg_Audio); // Checking for objects in the right side
  } else {
    // Inform to turn right (audio played)
     digitalWrite(audio_pin, LOW); 
  }
}
//+++++++++++++ Function 6 Checking for objects in the right side ++++++++++++++++++++++++++++

void Check_Right(int angle, int audio_pin) {

  servo_motor.write(angle); // Servo facing front 0 degrees (right)
  delay(1500);

  // Start measuring distance of the object in the rightside
  digitalWrite(trigPin_ObjSensor, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_ObjSensor, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_ObjSensor, LOW);
  long duration_ObjSensor = pulseIn(echoPin_ObjSensor, HIGH);

  object_distance_cm = duration_ObjSensor * 0.034 / 2;  // Calculating the distance in centimeters

  Serial.print("Right side Object Distance: ");
  Serial.print(object_distance_cm);
  Serial.println(" cm");
 
  if ((angle == 0) && (object_distance_cm <= 50)) {
     // Something detected, Checking for objects in the left side
     Check_Left(180, Pi_deg_Audio);
  } else {    
  // Turn right audio is played here
     digitalWrite(audio_pin, HIGH); // Turn on LED if object is detected
     delay(50);
     digitalWrite(audio_pin, LOW); 
  }
}

//+++++++++++++ Function 7 Checking for objects in the left side ++++++++++++++++++++++++++++

void Check_Left(int angle, int audio_pin) {

  servo_motor.write(angle); // Servo facing front 180 degrees (left)
  delay(1500);

  // Start measuring distance of the object in the leftside
  digitalWrite(trigPin_ObjSensor, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_ObjSensor, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_ObjSensor, LOW);
  long duration_ObjSensor = pulseIn(echoPin_ObjSensor, HIGH);

  object_distance_cm = duration_ObjSensor * 0.034 / 2;  // Calculating distance in centimeters

  Serial.print("Right side Object Distance: ");
  Serial.print(object_distance_cm);
  Serial.println(" cm");
 
  if ((angle == 180) && (object_distance_cm <= 50)) {
    
    // Something detected on the leftside too, 
    playMelody(warning2Melody, warning2NoteDurations, sizeof(warning2Melody) / sizeof(warning2Melody[0]));
 
  } else {
    
  // Turn left audio is played here
     digitalWrite(audio_pin, HIGH); // Turn on LED if object is detected
     delay(50);
     digitalWrite(audio_pin, LOW);

  }
}
