#include <Servo.h>
#include <arduinoFFT.h>  // This is for frequency detection

// FFT variables
#define SAMPLES 128
#define SAMPLING_FREQUENCY 3500  // This can be changed based on the mic in use
// Nyquist theorem suggest twice as high as the peak frequency, henceforth 3500 Hz.
int rawData[SAMPLES];  // Array for storing ADC readings


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
int THRESHOLD = 6;

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
                           // Serial.println(delta); // DEBUG PRINT

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

  double whistleFreq = computeFrequency(delta);  // Use FFT to separate dominant frequency
  // Serial.println(peakFrequency);  // Print peak freq for debugging


  // First create the rawData.
  sampleMicrophone();
  // Validate the rawData
  /*{
    for (int i = 0; i < SAMPLES; i++) {
      Serial.print("rawData[");
      Serial.print(i);
      Serial.print("]: ");
      Serial.println(rawData[i]);
    } 
  }*/

  // Do the autocorrelation
  double autoCorrelatedWhistle = autoCorrelate(rawData, SAMPLES);
  Serial.println(autoCorrelatedWhistle);  // DEBUG PRINT

  // Whistle detection logic
  if (whistleFreq >= 100000 && whistleFreq <= 200000) {
    debugFlash();
    moveServoLeft();  // Low whistle turns the servo to the left

  } else if (whistleFreq >= 300000 && whistleFreq <= 400000) {
    debugFlash();
    moveServoRight();  // High whistle turns it to the right
  }
}

double computeFrequency(double delta) {
  samplingPeriod = round(1000000 * (1.0 / SAMPLING_FREQUENCY));

  // Now we will do some Fourier transforms
  // Sample audio signal
  for (int i = 0; i < SAMPLES; i++) {
    microSeconds = micros();

    double rawInput = analogRead(noisePin);  // Read the microphone input (SCAN HAPPENS HERE)
    vReal[i] = rawInput - 512;               // Center the signal

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
  double peakFrequency = peakFrequencyRaw;
  return peakFrequency;
}

// Alternative to FFT is autocorrelation
float autoCorrelate(int *rawData, int len) {

  int i, k;
  long sum, sum_old;
  int thresh = 0;
  float freq_per = 0;
  byte pd_state = 0;

  sum = 0;
  pd_state = 0;
  int period = 0;
  // Autocorrelation
  for (i = 0; i < len; i++) {

    sum_old = sum;
    sum = 0;

    //sum += rawData[k] * rawData[k + i];
    for (k = 0; k < len - i; k++) sum += (rawData[k] - 128) * (rawData[k + i] - 128) / 256;

    // Peak Detect State Machine
    if (pd_state == 2 && (sum - sum_old) <= 0) {
      period = i;
      pd_state = 3;
    }
    if (pd_state == 1 && (sum > thresh) && (sum - sum_old) > 0) pd_state = 2;

    if (!i) {
      thresh = sum * 0.5;
      pd_state = 1;
    }
  }

  // Frequency identified in Hz
  freq_per = (float)SAMPLING_FREQUENCY / period;
  return freq_per;
}

// This function is mainly used for autocorrelation
void sampleMicrophone() {
  unsigned long samplingPeriod = 1000000 / SAMPLING_FREQUENCY;  // Microseconds
  unsigned long lastSampleTime = 0;

  for (int i = 0; i < SAMPLES; i++) {
    while (micros() - lastSampleTime < samplingPeriod) {
      // Wait until the next sampling period
    }
    rawData[i] = analogRead(A0);  // Read the microphone input
    lastSampleTime = micros();
  }
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
