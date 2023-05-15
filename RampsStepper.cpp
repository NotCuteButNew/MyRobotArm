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
  stepperStepTargetPosition = deg * degToStepFactor;
}

void RampsStepper::update() {
  //   if (stepperStepTargetPosition) {
  // #if debug
  //     // Logger::logDEBUG("----Updating----");
  //     // debug_logs();
  //     unsigned long startTime = micros();
  // #endif
  //     bool temp_dir = true;
  //     if (stepperStepTargetPosition < 0) {
  //       temp_dir = false;
  //       stepperStepTargetPosition++;
  //     } else {
  //       stepperStepTargetPosition--;
  //     }
  //     if (temp_dir ^ inverse) //
  //     (旋转方向)^(是否反转)，相同为逆时针，不同为顺时针
  //       digitalWrite(dirPin, LOW);
  //     else
  //       digitalWrite(dirPin, HIGH);
  //     digitalWrite(stepPin, HIGH);
  //     digitalWrite(stepPin, LOW);
  // #if debug
  //     unsigned long endTime = micros();
  //     char buf[30];
  //     sprintf(buf, "[%lu us]", endTime - startTime);
  //     Logger::logDEBUG(buf);
  // #endif
  //   }
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
