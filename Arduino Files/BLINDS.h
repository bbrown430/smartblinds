#include <AH_EasyDriver.h>

#define STEPPER_SPEED             18                  //Defines the speed in RPM for the stepper motor
#define STEPPER_STEPS_PER_REV     2048                //Defines the number of pulses that is required for the stepper to rotate 360 degrees
#define STEPPER_MICROSTEPPING     0                   //Defines microstepping 0 = no microstepping, 1 = 1/2 stepping, 2 = 1/4 stepping 

#define STEPPER_DIR_PIN_BIG           12
#define STEPPER_STEP_PIN_BIG          13
#define STEPPER_SLEEP_PIN_BIG         14

// not used
#define STEPPER_MICROSTEP_1_PIN   18
#define STEPPER_MICROSTEP_2_PIN   19

AH_EasyDriver shadeStepperBig(STEPPER_STEPS_PER_REV, STEPPER_DIR_PIN_BIG ,STEPPER_STEP_PIN_BIG,STEPPER_MICROSTEP_1_PIN,STEPPER_MICROSTEP_2_PIN,STEPPER_SLEEP_PIN_BIG);

struct DEV_MyBlind : Service::WindowCovering {

  // homekit states
  SpanCharacteristic *current;
  SpanCharacteristic *target;

  bool moving = false;

  DEV_MyBlind() : Service::WindowCovering() {
    
    current = new Characteristic::CurrentPosition(0); // initialize as closed
    target = new Characteristic::TargetPosition(0);
    target->setRange(0, 100, 10);
    current->setRange(0, 100, 10);


    shadeStepperBig.setMicrostepping(STEPPER_MICROSTEPPING);                           
    shadeStepperBig.setSpeedRPM(STEPPER_SPEED);
    #if DRIVER_INVERTED_SLEEP == 1
    shadeStepperBig.sleepOFF();
    #endif
    #if DRIVER_INVERTED_SLEEP == 0
    shadeStepperBig.sleepON();
    #endif
    }
  
  boolean update() {
    return(true);
  }

  // FORWARD = open, BACKWARD = close
  void step(bool forward, int amount) {
    LOG1("stepping, direction =" + forward);
    if (!moving) {
      #if DRIVER_INVERTED_SLEEP == 1
      shadeStepperBig.sleepON();
      #endif
      #if DRIVER_INVERTED_SLEEP == 0
      shadeStepperBig.sleepOFF();
      #endif
    }
    moving = true;
    shadeStepperBig.move(amount, !forward);
  }
  
  void loop() {
    if (current->getVal() - target->getVal() >= 10)
      {
      step(FORWARD, 106);

      if (current->getVal() == 10)
      {
        step(FORWARD, 106);
      }

      current->setVal(current->getVal() - 10);
      } 

    else if (current->getVal() - target->getVal() <= -10) {
      step(BACKWARD, 106);
      current->setVal(current->getVal() + 10);
    } else {
      if (moving) {
        #if DRIVER_INVERTED_SLEEP == 1
        shadeStepperBig.sleepOFF();
        #endif
        #if DRIVER_INVERTED_SLEEP == 0
        shadeStepperBig.sleepON();
        #endif

        moving = false;
      }
    }
  }
};
