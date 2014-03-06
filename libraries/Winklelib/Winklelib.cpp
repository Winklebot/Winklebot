//MotorLib.cpp - this library lists out the functions we need to control
// the motors and bumpers of our ME210 BitBot.
// Team WinkleBot -- last updated 3.2.2014
// code influenced by RoachLib
// we can also begin to assign pins in these libraries, if we have permanent pin functions


/*----------------------------- Include Files -------------------------------*/
#if defined(ARDUINO) && ARDUINO >= 100 
#include "Arduino.h"  // if Arduino version 1.0 or later, include Arduino.h
#else
#include "WProgram.h"  // if Arduino version 22, include WProgram.h
#endif

#include "Winklelib.h"


/*----------------------------- Module Defines ------------------------------*/
 #define L_MOTOR_DIR           8
 #define L_MOTOR_EN            6  // changed from 9 because servo library utilizes 9 and 10
 #define R_MOTOR_DIR          12		//moved this from 10 to 12 in order to give the pin_Servo 9 or 10	
 
 #define R_MOTOR_EN           11

 #define LEFT_FRONT_BUMPER        4
 #define RIGHT_FRONT_BUMPER       5
 #define LEFT_BACK_BUMPER         A5 //changed from 6 to accommodate servo library limitation
 #define RIGHT_BACK_BUMPER        7 

 #define TAPE_INPUT_PIN           2
 #define HIGH_RANGE_BEACON_INPUT_PIN         3
#define LOW_RANGE_BEACON_INPUT_PIN         13


// #define BUTTON_PUSH_DIR          12
// #define BUTTON_PUSH_EN           13

 #define pin_Servo	            10		

#define SERVER_BEACON_MICROS 		1176
#define EXCHANGE_BEACON_MICRO		 333
#define NO_SIGNAL_MICROS 		2000

 #define BEACON_INTERRUPT_NUMBER  	1 //number, not pin
 #define TAPE_INTERRUPT_NUMBER  	 0 //number, not pin
 
 //free pins: 0, 1 (we shouldn't use), 12, all the analog pins


#define SPEED_SCALER 25  // map 0-255 PWM settings to 0-10 speed settings



// ***need to initiate the TAPE_INPUT, BEACON_INPUT, BEACON_INTERRUPT_NUMBER, AND TAPE_INTERRUPT_NUMBER pins
// can we do it with pinModes? 
// pinMode(BEACON_INPUT_PIN, INPUT);  
// pinMode(TAPE_INPUT_PIN, INPUT);
// pinMode(BUTTON_PUSH_DIR, OUTPUT);
// pinMode(BUTTON_PUSH_EN, OUTPUT);



/*----------------------------- Module Variables ---------------------------*/
static unsigned char SharedByte;
static unsigned int SharedWord;

/*----------------------------- Module Code --------------------------------*/
/******************************************************************************
  Function:    WinkleInit
  Contents:    Performs all the intitialization necessary for the Cockroach.
               This includes initializing the port pins to interface with the
               bumper sensors and the h-bridges that drive the motors.
  Parameters:  None
  Returns:     Nothing
  Notes:
******************************************************************************/
void WinkleInit(void) {
  /* initialize motor control pins */
  //PORTB &= 0xF0;  // EN=0 (off) for both motors and also DIR=0 for both motors
  //DDRB |= 0x0F;   // make motor EN and DIR pins outputs
  pinMode(L_MOTOR_DIR , OUTPUT);
  pinMode(R_MOTOR_DIR , OUTPUT);
  pinMode(L_MOTOR_EN , OUTPUT);
  pinMode(R_MOTOR_EN , OUTPUT);





  /* initialize bumper pins */
  //DDRD &= 0x0F;   // make bumper sensor pins inputs
}

void DriveBackward(char newSpeed){
	LeftMtrSpeed(-1 * newSpeed);
	RightMtrSpeed(-1 * newSpeed);
}
void DriveForward(char newSpeed){
	LeftMtrSpeed(newSpeed);
	RightMtrSpeed(newSpeed);
}

void SpinRight(char newSpeed){
	LeftMtrSpeed(newSpeed);
	RightMtrSpeed(-1 * newSpeed);
}

void SpinLeft(char newSpeed){
	LeftMtrSpeed(-1 * newSpeed);
	RightMtrSpeed(newSpeed);
}
/******************************************************************************
  Function:    LeftMtrSpeed
  Contents:    This function is used to set the speed and direction of the left motor.
  Parameters:  A value between -10 and 10 which is the new speed of the left motor.
                0 stops the motor.  A negative value is reverse.
  Returns:     OK_OPERATION == new speed was successfully sent
               ERR_BADSPEED == an invalid speed was given
  Notes:
******************************************************************************/
RoachReturn_t LeftMtrSpeed(char newSpeed) {
  if ((newSpeed < -10) || (newSpeed > 10)) {
    return ERR_BADSPEED;
  }
  if (newSpeed < 0) {
    digitalWrite(L_MOTOR_DIR,LOW); // set the direction to reverse
  } else {
    digitalWrite(L_MOTOR_DIR,HIGH); // set the direction to forward
  }
  analogWrite(L_MOTOR_EN,SPEED_SCALER*abs(newSpeed));
    return OK_SPEED;
}

/******************************************************************************
  Function:    RightMtrSpeed
  Contents:    This function is used to set the speed and direction of the right motor.
  Parameters:  A value between -10 and 10 which is the new speed of the right motor.
                0 stops the motor.  A negative value is reverse.
  Returns:     OK_OPERATION == new speed was successfully sent
               ERR_BADSPEED == an invalid speed was given
  Notes:
******************************************************************************/
RoachReturn_t RightMtrSpeed(char newSpeed) {
  if ((newSpeed < -10) || (newSpeed > 10)) {
    return ERR_BADSPEED;
  }
  if (newSpeed < 0) {
    digitalWrite(R_MOTOR_DIR,LOW); // set the direction to reverse
  } else {
    digitalWrite(R_MOTOR_DIR,HIGH); // set the direction to forward
  }
  analogWrite(R_MOTOR_EN,SPEED_SCALER*abs(newSpeed));
    return OK_SPEED;
}



/******************************************************************************
  Function:    ReadBumpers
  Contents:    This function checks the four bumper sensors and reports whether
                the bumper has been displaced sufficiently to trigger 1 or more
                of the bumper sensors.
  Parameters:  None
  Returns:     An 8 bit value where the upper 4 bits correspond to the bumper
                sensors.  If a bumper is hit, the corresponding bit will be 0,
                otherwise it will be 1.  The lower 4 bits always return 0.
  Notes:
******************************************************************************/
unsigned char ReadBumpers(void) {
  unsigned char bumpers = 0;
  
  bumpers = PIND & 0xF0;
  return bumpers;
}

/******************************************************************************
  Function:    SET_SHARED_BYTE_TO
  Contents:    This function sets the value of the module-level variable
                SharedByte to the new value specified when called.  The data
                is intended to be used immediately afterward using the function
                GET_SHARED_BYTE.
  Parameters:  An 8 bit value.
  Returns:     Nothing
  Notes:
******************************************************************************/
void SET_SHARED_BYTE_TO(unsigned char newByte)
{
  SharedByte = newByte;
}

/******************************************************************************
  Function:    GET_SHARED_BYTE
  Contents:    This function returns the value of the module-level variable
                SharedByte, and is intended to be called immediately after storing
                a value in SharedByte using the SET_SHARED_BYTE_TO function.
  Parameters:  None
  Returns:     An 8 bit value.
  Notes:
******************************************************************************/
unsigned char GET_SHARED_BYTE(void)
{
  return SharedByte;
}

/******************************************************************************
  Function:    SET_SHARED_WORD_TO
  Contents:    This function sets the value of the module-level variable
                SharedWord to the new value specified when called.  The data
                is intended to be used immediately afterward using the function
                GET_SHARED_WORD.
  Parameters:  A 16 bit value.
  Returns:     Nothing
  Notes:
******************************************************************************/
void SET_SHARED_WORD_TO(unsigned int newWord)
{
  SharedWord = newWord;
}

// Function: BitBotInit: Initializes the BitBot. Includes initializing the port pins.
void BitBotInit(void) {
  PORTB &= 0xF0;  // EN=0 (off) for both motors and also DIR=0 for both motors
  DDRB |= 0x0F;   // make motor EN and DIR pins outputs (fromRoachProof)
  DDRD &= 0x0F;   // make bumper sensor pins inputs
}

// Function: LeftMtrSpeed: Sets speed and direction of the left motor with a value from -10 to 10
//Return_t LeftMtrSpeed(char newSpeed) {
//  if ((newSpeed < -10) || (newSpeed > 10)) {
//    return ERR_BADSPEED;
//  }
//  if (newSpeed < 0) {
//    digitalWrite(LEFT_MOTOR_DIR,LOW); // set the direction to reverse
//  } else {
//    digitalWrite(LEFT_MOTOR_DIR,HIGH); // set the direction to forward
//  }
//  analogWrite(LEFT_MOTOR_EN,SPEED_DIVIDER*abs(newSpeed));
//    return OK_SPEED;
//}

// Function: RightMtrSpeed: Sets speed and direction of the right motor with a value from -10 to 10
//Return_t RightMtrSpeed(char newSpeed) {
//  if ((newSpeed < -10) || (newSpeed > 10)) {
//    return ERR_BADSPEED;
//  }
//  if (newSpeed < 0) {
//    digitalWrite(RIGHT_MOTOR_DIR,LOW); // set the direction to reverse
//  } else {
//    digitalWrite(RIGHT_MOTOR_DIR,HIGH); // set the direction to forward
//  }
//  analogWrite(RIGHT_MOTOR_EN,SPEED_DIVIDER*abs(newSpeed));
//    return OK_SPEED;
//}

//Function: ReadBumpers: Checks bumper sensors and reports if the bumper has been triggered.
// Like RoachLib: returns an 8 bit value where the upper 4 bits correspond to bumper sensors.
// If a bumper is hit, the corresponding bit will be 0, otherwise it will be 1.
// The lower 4 bits will always return 0.

//unsigned char ReadBumpers(void) {
//  unsigned char bumpers = 0;
  
//  bumpers = PIND & 0xF0;
//  return bumpers;
//}

//Function: Forward: motors move the BitBot forward
void Forward(unsigned char newSpeed){
  Serial.println("I'm going forward!");
  LeftMtrSpeed(0); // motors not meant for instantaneous reverse so including a brief stop
  RightMtrSpeed(0);
  delayMicroseconds(10);
  LeftMtrSpeed(newSpeed);
  RightMtrSpeed(newSpeed);
// void Forward(unsigned char LeftMtrSpeed, unsigned char RightMtrSpeed);
}

//Function: Backward: motors move the BitBot backward
void Backward(unsigned char newSpeed){
  Serial.println("I'm going backward!");
  LeftMtrSpeed(0); // motors not meant for instantaneous reverse so including a brief stop
  RightMtrSpeed(0);
  delayMicroseconds(10);
  LeftMtrSpeed(0-newSpeed);
  RightMtrSpeed(0-newSpeed);
}

//Function: Stop: stops the motors
void Stop() {
  Serial.println("I'm stopping!");
  LeftMtrSpeed(0);
  RightMtrSpeed(0);
}

//Function: TurnRight: Turns right at given speed.
void TurnRight(unsigned char newSpeed) {
  Serial.println("I'm turning right!");
  LeftMtrSpeed(0); // motors not meant for instantaneous reverse so including a brief stop
  RightMtrSpeed(0);
  delayMicroseconds(10);
  LeftMtrSpeed(newSpeed);
  RightMtrSpeed(0-newSpeed);
}

//Function: TurnLeft: Turns left at given speed.
void TurnLeft(unsigned char newSpeed) {
  Serial.println("I'm turning left!");
  LeftMtrSpeed(0); // motors not meant for instantaneous reverse so including a brief stop
  RightMtrSpeed(0);
  delayMicroseconds(10);
  LeftMtrSpeed(0-newSpeed);
  RightMtrSpeed(newSpeed);
}


//Function: RotateCW: rotates the bot CW.
void RotateCW(unsigned char newSpeed) {
  Serial.println("I'm rotating clockwise!");
  LeftMtrSpeed(0); // motors not meant for instantaneous reverse so including a brief stop
  RightMtrSpeed(0);
  delayMicroseconds(10);
  LeftMtrSpeed(0-.5*newSpeed);
  RightMtrSpeed(newSpeed);
}

//Function: RotateCCW: rotates the bot CCW.
void RotateCCW(unsigned char newSpeed) {
  Serial.println("I'm rotating counterclockwise!");
  LeftMtrSpeed(0); // motors not meant for instantaneous reverse so including a brief stop
  RightMtrSpeed(0);
  delayMicroseconds(10);
  LeftMtrSpeed(newSpeed);
  RightMtrSpeed(-0.5*newSpeed);
}

//Function: TestForKey
unsigned char TestForKey(void) {
  unsigned char KeyEventOccurred;
  
  KeyEventOccurred = Serial.available();
  return KeyEventOccurred;
}

//Function: RespToKey
//defined within each separate module
/******************************************************************************
  Function:    GET_SHARED_WORD
  Contents:    This function returns the value of the module-level variable
                SharedWord, and is intended to be called immediately after storing
                a value in SharedWord using the SET_SHARED_WORD_TO function.
  Parameters:  None
  Returns:     A 16 bit value.
  Notes:
******************************************************************************/
unsigned int GET_SHARED_WORD(void)
{
  return SharedWord;
}

