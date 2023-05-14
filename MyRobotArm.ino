#include "HardwareSerial.h"
#include "RampsStepper.h"
#include "command.h"
#include "config.h"
#include "endstop.h"
#include "logger.h"
#include "pinout.h"
#include "pins_arduino.h"
#include "queue.h"
#include <TimerOne.h>
#include <arduino.h>

void setStepperEnable(bool enable);
void homeSequence();
void updateWraper();

Endstop endstopX(X_MIN_PIN, X_DIR_PIN, X_STEP_PIN, X_ENABLE_PIN, X_MIN_INPUT,
                 X_HOME_STEPS, HOME_DWELL, false);
Endstop endstopY(Y_MIN_PIN, Y_DIR_PIN, Y_STEP_PIN, Y_ENABLE_PIN, Y_MIN_INPUT,
                 Y_HOME_STEPS, HOME_DWELL, false);
Endstop endstopZ(Z_MIN_PIN, Z_DIR_PIN, Z_STEP_PIN, Z_ENABLE_PIN, Z_MIN_INPUT,
                 Z_HOME_STEPS, HOME_DWELL, false);

RampsStepper stepperHigher(X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN,
                           INVERSE_X_STEPPER, MAIN_GEAR_TEETH, MOTOR_GEAR_TEETH,
                           MICROSTEPS, STEPS_PER_REV, endstopX);
RampsStepper stepperLower(Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN,
                          INVERSE_Y_STEPPER, MAIN_GEAR_TEETH, MOTOR_GEAR_TEETH,
                          MICROSTEPS, STEPS_PER_REV, endstopY);
RampsStepper stepperRotate(Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN,
                           INVERSE_Z_STEPPER, MAIN_GEAR_TEETH, MOTOR_GEAR_TEETH,
                           MICROSTEPS, STEPS_PER_REV, endstopZ);
Queue<float *> queue(QUEUE_SIZE);

Command command;

void executeCommand(float *cmd);

void setup() {
  Serial.begin(BAUD);
  // homeSequence();
  Logger::logINFO("************start************");
  Timer1.initialize(HOME_DWELL);
  Timer1.attachInterrupt(updateWraper);
}

void loop() {
  // stepperRotate.update();
  // stepperLower.update();
  // stepperHigher.update();
  if (!queue.isFull()) {
    Logger::logINFO("---waiting for the next command---");
    if (command.handleCommand()) {
      queue.push(command.new_command);
    }
  }
  if ((!queue.isEmpty()) && stepperHigher.isdone() && stepperLower.isdone() &&
      stepperRotate.isdone()) {
    executeCommand(queue.pop());
  }
}

void setStepperEnable(bool enable) {
  stepperRotate.enable(enable);
  stepperLower.enable(enable);
  stepperHigher.enable(enable);
}

void homeSequence() {
  setStepperEnable(false);
  if (HOME_Y_STEPPER && HOME_X_STEPPER) {
    endstopY.home(!INVERSE_Y_STEPPER);
    endstopX.home(!INVERSE_X_STEPPER);
  }
  if (HOME_Z_STEPPER) {
    endstopZ.home(!INVERSE_Z_STEPPER);
  }
  Logger::logINFO("HOMING COMPLETE");
}

void executeCommand(float *cmd) {
  stepperHigher.stepToPositionDeg(cmd[0]);
  stepperLower.stepToPositionDeg(cmd[1]);
  stepperRotate.stepToPositionDeg(cmd[2]);
  Logger::logINFO("executing command: [(" + String(int(cmd[0])) + ")" + " " +
                  String(cmd[1]) + " " + String(cmd[2]) + " " + String(cmd[3]) +
                  "]");
}

void updateWraper() { stepperHigher.update(); }