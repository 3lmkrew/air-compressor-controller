// === Pin Definitions ===
const int sensorPin1 = A0;
const int sensorPin2 = A1;
const int sensorPin3 = A2;

const int compressorPin1 = 2;
const int compressorPin2 = 3;
const int compressorPin3 = 4;

// === Compressor State Variables ===
bool compressor1State = false;
bool compressor2State = false;
bool compressor3State = false;

bool compressor1Waiting = false;
bool compressor2Waiting = false;
bool compressor3Waiting = false;

// === Timers ===
unsigned long lastStartTime = 0;                  // Time of last compressor activation (for staggering)
const unsigned long startupDelay = 7000;          // Delay between starting compressors (ms)
const unsigned long printInterval = 10000;        // How often to print live pressure readings

unsigned long lastPrint1 = 0;
unsigned long lastPrint2 = 0;
unsigned long lastPrint3 = 0;

// === Pressure Thresholds (PSI) ===
const float pressureThresholdLow = 70.0;
const float pressureThresholdHigh = 120.0;

// Relay logic (active-low)
const bool RELAY_ON = LOW;
const bool RELAY_OFF = HIGH;

void initializeRelays() {
  pinMode(compressorPin1, OUTPUT);
  pinMode(compressorPin2, OUTPUT);
  pinMode(compressorPin3, OUTPUT);

  // Ensure all compressors are OFF at startup
  digitalWrite(compressorPin1, RELAY_OFF);
  digitalWrite(compressorPin2, RELAY_OFF);
  digitalWrite(compressorPin3, RELAY_OFF);
}

void setup() {
  initializeRelays();
  Serial.begin(9600);
  delay(2000); // Optional: let sensors stabilize
  Serial.println("System initialized with all compressors OFF.");
}

void loop() {
  float psi1 = readPressure(sensorPin1);
  float psi2 = readPressure(sensorPin2);
  float psi3 = readPressure(sensorPin3);

  unsigned long now = millis();

  // Count how many compressors are ON
  int compressorsRunning = 0;
  if (compressor1State) compressorsRunning++;
  if (compressor2State) compressorsRunning++;
  if (compressor3State) compressorsRunning++;

  // === Compressor 1 Logic ===
  if (psi1 < pressureThresholdLow) {
    if (!compressor1State && !compressor1Waiting && compressorsRunning < 2 && now - lastStartTime >= startupDelay) {
      digitalWrite(compressorPin1, RELAY_ON);
      compressor1State = true;
      lastStartTime = now;
      compressorsRunning++;
      lastPrint1 = now;
      Serial.print("[Compressor 1 ON] Pressure: ");
      Serial.println(psi1);
    } else if (!compressor1State && !compressor1Waiting) {
      compressor1Waiting = true;
    }
  } else if (psi1 > pressureThresholdHigh && compressor1State) {
    digitalWrite(compressorPin1, RELAY_OFF);
    compressor1State = false;
    compressor1Waiting = false;
    Serial.print("[Compressor 1 OFF] Final Pressure: ");
    Serial.println(psi1);
  }

  if (compressor1State && now - lastPrint1 >= printInterval) {
    Serial.print("[Compressor 1] Pressure: ");
    Serial.println(psi1);
    lastPrint1 = now;
  }

  // === Compressor 2 Logic ===
  if (psi2 < pressureThresholdLow) {
    if (!compressor2State && !compressor2Waiting && compressorsRunning < 2 && now - lastStartTime >= startupDelay) {
      digitalWrite(compressorPin2, RELAY_ON);
      compressor2State = true;
      lastStartTime = now;
      compressorsRunning++;
      lastPrint2 = now;
      Serial.print("[Compressor 2 ON] Pressure: ");
      Serial.println(psi2);
    } else if (!compressor2State && !compressor2Waiting) {
      compressor2Waiting = true;
    }
  } else if (psi2 > pressureThresholdHigh && compressor2State) {
    digitalWrite(compressorPin2, RELAY_OFF);
    compressor2State = false;
    compressor2Waiting = false;
    Serial.print("[Compressor 2 OFF] Final Pressure: ");
    Serial.println(psi2);
  }

  if (compressor2State && now - lastPrint2 >= printInterval) {
    Serial.print("[Compressor 2] Pressure: ");
    Serial.println(psi2);
    lastPrint2 = now;
  }

  // === Compressor 3 Logic ===
  if (psi3 < pressureThresholdLow) {
    if (!compressor3State && !compressor3Waiting && compressorsRunning < 2 && now - lastStartTime >= startupDelay) {
      digitalWrite(compressorPin3, RELAY_ON);
      compressor3State = true;
      lastStartTime = now;
      compressorsRunning++;
      lastPrint3 = now;
      Serial.print("[Compressor 3 ON] Pressure: ");
      Serial.println(psi3);
    } else if (!compressor3State && !compressor3Waiting) {
      compressor3Waiting = true;
    }
  } else if (psi3 > pressureThresholdHigh && compressor3State) {
    digitalWrite(compressorPin3, RELAY_OFF);
    compressor3State = false;
    compressor3Waiting = false;
    Serial.print("[Compressor 3 OFF] Final Pressure: ");
    Serial.println(psi3);
  }

  if (compressor3State && now - lastPrint3 >= printInterval) {
    Serial.print("[Compressor 3] Pressure: ");
    Serial.println(psi3);
    lastPrint3 = now;
  }

  // === Handle Delayed Start for Waiting Compressors ===
  compressorsRunning = 0;
  if (compressor1State) compressorsRunning++;
  if (compressor2State) compressorsRunning++;
  if (compressor3State) compressorsRunning++;

  if (compressorsRunning < 2 && now - lastStartTime >= startupDelay) {
    if (compressor1Waiting) {
      digitalWrite(compressorPin1, RELAY_ON);
      compressor1State = true;
      compressor1Waiting = false;
      lastStartTime = now;
      lastPrint1 = now;
      Serial.print("[Compressor 1 (DELAYED ON)] Pressure: ");
      Serial.println(psi1);
    } else if (compressor2Waiting) {
      digitalWrite(compressorPin2, RELAY_ON);
      compressor2State = true;
      compressor2Waiting = false;
      lastStartTime = now;
      lastPrint2 = now;
      Serial.print("[Compressor 2 (DELAYED ON)] Pressure: ");
      Serial.println(psi2);
    } else if (compressor3Waiting) {
      digitalWrite(compressorPin3, RELAY_ON);
      compressor3State = true;
      compressor3Waiting = false;
      lastStartTime = now;
      lastPrint3 = now;
      Serial.print("[Compressor 3 (DELAYED ON)] Pressure: ");
      Serial.println(psi3);
    }
  }

  delay(100); // Optional: loop pacing
}

// === Calibrated Pressure Reading ===
float readPressure(int pin) {
  int rawADC = analogRead(pin);
  float voltage = (rawADC / 1023.0) * 5.0;
  float psi = 59.60 * (voltage - 0.49);
  if (psi < 0) psi = 0;
  return psi;
}
