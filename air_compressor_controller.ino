#include "Compressor.h"

// === System-Wide Settings ===
const float pressureThresholdLow = 70.0;
const float pressureThresholdHigh = 120.0;
const unsigned long startupDelay = 7000; // Minimum time between starting compressors
const int maxRunningCompressors = 2;

// === Compressor Definitions ===
Compressor compressors[] = {
  Compressor("Compressor 1", A0, 2),
  Compressor("Compressor 2", A1, 3),
  Compressor("Compressor 3", A2, 4)
};
const int numCompressors = sizeof(compressors) / sizeof(compressors[0]);

// === Global State ===
unsigned long lastStartTime = 0; // Time the last compressor was started

void setup() {
  Serial.begin(9600);
  delay(2000); // Wait for sensors to stabilize

  for (int i = 0; i < numCompressors; i++) {
    compressors[i].begin();
  }

  Serial.println("System initialized. All compressors are OFF.");
}

void loop() {
  unsigned long now = millis();

  // 1. Update the state of each compressor based on its pressure
  for (int i = 0; i < numCompressors; i++) {
    compressors[i].update(pressureThresholdLow, pressureThresholdHigh);
  }

  // 2. Count how many compressors are currently running
  int compressorsRunning = 0;
  for (int i = 0; i < numCompressors; i++) {
    if (compressors[i].isOn()) {
      compressorsRunning++;
    }
  }

  // 3. Check if we can start a waiting compressor
  if (compressorsRunning < maxRunningCompressors && (now - lastStartTime >= startupDelay)) {
    // Find the first waiting compressor and turn it on
    for (int i = 0; i < numCompressors; i++) {
      if (compressors[i].isWaiting()) {
        compressors[i].turnOn();
        lastStartTime = now;
        break; // Start only one compressor per cycle
      }
    }
  }

  delay(100); // Loop pacing
}
