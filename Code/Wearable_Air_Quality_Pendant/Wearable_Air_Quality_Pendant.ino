/*
  =======================================================================
  Project: Air Quality Monitor with TVOC Index using SGP40 Sensor

  Features:
  - Reads Total Volatile Organic Compounds (TVOC) index from SGP40 sensor
  - Indicates air quality using LEDs (Red, Green) with smooth fading effect
  - Plays RTTTL tone alerts for different air quality levels via buzzer
  - Displays TVOC values via LPUART serial (PA2, PA3)
  - Shows error states with visual (LED) and audible (buzzer) indication

  Working:
  - SGP40 sensor periodically reads TVOC index
  - Based on the index value, system classifies air as:
      0–200   → Good (Soft tone, Green LED)
      201–300 → Warning (Warn tone, Yellow LED)
      301–500 → Alarm (Alarm tone, Red LED)
  - If TVOC exceeds 500 or sensor fails, Error tone & Red LED flash
  - Scheduler ensures non-blocking periodic sensor read, LED updates, and sound
  =======================================================================
*/

#include <DFRobot_SGP40.h> // need to install
#include <HardwareSerial.h>
#include <Arduino.h>
#include <PlayRtttl.hpp> // need to install
#include <ptScheduler.h> // need to install

// LED and Buzzer pin definitions
#define RED_PIN PB3
#define GREEN_PIN PB4
#define BUZZER_PIN PB0

// Define serial on PA3 (RX) and PA2 (TX)
HardwareSerial MyLPUART1(PA3, PA2);

// Create SGP40 sensor object
DFRobot_SGP40 mySgp40;

// Variables for tone request and fading effect
int tvoc = 0;
int toneRequest = -1;
int fadeStep = 0;

// Time tracking variables for non-blocking operations
unsigned long prevLedTime = 0;
unsigned long prevToneTime = 0;

// Define RTTTL tone melodies
const char softTone[] = "Soft:d=8,o=5,b=100:g";
const char warnTone[] = "Warn:d=4,o=5,b=120:c,e,g";
const char alarmTone[] = "Alarm:d=16,o=5,b=180:c6,c6,c6";
const char startTone[] = "Start:d=8,o=6,b=120:c,e,g";
const char errorTone[] = "Error:d=4,o=5,b=160:c,c,r,c";

// Enum to associate tone types with time intervals
enum ToneType {
  TONE_SOFT = 5000,
  TONE_WARN = 2000,
  TONE_ALARM = 500,
  TONE_START = 1000,
  TONE_ERROR = 200
};

// Task scheduler for non-blocking periodic functions
ptScheduler readSensorTask(PT_TIME_1S);   // Sensor reading every 1 second
ptScheduler lightTask(PT_TIME_100MS);     // LED update task
ptScheduler soundTask(PT_TIME_100MS);     // Buzzer task

// Function declarations
void playTone(ToneType tone);
void updateLedFade(ToneType tone);
void showErrorState();
void readAndEvaluateAir();

void setup() {
  MyLPUART1.begin(115200);

  // Initialize output pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  MyLPUART1.println("sgp40 is starting, the reading can be taken after 10 seconds...");

  // Startup indication
  updateLedFade(TONE_START);
  playTone(TONE_START);

  // Initialize sensor
  if (!mySgp40.begin(10000)) {
    MyLPUART1.println("Failed to init chip. Please check connection.");
    showErrorState();
    while (true);  // Stop execution on failure
  }

  MyLPUART1.println("sgp40 initialized successfully!");
}

void loop() {
  // Periodically read sensor
  if (readSensorTask.call()) {
    readAndEvaluateAir();
  }

  // Update LED fading effect based on tone
  if (lightTask.call() && toneRequest != -1) {
    updateLedFade((ToneType)toneRequest);
  }

  // Play tone based on tone request
  if (soundTask.call() && toneRequest != -1 && toneRequest != TONE_SOFT) {
    playTone((ToneType)toneRequest);
  }
}

// Reads TVOC value and determines air quality level
void readAndEvaluateAir() {
  tvoc = mySgp40.getVoclndex();

  MyLPUART1.print("vocIndex = ");
  MyLPUART1.println(tvoc);

  if (tvoc <= 200) {
    toneRequest = TONE_SOFT;
  } else if (tvoc <= 300) {
    toneRequest = TONE_WARN;
  } else if (tvoc <= 500) {
    toneRequest = TONE_ALARM;
  } else {
    MyLPUART1.println("Invalid TVOC value.");
    showErrorState();
  }
}

// Smooth LED fading based on tone type
void updateLedFade(ToneType tone) {
  static const int maxBrightness = 255;
  static const int fadeSpeed = 5;
  static const int fadeDelay = 10;

  unsigned long currentMillis = millis();
  if (currentMillis - prevToneTime < tone) return;
  prevToneTime = currentMillis;

  int redTarget = 0, greenTarget = 0;

  // Set target brightness levels based on tone
  switch (tone) {
    case TONE_SOFT: greenTarget = maxBrightness; break;                  // Green only
    case TONE_WARN: redTarget = greenTarget = maxBrightness; break;     // Yellow (Red + Green)
    case TONE_ALARM:
    case TONE_ERROR: redTarget = maxBrightness; break;                  // Red only
    default: break;
  }

  // Fade in
  for (int brightness = 0; brightness <= 255; brightness += fadeSpeed) {
    analogWrite(RED_PIN, (brightness * redTarget) / 255);
    analogWrite(GREEN_PIN, (brightness * greenTarget) / 255);
    delay(fadeDelay);
  }

  // Fade out
  for (int brightness = 255; brightness >= 0; brightness -= fadeSpeed) {
    analogWrite(RED_PIN, (brightness * redTarget) / 255);
    analogWrite(GREEN_PIN, (brightness * greenTarget) / 255);
    delay(fadeDelay);
  }

  // Reset if loop exceeds max fade cycle
  fadeStep += fadeSpeed;
  if (fadeStep > 510) {
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, 0);
  }
}

// Play a tone melody corresponding to the tone type
void playTone(ToneType tone) {
  unsigned long currentMillis = millis();
  if (currentMillis - prevLedTime < tone) return;
  prevLedTime = currentMillis;

  switch (tone) {
    case TONE_SOFT:  playRtttlBlockingPGM(BUZZER_PIN, softTone); break;
    case TONE_WARN:  playRtttlBlockingPGM(BUZZER_PIN, warnTone); break;
    case TONE_ALARM: playRtttlBlockingPGM(BUZZER_PIN, alarmTone); break;
    case TONE_START: playRtttlBlockingPGM(BUZZER_PIN, startTone); break;
    case TONE_ERROR: playRtttlBlockingPGM(BUZZER_PIN, errorTone); break;
  }
}

// Show error condition using LED and buzzer
void showErrorState() {
  toneRequest = TONE_ERROR;
  soundTask.enable();
}
