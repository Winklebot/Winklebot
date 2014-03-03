
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



/*---------------- Module Function Prototypes ---------------*/
void rise_detected();
/*---------------- Module Level Variables -------------------*/
volatile unsigned long previousTime = 0;
volatile unsigned long period = NO_SIGNAL_MICROS; 
volatile boolean risingEdgeFlag = false; 
boolean beaconFoundFlag = false;

/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The Tape_Stop_Test program has started!");
  pinMode(TAPE_INPUT_PIN, INPUT);
  WinkleInit();  
  SpinRight(8);
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
  unsigned long currentPeriod = period;
  if(!beaconFoundFlag && currentPeriod < SERVER_BEACON_MICROS + 10 && currentPeriod > SERVER_BEACON_MICROS - 10 ){
    beaconFoundFlag = true;
    SpinRight(0);
    //DriveForward(2);
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
  





