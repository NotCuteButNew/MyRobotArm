#include "HardwareSerial.h"
#include "RampsStepper.h"
#include "WString.h"
#include "arduino.h"
#include "command.h"
#include "config.h"
#include "endstop.h"
#include "logger.h"
#include "pinout.h"
#include "queue.h"
#include <SD.h>
#include <Servo.h>
#include <TimerFour.h>
#include <TimerOne.h>
#include <TimerThree.h>
#include <arduino.h>

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

Servo servo_sg90;
int servoStatus = 0;

Queue<float *> queue(QUEUE_SIZE);

Command command;

File file;

void setStepperEnable(bool enable);
void homeSequence();
void updateWraper1(); // Higher步进电机update函数接口，用于timer1中断
void updateWraper3(); // lower步进电机update函数接口，用于timer3中断
void updateWraper4(); // rotate步进电机update函数接口，用于timer4中断
void executeCommand(float *cmd);

void setup() {
  Serial.begin(BAUD);
  homeSequence();
  Logger::logINFO("************start************");
  if (!SD.begin(SDSS)) {
    Logger::logINFO("SD card initialization failed");
    return;
  }
  Logger::logINFO("SD card initialized successfully");
  file = SD.open("deg.txt");
  if (!file) {
    Logger::logINFO("cannot open file");
    return;
  }
  servo_sg90.attach(SERVO_PIN);
  Timer1.initialize(HOME_DWELL);
  Timer1.attachInterrupt(updateWraper1);
  delayMicroseconds(HOME_DWELL / 3);
  Timer3.initialize(HOME_DWELL);
  Timer3.attachInterrupt(updateWraper3);
  delayMicroseconds(HOME_DWELL / 3);
  Timer4.initialize(HOME_DWELL);
  Timer4.attachInterrupt(updateWraper4);
}

void loop() {
  if (!queue.isFull()) {
    if (file.available()) {
      String line = file.readStringUntil('\r');
      command.processMessage(line);
      queue.push(command.new_command);
    } else {
      Logger::logINFO("mission completed");
    }
  }
  if ((!queue.isEmpty()) && stepperHigher.isdone() && stepperLower.isdone() &&
      stepperRotate.isdone()) {
    if (servoStatus)
      servo_sg90.write(90);
    else
      servo_sg90.write(0);
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
  servoStatus = cmd[3];
#if true
  Logger::logINFO("executing command: [" + String(cmd[0]) + " " +
                  String(cmd[1]) + " " + String(cmd[2]) + " " + String(cmd[3]) +
                  "]");
  Logger::logDEBUG("realCount: " + String(queue.getRealCount()));
#endif
}

void updateWraper1() { stepperHigher.update(); }
void updateWraper3() { stepperLower.update(); }
void updateWraper4() { stepperRotate.update(); }