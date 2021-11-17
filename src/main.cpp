#include "vex.h"

using namespace vex;

competition Competition;
controller Control = controller(primary);
// Front Pincers
motor FrontLeftPincer = motor(PORT5, ratio36_1, false);
motor FrontRightPincer = motor(PORT6, ratio36_1, true);
// Back Pincers
motor BackLeftPincer = motor(PORT15, ratio36_1, false);
motor BackRightPincer =motor(PORT16, ratio36_1, true);
// Right Wheels
motor FrontRightWheel = motor(PORT10, ratio18_1, true);
motor BackRightWheel = motor(PORT20, ratio18_1, true);
// Left Wheels
motor FrontLeftWheel = motor(PORT1, ratio18_1, false);
motor BackLeftWheel = motor(PORT11, ratio18_1, false);
// Motor Groups
motor_group LeftWheels = motor_group(FrontLeftWheel, BackLeftWheel);
motor_group RightWheels = motor_group(BackRightWheel, FrontRightWheel);
motor_group BackPincers = motor_group(BackLeftPincer, BackRightPincer);
motor_group FrontPincers = motor_group(FrontLeftPincer, FrontRightPincer);

// Drivetrain & Inertial Sensor
inertial InertialSensor = inertial(PORT19);
smartdrive Drivetrain = smartdrive(LeftWheels, RightWheels, InertialSensor, 319.19, 304.8, 304.8, mm, 1.6);

bool isReverse = false;
/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

  vexcodeInit();
}
/*---------------------------------------------------------------------------*/
/*                              Autonomous Task                              */
/*---------------------------------------------------------------------------*/

void autonomousYellowGoal() {
  Drivetrain.driveFor(forward, 40, inches);
  FrontPincers.spinFor(reverse, 350, degrees);
  Drivetrain.driveFor(forward, 10, inches);
  FrontPincers.spinFor(forward, 240, degrees);
  LeftWheels.setVelocity(40, percent);
  RightWheels.setVelocity(40, percent);
  Drivetrain.driveFor(reverse, 29, inches);
}

void autonomousPlatform() {
  Drivetrain.turnFor(-93, degrees);
  Drivetrain.driveFor(forward, 70, inches);
  Drivetrain.turnFor(-100, degrees);
  Drivetrain.driveFor(forward, 10, inches);
}

void autonomous(void) {
  LeftWheels.setVelocity(75, percent);
  RightWheels.setVelocity(75, percent);
  autonomousYellowGoal();
  autonomousPlatform();
}
/*---------------------------------------------------------------------------*/
/*                              User Control Task                            */
/*---------------------------------------------------------------------------*/

void changeMovement() {
  if(Control.ButtonDown.pressing()) {
    isReverse = true;
  }
  if(Control.ButtonUp.pressing()) {
    isReverse = false;
  }
}

void leftMovement() {
  if(Control.Axis3.value() != 0){
    if(isReverse) {
      LeftWheels.spin(reverse, Control.Axis3.value(), percent);
    } else {
      LeftWheels.spin(forward, Control.Axis3.value(), percent);
    }
    } 
  else {
    LeftWheels.stop(hold);
  }
}

void rightMovement() {
  if(Control.Axis2.value() !=0) {
    if(isReverse){
      RightWheels.spin(reverse, Control.Axis2.value(), percent);
    }else {
      RightWheels.spin(forward, Control.Axis2.value(), percent);
    }
  } else {
    RightWheels.stop(hold);
  }
}

void frontPincersMovement() {
  if(isReverse) {
      if(Control.ButtonR1.pressing()) {
        BackPincers.spin(reverse);
    } else if(Control.ButtonR2.pressing()) {
        BackPincers.spin(forward);
    } else {
        BackPincers.stop(hold);
    }
  } else {
      if(Control.ButtonR1.pressing()){
        FrontPincers.spin(forward);
    } else if(Control.ButtonR2.pressing()) {
        FrontPincers.spin(reverse);
    } else {
        FrontPincers.stop(hold);
    }
  }
}

void backPincersMovement() {
  if(isReverse) {
    if(Control.ButtonL1.pressing()) {
      FrontPincers.spin(forward);
    } else if(Control.ButtonL2.pressing()) {
      FrontPincers.spin(reverse);
    } else {
      FrontPincers.stop(hold);
    }
  } else {
      if(Control.ButtonL1.pressing()) {
      BackPincers.spin(reverse);
    } else if(Control.ButtonL2.pressing()) {
      BackPincers.spin(forward);
    } else {
      BackPincers.stop(hold);
    }
  }
}

void usercontrol(void) {
  LeftWheels.setVelocity(100, percent);
  RightWheels.setVelocity(100, percent);
  FrontPincers.setVelocity(80, percent);
  BackPincers.setVelocity(80, percent);
  FrontPincers.setMaxTorque(100, percent);
  BackPincers.setMaxTorque(100, percent);
  while (1) {
    changeMovement();
    rightMovement();
    leftMovement();
    frontPincersMovement();
    backPincersMovement();

    wait(20, msec);
  }
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    wait(100, msec);
  }
}