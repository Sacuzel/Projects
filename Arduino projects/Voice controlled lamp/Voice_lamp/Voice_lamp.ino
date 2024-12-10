#include <Servo.h>
#include <arduinoFFT.h>  // This is for frequency detection

// FFT variables
#define SAMPLES 128
#define SAMPLING_FREQUENCY 4000  // This can be changed based on the mic in use
// Nyquist theorem suggest twice as high as the peak frequency, henceforth 3500 Hz.

// Servo and LED variables
Servo myServo;
int ledPin = 3;
int noiseDetectedLedPin = 4;
bool ledState = false;     // Lamp led's state
bool lampChanged = false;  // Is true if lamp state changed by clapping
int noisePin = A0;         // Microphone analog input
int clapCount = 0;         // Keep track of the claps
unsigned long lastClapTime = 0;
unsigned long clapWindow = 1000;  // Claps should happen within given seconds
int servoPosition = 90;           // Start the servo motor at the center
int THRESHOLD = 5;

// FFT variables
ArduinoFFT<double> FFT = ArduinoFFT<double>();
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned int samplingPeriod;
unsigned long microSeconds;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(noiseDetectedLedPin, OUTPUT);
  myServo.attach(9);
  myServo.write(90);     // Bring the lamp to center
  Serial.begin(115200);  // Higher baud rate
}

void loop() {
  // Adjust the detected sound value
  double mn = 1024;
  double mx = 0;

  for (int i = 0; i < 100; ++i) {
    double val = analogRead(noisePin);
    mn = min(mn, val);
    mx = max(mx, val);
  }

  double delta = mx - mn;  // This is the "volume"
   Serial.println(delta); // DEBUG PRINT

  // Check for the claps
  if (detectClap(delta)) {
    clapCount++;
    lastClapTime = millis();
  }

  // If certain number of claps is exceeded within time window
  if (millis() - lastClapTime <= clapWindow && clapCount == 2 && !ledState) {
    digitalWrite(ledPin, HIGH);  // Turn on the led
    ledState = true;
    lampChanged = true;
    clapCount = 0;  // Reset counter
  }

  else if ((millis() - lastClapTime <= clapWindow && clapCount == 2 && ledState)) {
    digitalWrite(ledPin, LOW);  // Turn off the led
    ledState = false;
    clapCount = 0;  // Reset counter
    lampChanged = true;
  }

  // If claps don't happen within sound window, ignore individual claps
  if (millis() - lastClapTime > clapWindow) {
    clapCount = 0;
    lastClapTime = 0;
  }

  double whistleFreq = computeFrequency();

  // Whistle detection logic
  if (whistleFreq >= 100000 && whistleFreq <= 200000) {
    debugFlash();
    moveServoLeft();  // Low whistle turns the servo to the left

  } else if (whistleFreq >= 300000 && whistleFreq <= 400000) {
    debugFlash();
    moveServoRight();  // High whistle turns it to the right
  }
}

double computeFrequency() {
  samplingPeriod = round(1000000 * (1.0 / SAMPLING_FREQUENCY));

  // Now we will do some Fourier transforms
  // Sample audio signal
  for (int i = 0; i < SAMPLES; i++) {
    microSeconds = micros();

    vReal[i] = analogRead(noisePin);  // Read the microphone input (SCAN HAPPENS HERE)

    vImag[i] = 0;

    while (micros() < (microSeconds + samplingPeriod)) {
      // Wait for next sample
    }
  }

  // Perform the FFT (Fast Fourier Transform, MAGIC!)
  FFT.windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.complexToMagnitude(vReal, vImag, SAMPLES);

  // Detect the dominant frequency, this should be the whistle
  double peakFrequencyRaw = FFT.majorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
  double peakFrequency = peakFrequencyRaw * 100;

  Serial.println(peakFrequency);  // Print peak freq for debugging

  return peakFrequency;
}


bool detectClap(double delta) {
  int noiseLevel = delta;
  if (noiseLevel >= THRESHOLD) {
    debugFlash();
    return true;  // Confirm detection
  }
  return false;  // No clap detected
}


void moveServoLeft() {
  servoPosition = max(servoPosition - 10, 0);  // Servo should not go below 0 degrees
  myServo.write(servoPosition);
  Serial.println("Servo moved left");
  delay(200);  // Prevent servo from jittering
}

void moveServoRight() {
  servoPosition = min(servoPosition + 10, 180);  // Neither should it go above 180 degrees
  myServo.write(servoPosition);
  Serial.println("Servo moved right");
  delay(200);  // Prevent servo from jittering
}

// Flash the red LED to prove that noise is detected
void debugFlash() {
  digitalWrite(noiseDetectedLedPin, HIGH);
  delay(25);
  digitalWrite(noiseDetectedLedPin, LOW);
}
