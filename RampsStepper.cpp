#include "RampsStepper.h"
#include <Arduino.h>
#include <stdio.h>

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
  enable(true);
}

void RampsStepper::enable(bool value) { digitalWrite(enablePin, !value); }

void RampsStepper::stepToPositionDeg(float deg) {
  if (inverse)
    deg = -deg;
  stepperStepTargetPosition = deg * degToStepFactor;
}

void RampsStepper::
    update() { // 通过定时器的中断函数调用，每调用一次发送一次脉冲
  if (stepperStepTargetPosition) {
    if (stepperStepTargetPosition < 0) {
      stepperStepTargetPosition++;
      digitalWrite(dirPin, LOW);
    } else {
      digitalWrite(dirPin, HIGH);
      stepperStepTargetPosition--;
    }
    digitalWrite(stepPin, HIGH);
    digitalWrite(stepPin, LOW);
  }
}
void RampsStepper::setReductionRatio(float gearRatio, int stepsPerRev) {
  degToStepFactor = gearRatio * stepsPerRev / 360;
};

bool RampsStepper::isdone() {
  if (stepperStepTargetPosition)
    return false;
  else
    return true;
}
