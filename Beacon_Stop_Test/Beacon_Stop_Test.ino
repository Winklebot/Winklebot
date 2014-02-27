
/**************************************************************
  File:      Beacon_Stop_Test.ino
  Contents:  Turns until it detects a server beacon
             
  Notes:    Target: Arduino UNO R1 & R2
            Arduino IDE version: 0022
            
**************************************************************/

/*---------------- Includes ---------------------------------*/
#include <Winklelib.h>
/*---------------- Module Defines ---------------------------*/
#define TAPE_INPUT_PIN 2
#define BEACON_INPUT_PIN 3
#define BEACON_INTERRUPT_NUMBER 1
#define TAPE_INTERRUPT_NUMBER 0  
  
#define SERVER_BEACON_MICROS 1176
#define EXCHANGE_BEACON_MICROS 333
#define NO_SIGNAL_MICROS 2000



/*---------------- Module Function Prototypes ---------------*/
void rise_detected();

/*---------------- Module Level Variables -------------------*/
volatile unsigned long previousTime = 0;
volatile unsigned long period = NO_SIGNAL_MICROS; 

/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The Tape_Stop_Test program has started!");
  pinMode(TAPE_INPUT_PIN, INPUT);
  WinkleInit();  
  attachInterrupt(BEACON_INTERRUPT_NUMBER, rise_detected, RISING);
  SpinRight(5);
 // attachInterrupt(1, fall_detected, FALLING);
  
}

void loop() {
  unsigned long currentTime = micros(); 
  
  if(currentTime >= NO_SIGNAL_MICROS){
    period = NO_SIGNAL_MICROS;
  }
  unsigned long currentPeriod = period;
  if(currentPeriod < SERVER_BEACON_MICROS + 10 || currentPeriod > SERVER_BEACON_MICROS - 10){
    SpinRight(0);
    DriveForward(2);
    Serial.println("Beacon IN range");
    return;
  }
  
}

/*---------------- Module Functions -------------------------*/

void rise_detected() {
  unsigned long currentTime = micros(); 
  period = currentTime - previousTime;
  previousTime = currentTime; 
}  
  





