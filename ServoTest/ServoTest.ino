//**************************************************************
//  File:      ButtonPressTest.pde 
//  Contents:  This program operates the push-button motor 
//    an H-bridge
//    (based on Lab3 Part 4 code)
//  Notes:    Target: Arduino UNO R1 & R2
//            Arduino IDE version: 0022
//
//  History:
//  when      who  what/why
//  ----      ---  -------------------------------------------
//  02/27/14  HMK  Program created
//  
//**************************************************************/

/*---------------- Includes ---------------------------------*/

/*---------------- Module Defines ---------------------------*/

/*---------------- Module Function Prototypes ---------------*/

//unsigned char theKey;
//unsigned char direct;
int SERVO = 9;
//int DIR1 = 12;
//int ENABLE2 = 5;
//int DIR2 = 4;
//void RespToKey(void);

/*---------------- Module Level Variables ---------------*/

/*---------------- Arduino Main Functions -------------------*/
void setup() { 
  Serial.begin(9600);
//  Serial.println("Starting MotorTest...");
  pinMode(SERVO, OUTPUT);
//  pinMode(DIR1, OUTPUT);
//  pinMode(ENABLE2, OUTPUT);
//  pinMode(DIR2, OUTPUT);
//  direct = HIGH;

}

void loop() {  
  analogWrite(SERVO, 240);
//    RunMotor();
//    if (TestForKey()) RespToKey(); 
     
}
      
/*---------------- Module Functions -------------------------*/

//void RunMotor(void) {
//    digitalWrite(DIR1, direct);
////    digitalWrite(DIR2, direct);
//    digitalWrite(ENABLE1, HIGH);
////    digitalWrite(ENABLE2, HIGH);
//}
//
//unsigned char TestForKey(void) {
//  unsigned char KeyEventOccurred; 
//  KeyEventOccurred = Serial.available();
//}
//  
//void RespToKey(void) {
//  Serial.read();
//  ReverseDir();
//  }
//  
//void ReverseDir(void) {
//  direct = digitalRead(DIR1);
////  Serial.println(direct);
//  if (direct == HIGH){
//  direct = LOW;
//  digitalWrite(DIR1, LOW);
////  digitalWrite(DIR2, LOW);
//  Serial.println("pull in");
//  }
//  else {
//  direct = HIGH;
//  digitalWrite (DIR1, HIGH);
////  digitalWrite (DIR2, HIGH);
//  Serial.println("push out");
//  }
//}
