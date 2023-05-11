#include "RampsStepper.h"
#include "config.h"
#include "endstop.h"
#include "logger.h"
#include "pinout.h"
#include <arduino.h>

void setStepperEnable(bool enable);
void homeSequence();

RampsStepper stepperHigher(X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN,
                           INVERSE_X_STEPPER, MAIN_GEAR_TEETH, MOTOR_GEAR_TEETH,
                           MICROSTEPS, STEPS_PER_REV);
RampsStepper stepperLower(Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN,
                          INVERSE_Y_STEPPER, MAIN_GEAR_TEETH, MOTOR_GEAR_TEETH,
                          MICROSTEPS, STEPS_PER_REV);
RampsStepper stepperRotate(Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN,
                           INVERSE_Z_STEPPER, MAIN_GEAR_TEETH, MOTOR_GEAR_TEETH,
                           MICROSTEPS, STEPS_PER_REV);

Endstop endstopX(X_MIN_PIN, X_DIR_PIN, X_STEP_PIN, X_ENABLE_PIN, X_MIN_INPUT,
                 X_HOME_STEPS, HOME_DWELL, false);
Endstop endstopY(Y_MIN_PIN, Y_DIR_PIN, Y_STEP_PIN, Y_ENABLE_PIN, Y_MIN_INPUT,
                 Y_HOME_STEPS, HOME_DWELL, false);
Endstop endstopZ(Z_MIN_PIN, Z_DIR_PIN, Z_STEP_PIN, Z_ENABLE_PIN, Z_MIN_INPUT,
                 Z_HOME_STEPS, HOME_DWELL, false);

void setup() { homeSequence(); }

void loop() {
    
}

void setStepperEnable(bool enable) {
  stepperRotate.enable(enable);
  stepperLower.enable(enable);
  stepperHigher.enable(enable);
}

void homeSequence() {
  setStepperEnable(false);
  if (HOME_Y_STEPPER && HOME_X_STEPPER) {
    endstopY.home(INVERSE_Y_STEPPER);
    endstopX.home(INVERSE_X_STEPPER);
  }
  if (HOME_Z_STEPPER) {
    endstopZ.home(INVERSE_Z_STEPPER);
  }
  Logger::logINFO("HOMING COMPLETE");
}