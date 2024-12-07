#include <Servo.h> 

// variables
const int trigPin = 2;
const int echoPin  = 9;

long duration;
int  distance;

int buzzpin = 7;
int buzzState = LOW;

int ledRed = 3;
int ledGreen = 4;

int switchpin = 10;
int ledStatus = 8;

byte leds = 0;

bool canSpin = true;

unsigned long previousMillis = 0;

unsigned long detectedObjects = 0;

const long intervalFar = 250;
const long intervalClose = 50;
const long intervalIdle = 1250;

Servo myServo;

bool warnings = true;


void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  Serial.begin(9600);
  myServo.attach(12); 
  pinMode(buzzpin, OUTPUT);
  Serial.begin(9600);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(switchpin, INPUT);
  pinMode(ledStatus, OUTPUT);
}

// turning the radar on-off
void loop() {
  if (digitalRead(switchpin) == HIGH){
    digitalWrite(ledStatus, LOW);

    warnings = true; // Warn if power is off

    // Play starting sound

    tone(buzzpin, 440, 250);
    delay(250);
    tone(buzzpin, 784, 250);
    delay(250);
    tone(buzzpin, 587, 250);
    // Start the actual scan
    StartScan();
  }
  if (digitalRead(switchpin) == LOW){
    digitalWrite(ledStatus, HIGH);

    if(warnings){
      // Play closing sound
      tone(buzzpin, 440, 250);
      delay(250);
      tone(buzzpin, 523, 250);
      delay(250);
      tone(buzzpin, 587, 250);

      warnings = false;
    }
  }
}

// calculating the distance to a detected object
int calculateDistance(){ 
  digitalWrite(trigPin,  LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // reads echoPin and returns the sound wave travel time (ms)
  distance = duration * 0.034 / 2;
  return distance;
}

// Play a melody
void melody() {
  tone(buzzpin, 440); // A4
  delay(250);

  tone(buzzpin, 494); // B4
  delay(250);

  tone(buzzpin, 523); // C4
  delay(250);

  tone(buzzpin, 587); // D4
  delay(250);

  tone(buzzpin, 659); // E4
  delay(250);

  tone(buzzpin, 698); // F4
  delay(250);

  tone(buzzpin, 784); // G4
  delay(250);

  noTone(buzzpin);
  delay(250);
}

// start scanning for objects
void StartScan() {

  // If detected 5 objects, play a melody
  if (detectedObjects > 4) {
    melody();
    detectedObjects = 0;
  }
  // checking for every degree

  // first from right to left
  for (int i = 15; i <= 165; i++) {  
    myServo.write(i);
    delay(30);
    distance = calculateDistance();

    // checking if something is detected in the 50 cm range
    if (distance <= 50 && distance > 20) {
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= intervalFar) {
        previousMillis = currentMillis;

        detectedObjects++;

        if (buzzState == LOW) {
          buzzState = HIGH;
        } else {
          buzzState = LOW;
        }

        digitalWrite(buzzpin, buzzState);
        digitalWrite(ledRed, buzzState); 
        digitalWrite(ledGreen, HIGH); 
      }
    } else if (distance <= 20 && distance > 0) {
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= intervalClose) {
        previousMillis = currentMillis;

        detectedObjects++;


        if (buzzState == LOW) {
          buzzState = HIGH;
        } else {
          buzzState = LOW;
        }

        digitalWrite(buzzpin, buzzState);
        digitalWrite(ledRed, buzzState); 
        digitalWrite(ledGreen, HIGH); 
      }
    } else if (distance > 50) {
      digitalWrite(buzzpin, LOW);
      digitalWrite(ledGreen, LOW);
      digitalWrite(ledRed, HIGH); 
    }
      
    Serial.print(i); 
    Serial.print(","); 
    Serial.print(distance);  
    Serial.print(".");
  }

  // then from left to right
  for (int i = 165; i >= 15; i--) {  
    myServo.write(i);
    delay(30);
    distance  = calculateDistance();

    if (distance <= 50 && distance > 20) {
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= intervalFar) {
        previousMillis = currentMillis;

        detectedObjects++;


        if (buzzState == LOW) {
          buzzState = HIGH;
        } else {
          buzzState = LOW;
        }

        digitalWrite(buzzpin, buzzState);
        digitalWrite(ledRed, buzzState); 
        digitalWrite(ledGreen, HIGH); 
      }
    } else if (distance <= 20 && distance > 0) {
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= intervalClose) {
        previousMillis = currentMillis;

        detectedObjects++;


        if (buzzState == LOW) {
          buzzState = HIGH;
        } else {
          buzzState = LOW;
        }

        digitalWrite(buzzpin, buzzState);
        digitalWrite(ledRed, buzzState); 
        digitalWrite(ledGreen, HIGH); 
      }
    } else if (distance > 50) {
      digitalWrite(buzzpin, LOW);
      digitalWrite(ledGreen, LOW);
      digitalWrite(ledRed, HIGH); 
    }

    Serial.print(i);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");
  }
}