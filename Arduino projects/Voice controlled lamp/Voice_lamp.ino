#include <Servo.h>
#include  "arduinoFFT.h" // This is for frequency detection

// Servo and LED variables
Servo myServo;
int ledPin = 3;
int noiseDetectedPin = 4;
bool ledState = false;
int noisePin = A0; // Microphone analog input
int clapCount = 0; // Keep track of the claps
unsigned long lastClapTime = 0;
unsigned long clapWindow = 1000; // Claps should happen within a second
int servoPosition = 90; // Start the servo motor at the center
int THRESHOLD = 300;

// FFT variables
#define SAMPLES 128
#define SAMPLING_FREQUENCY 3500 // It should cover the user's whistle range (in my case 900–1700 Hz)
// Nyquist theorem suggest twice as high as the peak frequency, henceforth 3500 Hz.
arduinoFFT FFT = arduinoFFT();
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned int samplingPeriod;
unsigned long microSeconds;

void setup() {
    // FFT setup
    samplingPeriod = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
    pinMode(ledPin, OUTPUT);
    pinMode(noiseDetectedPin, OUTPUT);
    myServo.attach(9);
    myServo.write(90); // Bring the lamp to center
    Serial.begin(115200); // Higher baud rate
}

void loop() {
  // Check for the claps
  if (detectClap()) {
    clapCount++;
    lastClapTime = millis();

    debugFlash();
  }


  // If certain number of claps is exceeded within time window
  if (millis() - lastClapTime > clapWindow && clapCount == 2 && !ledState) {
    digitalWrite(ledPin, HIGH); // Turn on the led
    ledState = true;
  }

  else if ((millis() - lastClapTime > clapWindow && clapCount == 2 && ledState)){
    digitalWrite(ledPin, LOW); // Turn off the led
    }

    clapCount = 0; // Reset counter
  

  // Now we will do some Fourier transforms
  // Sample audio signal
  for (int i = 0; i < SAMPLES; i++) {
    microSeconds = micros();
    vReal[i] = analogRead(noisePin); // Read the microphone input (SCAN HAPPENS HERE)
    vImag[i] = 0;

    while (micros() < (microSeconds + samplingPeriod)) {
      // Wait for next sample
    }
  }

  // Perform the FFT (Fast Fourier Transform, MAGIC!)
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  // Detect the dominant frequency, this should be the whistle
  double peakFrequency = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

  // Whistle detection logic
  if (peakFrequency >= 900 && peakFrequency <= 1100) {
    debugFlash();
    moveServoLeft(); // Low whistle turns the servo to the left
  } else if (peakFrequency >= 1600 && peakFrequency <= 1700) {
    debugFlash();
    moveServoRight(); // High whistle turns it to the right
  }
}

bool detectClap() {
  // Replace with your noise detection logic
  int noiseLevel = analogRead(noisePin);
  return noiseLevel > THRESHOLD; // Adjust THRESHOLD based on testing
}

void moveServoLeft() {
  servoPosition = max(servoPosition - 10, 0); // Servo should not go below 0 degrees
  myServo.write(servoPosition);
  Serial.println("Servo moved left");
}

void moveServoRight() {
  servoPosition = min(servoPosition + 10, 180); // Neither should it go above 180 degrees
  myServo.write(servoPosition);
  Serial.println("Servo moved right");
}

// Flash the red LED to prove that noise is detected
void debugFlash() {
    digitalWrite(noiseDetectedPin, HIGH);
    delay(100);
    digitalWrite(noiseDetectedPin, LOW);
}





