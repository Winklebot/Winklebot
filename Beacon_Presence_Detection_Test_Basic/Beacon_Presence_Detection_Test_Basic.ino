
/**************************************************************
  File:      Beacon_Presence_Detection_Test_Basic.ino
  Contents:  Detects whether the sensor is facing a beacon and what
              the period of the beacon is. Only stores one rising edge
              time
             
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
#define NO_SIGNAL_MICROS 10000


/*---------------- Module Function Prototypes ---------------*/
void riseDetected();
void RespToKey();
/*---------------- Module Level Variables -------------------*/
volatile unsigned long previousTime = 0;
volatile unsigned long period = NO_SIGNAL_MICROS;
volatile boolean risingEdgeFlag = false; 
/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The Beacon_Presence_Detection_Test program has started!");
  pinMode(BEACON_INPUT_PIN, INPUT);
  attachInterrupt(1, riseDetected, RISING);
  TMRArd_InitTimer(0, 2);
}

void loop() {
  if(TMRArd_IsTimerExpired(0)){
    if(!risingEdgeFlag){
      period = NO_SIGNAL_MICROS;
    }
    risingEdgeFlag = false;
    TMRArd_InitTimer(0, 2);

  }
  if(TestForKey()) RespToKey();
}

/*---------------- Module Functions -------------------------*/

void riseDetected() {
  unsigned long currentTime = micros(); 
  period = currentTime - previousTime;
  previousTime = currentTime; 
  risingEdgeFlag = true;
}  




void RespToKey() {
  unsigned char theKey;
  theKey = Serial.read();
  if(period > SERVER_BEACON_MICROS + 10 || period < SERVER_BEACON_MICROS - 10){
    Serial.println("Server Beacon out of range");
  }
  else{
    Serial.println("Server Beacon in range");
  }
  Serial.print("Period: ");
  Serial.println(period);
  Serial.println("--------------------------");
  
} 
    
 


  





