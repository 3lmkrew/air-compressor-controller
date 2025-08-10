#include "Compressor.h"

// Relay logic (active-low)
const bool RELAY_ON = LOW;
const bool RELAY_OFF = HIGH;

// How often to print live pressure readings
const unsigned long printInterval = 10000;

Compressor::Compressor(const char* name, int sensorPin, int compressorPin)
  : _name(name),
    _sensorPin(sensorPin),
    _compressorPin(compressorPin),
    _state(false),
    _waiting(false),
    _lastPrintTime(0) {}

void Compressor::begin() {
  pinMode(_compressorPin, OUTPUT);
  digitalWrite(_compressorPin, RELAY_OFF); // Ensure it's off initially
}

void Compressor::update(float pressureThresholdLow, float pressureThresholdHigh) {
  float psi = readPressure();

  // Turn off if pressure is high
  if (psi > pressureThresholdHigh && _state) {
    turnOff();
    Serial.print("[");
    Serial.print(_name);
    Serial.print(" OFF] Final Pressure: ");
    Serial.println(psi);
  }

  // Request to turn on if pressure is low
  if (psi < pressureThresholdLow) {
    if (!_state && !_waiting) {
      setWaiting(true);
    }
  }

  // Print periodic status update if the compressor is on
  if (_state && (millis() - _lastPrintTime >= printInterval)) {
    Serial.print("[");
    Serial.print(_name);
    Serial.print("] Pressure: ");
    Serial.println(psi);
    _lastPrintTime = millis();
  }
}


void Compressor::turnOn() {
  if (!_state) {
    digitalWrite(_compressorPin, RELAY_ON);
    _state = true;
    _waiting = false;
    _lastPrintTime = millis();

    Serial.print("[");
    Serial.print(_name);
    Serial.print(" ON] Pressure: ");
    Serial.println(readPressure());
  }
}

void Compressor::turnOff() {
  if (_state) {
    digitalWrite(_compressorPin, RELAY_OFF);
    _state = false;
    _waiting = false;
  }
}

bool Compressor::isOn() const {
  return _state;
}

bool Compressor::isWaiting() const {
  return _waiting;
}

void Compressor::setWaiting(bool waiting) {
  _waiting = waiting;
}

const char* Compressor::getName() const {
    return _name;
}

float Compressor::readPressure() {
  int rawADC = analogRead(_sensorPin);
  float voltage = (rawADC / 1023.0) * 5.0;
  float psi = 59.60 * (voltage - 0.49);
  if (psi < 0) psi = 0;
  return psi;
}
