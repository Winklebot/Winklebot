


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
/*---------------- Module Defines ---------------------------*/
#define TAPE_INPUT_PIN 2
#define BEACON_INPUT_PIN 3
#define BEACON_INTERRUPT_NUMBER 1
#define TAPE_INTERRUPT_NUMBER 0  
  
#define SERVER_BEACON_MICROS 1176
#define EXCHANGE_BEACON_MICROS 333
#define NO_SIGNAL_MICROS 2000


/*---------------- Module Function Prototypes ---------------*/
void riseDetected();
unsigned char TestForKey(void);
void RespToKey();
/*---------------- Module Level Variables -------------------*/
volatile unsigned long previousTime = 0;
volatile unsigned long period = NO_SIGNAL_MICROS; 
/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The Beacon_Presence_Detection_Test program has started!");
  pinMode(BEACON_INPUT_PIN, INPUT);
  attachInterrupt(1, riseDetected, RISING);  
}

void loop() {
  unsigned long currentTime = micros(); 
  if(currentTime >= NO_SIGNAL_MICROS){
    period = NO_SIGNAL_MICROS;
  }
  if(TestForKey()) RespToKey();
}

/*---------------- Module Functions -------------------------*/

void riseDetected() {
  unsigned long currentTime = micros(); 
  period = currentTime - previousTime;
  previousTime = currentTime; 
  
}  


unsigned char TestForKey(void) {
  unsigned char KeyEventOccurred;
  
  KeyEventOccurred = Serial.available();
  return KeyEventOccurred;
}

void RespToKey() {
  unsigned char theKey;
  theKey = Serial.read();
  unsigned long currentPeriod = period; // make a copy so it doesn't get changed by interrupts
  if(currentPeriod > SERVER_BEACON_MICROS + 10 || currentPeriod < SERVER_BEACON_MICROS - 10){
    Serial.println("Server Beacon out of range");
  }
  else{
    Serial.println("Server Beacon in range");
  }
  Serial.print("Period: ");
  Serial.println(currentPeriod);
  Serial.println("--------------------------");
} 
    
 


  





