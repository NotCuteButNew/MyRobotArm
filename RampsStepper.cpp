#include "RampsStepper.h"
#include <Arduino.h>

RampsStepper::RampsStepper(int aStepPin, int aDirPin, int aEnablePin,
                           bool aInverse, float main_gear_teeth,
                           float motor_gear_teeth, int microsteps,
                           int steps_per_rev, Endstop &aEndstop) {
  setReductionRatio(main_gear_teeth / motor_gear_teeth,
                    microsteps * steps_per_rev);
  stepPin = aStepPin;
  dirPin = aDirPin;
  enablePin = aEnablePin;
  inverse = aInverse;
  stepperStepTargetPosition;
  endstop = &aEndstop;
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  enable(false);
}

void RampsStepper::enable(bool value) { digitalWrite(enablePin, !value); }

void RampsStepper::stepToPositionDeg(float deg) {
  stepperStepTargetPosition = deg * degToStepFactor;
}

void RampsStepper::update() {
  float temp_steps = abs(stepperStepTargetPosition);
  bool temp_dir = true;
  if (stepperStepTargetPosition < 0)
    temp_dir = false;
  if (temp_dir ^ inverse) // 旋转方向^是否反转，相同为逆时针，不同为顺时针
    digitalWrite(dirPin, LOW);
  else
    digitalWrite(dirPin, HIGH);
  digitalWrite(enablePin, LOW);
  while (temp_steps > 0) {
    digitalWrite(stepPin, HIGH);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(400);
    temp_steps--;
    if (endstop->state()) {
      Logger::logINFO(
          "Limit: The limit switch is touched and the movement has stopped");
      break;
    }
  }
  digitalWrite(enablePin, HIGH);
  stepperStepTargetPosition = 0;
}

void RampsStepper::setReductionRatio(float gearRatio, int stepsPerRev) {
  degToStepFactor = gearRatio * stepsPerRev / 360;
};
