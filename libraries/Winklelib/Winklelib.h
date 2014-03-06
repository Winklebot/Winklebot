// WinkleLib.h - lists out all functions we need to control WinkleBot
// Team WinkleBot -- last updated 3.2.2014
// code influenced by Roachlib
// our pins are also defined in the cpp file

#ifndef WinkleLib_h
#define WinkleLib_h

#include "Arduino.h"
#include "Timers.h"

typedef enum {
   OK_SPEED, ERR_BADSPEED
} RoachReturn_t;

    void BitBotInit(void);
    
    void LeftMtrSpeed();
    void RightMtrSpeed();
    void Forward(unsigned char newSpeed);
 //   void Forward(unsigned char LeftMtrSpeed, unsigned char RightMtrSpeed);
    void Backward(unsigned char newSpeed);
    void Stop();
    void TurnLeft(unsigned char newSpeed);
    void TurnRight(unsigned char newSpeed);
    void RotateCW(unsigned char newSpeed);
 //   void RotateCW (unsigned char LeftMtrSpeed, unsigned char RightMtrSpeed);
    void RotateCCW(unsigned char newSpeed);
    unsigned char ReadBumpers(void);
    unsigned char TestForKey(void);
    void RespToKey(void);
    
   void WinkleInit(void);
   void DriveForward(char newSpeed);
   void DriveBackward(char newSpeed);
   void SpinRight(char newSpeed);
   void SpinLeft(char newSpeed);

   RoachReturn_t LeftMtrSpeed(char newSpeed);
   RoachReturn_t RightMtrSpeed(char newSpeed);
   unsigned char ReadBumpers(void);

   void SET_SHARED_BYTE_TO(unsigned char);
   unsigned char GET_SHARED_BYTE(void);
   void SET_SHARED_WORD_TO(unsigned int);
   unsigned int GET_SHARED_WORD(void);



#endif

