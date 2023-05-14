#ifndef RAMPSSTEPPER_H_
#define RAMPSSTEPPER_H_
#include "endstop.h"
#include "logger.h"
class RampsStepper {
public:
  RampsStepper(int aStepPin, int aDirPin, int aEnablePin, bool aInverse,
               float main_gear_teeth, float motor_gear_teeth, int microsteps,
               int steps_per_rev, Endstop &aEndstop);
  void enable(bool value = true);
  void stepToPositionDeg(float deg);
  void update();
  void setReductionRatio(float gearRatio, int stepsPerRev);
  bool isdone();
  void reverseOffset();

private:
  int stepperStepTargetPosition;
  int stepPin;
  int dirPin;
  int enablePin;
  bool inverse;
  float degToStepFactor;
  Endstop *endstop;
};

#endif
