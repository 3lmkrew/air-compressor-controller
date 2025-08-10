#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <Arduino.h>

class Compressor {
public:
  // Constructor
  Compressor(const char* name, int sensorPin, int compressorPin);

  // Methods
  void begin();
  void update(float pressureThresholdLow, float pressureThresholdHigh);
  void turnOn();
  void turnOff();

  bool isOn() const;
  bool isWaiting() const;
  void setWaiting(bool waiting);
  const char* getName() const;

private:
  const char* _name;
  int _sensorPin;
  int _compressorPin;
  bool _state;
  bool _waiting;
  unsigned long _lastPrintTime;

  float readPressure();
};

#endif // COMPRESSOR_H
