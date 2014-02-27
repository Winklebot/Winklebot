/******************************************************************************
Module
  Winklebotlib.cpp 
Description
  
Arduino IDE version:  0022

History
When      Who  Description
--------  ---  -------------------------------------
01/19/12  RMO  created new version of Roachlib.c for the Arduino UNO R2

******************************************************************************/
/*----------------------------- Include Files -------------------------------*/
#if defined(ARDUINO) && ARDUINO >= 100 
#include "Arduino.h"  // if Arduino version 1.0 or later, include Arduino.h
#else
#include "WProgram.h"  // if Arduino version 22, include WProgram.h
#endif

#include "Winklelib.h"

/*----------------------------- Module Defines ------------------------------*/
#define BEACON_INPUT_PIN 3 

#define BEACON_INTERRUPT_NUMBER 1
#define TAPE_INTERRUPT_NUMBER 0  
#define SERVER_BEACON_MICROS 1176
#define EXCHANGE_BEACON_MICROS 333
#define NO_SIGNAL_MICROS 2000
#define L_MOTOR_DIR  8   // left motor direction controlled with pin 8
#define L_MOTOR_EN   9   // left motor enable controlled by pin 9
#define R_MOTOR_DIR  10  // right motor direction controlled with pin 10
#define R_MOTOR_EN   11  // right motor enable controlled by pin 11

#define SPEED_SCALER 25  // map 0-255 PWM settings to 0-10 speed settings


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
  PORTB &= 0xF0;  // EN=0 (off) for both motors and also DIR=0 for both motors
  DDRB |= 0x0F;   // make motor EN and DIR pins outputs
  
  /* initialize bumper pins */
  //DDRD &= 0x0F;   // make bumper sensor pins inputs
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

