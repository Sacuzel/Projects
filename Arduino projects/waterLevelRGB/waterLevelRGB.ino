/* This program measures water level and uses an RGB LED to indicate different water levels
based on predetermined thresholds. A sound alert is also given when water level exceeds predetermined limits.

Author: Abdurrahman Faig
*/

/* These values should be changed based on the readings from the water level sensor*/
int emptyThreshold = 0;
int lowerThreshold = 95;
int upperThreshold = 105;
int criticalThreshold = 110;

// Sensor pins
#define sensorPower 7
#define sensorPin A1

// Buzzer pins
int buzzpin = 9;
int buzzState = LOW;

// Value for storing water level
int val = 0;

// Declare pins to which RGB LED's wires are connected
int redPin = 2;
int bluePin = 3;
int greenPin = 4;

bool openingPlayed = false;

void setup() {
	Serial.begin(115200);
	pinMode(sensorPower, OUTPUT);
	digitalWrite(sensorPower, LOW);
	
	// Set LED wires as OUTPUTs
	pinMode(redPin, OUTPUT);
	pinMode(bluePin, OUTPUT);
	pinMode(greenPin, OUTPUT);
  pinMode(buzzpin, OUTPUT); // Remember the buzzer


	// Initially turn off the RGB LED
	digitalWrite(redPin, LOW);
	digitalWrite(bluePin, LOW);
	digitalWrite(greenPin, LOW);
}

void loop() {

  if(!openingPlayed){
    // Play starting melody
    tone(buzzpin, 440, 250);
    delay(250);
    tone(buzzpin, 784, 250);
    delay(250);
    tone(buzzpin, 587, 250);
    delay(250);
    noTone(buzzpin);

    openingPlayed = true;
  }
	int level = readSensor();

	if (level <= emptyThreshold) {
		Serial.println("Water Level: Empty");
    Serial.println(level);
		setColor(255,  255, 255); // White Color
	}
	else if (level > 0 && level <= lowerThreshold) {
		Serial.println("Water Level: Low");
    Serial.println(level);
		setColor(0,  255, 0); // Green Color

    // Beep for Low level
    tone(buzzpin, 587, 250); // D4
    delay(250); 
    noTone(buzzpin); // Stop the tone

	}
	else if (level > lowerThreshold && level <= upperThreshold) {
		Serial.println("Water Level: Medium");
    Serial.println(level);
		setColor(255,  255, 0); // Yellow Color

    // Beep for Medium level
    tone(buzzpin, 440, 200); // A4
    delay(200);
    noTone(buzzpin); // Stop the tone
    delay(100); // Short pause
    tone(buzzpin, 440, 200); // A4
    delay(200);
    noTone(buzzpin); // Stop the tone
    delay(100); // Short pause
    tone(buzzpin, 440, 200); // A4
    delay(200);
    noTone(buzzpin); // Stop the tone
  }

	else if (level > upperThreshold && level <= criticalThreshold) {
		Serial.println("Water Level: High");
    Serial.println(level);
		setColor(255, 0, 0); // Red Color

    // Beep for High level
    tone(buzzpin, 494, 100); // B4
    delay(100);
    noTone(buzzpin); // Stop the tone
    delay(50); // Short pause
    tone(buzzpin, 494, 100); // B4
    delay(100);
    noTone(buzzpin); // Stop the tone
    delay(50); // Short pause
    tone(buzzpin, 494, 100); // B4
    delay(100);
    noTone(buzzpin); // Stop the tone
	}

  else if (level > criticalThreshold) {
		Serial.println("Water Level: Critical");
    Serial.println(level);

   // S.O.S for the critical level

    // Three short tones
    for (int i = 0; i < 3; i++) {
      tone(buzzpin, 494, 100); // B4
      setColor(255, 0, 0); // Red Color
      delay(100);
      noTone(buzzpin);
      setColor(0,0,0); // Blink!
      delay(100);
    }

    // Three long tones
    for (int i = 0; i < 3; i++) {
      tone(buzzpin, 494, 300); // B4
      setColor(255, 0, 0); // Red Color
      delay(300);
      noTone(buzzpin);
      setColor(0,0,0); // Blink!
      delay(200);
    }

    // Three short tones
    for (int i = 0; i < 3; i++) {
      tone(buzzpin, 494, 100); // B4
      setColor(255, 0, 0); // Red Color
      delay(100);
      noTone(buzzpin);
      setColor(0,0,0); // Blink!
      delay(100);
    }
	}
	delay(1000);
}

// This is a function used to get the reading
int readSensor() {
	digitalWrite(sensorPower, HIGH);
	delay(25);
	val = analogRead(sensorPin);
	digitalWrite(sensorPower, LOW);
	return val;
}

// This function sets the color for the RGB LED
void setColor(int redValue, int greenValue,  int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin,  greenValue);
  analogWrite(bluePin, blueValue);
}
