
/**************************************************************
  File:      Beacon_Stop_Test.ino
  Contents:  Turns until it detects a server beacon
             
  Notes:    Target: Arduino UNO R1 & R2
            Arduino IDE version: 0022
            
**************************************************************/

/*---------------- Includes ---------------------------------*/
#include <Winklelib.h>
#include <Timers.h>
/*---------------- Module Defines ---------------------------*/
#define TAPE_INPUT_PIN 2
#define BEACON_INPUT_PIN 3
#define BEACON_INTERRUPT_NUMBER 1
#define TAPE_INTERRUPT_NUMBER 0  
  
#define SERVER_BEACON_MICROS 1176
#define EXCHANGE_BEACON_MICROS 333
#define NO_SIGNAL_MICROS 2000
#define BEACON_ERROR 100

#define TURNING_SPEED 4
#define FORWARD_SPEED 7
/*---------------- Module Function Prototypes ---------------*/
void rise_detected();
/*---------------- Module Level Variables -------------------*/
volatile unsigned long previousTime = 0;
volatile unsigned long period = NO_SIGNAL_MICROS; 
volatile boolean risingEdgeFlag = false; 
boolean beaconFoundFlag = false;
int pause = 4;

/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The Beacon_Stop_Test program has started!");
  pinMode(BEACON_INPUT_PIN, INPUT);
  WinkleInit();  
  RightMtrSpeed(-1 * TURNING_SPEED);
  LeftMtrSpeed(TURNING_SPEED);
  TMRArd_InitTimer(0, 2);
  attachInterrupt(BEACON_INTERRUPT_NUMBER, rise_detected, RISING);
  Serial.println("Searching for beacon");
 
 // attachInterrupt(1, fall_detected, FALLING);
  
}

void loop() {
   if(TMRArd_IsTimerExpired(0)){
    if(!risingEdgeFlag){
      period = NO_SIGNAL_MICROS;
    }
    risingEdgeFlag = false;
    TMRArd_InitTimer(0, 2);

  }
  //If Beacon is found, then stop the rotation
  unsigned long currentPeriod = period;
  if(!beaconFoundFlag && currentPeriod < SERVER_BEACON_MICROS + BEACON_ERROR && currentPeriod > SERVER_BEACON_MICROS - BEACON_ERROR ){
    beaconFoundFlag = true;
      //LeftMtrSpeed(-10);
      //RightMtrSpeed(10);
     // delayMicroseconds(12000);
      LeftMtrSpeed(0); // motors not meant for instantaneous reverse so including a brief stop
      RightMtrSpeed(0);
      LeftMtrSpeed(FORWARD_SPEED);
      RightMtrSpeed(FORWARD_SPEED);
    Serial.println("850 hz beacon found - stopping ");
  }
  
}

/*---------------- Module Functions -------------------------*/

void rise_detected() {
  unsigned long currentTime = micros(); 
  period = currentTime - previousTime;
  previousTime = currentTime; 
  risingEdgeFlag = true;

}  
  





