/**************************************************************
  File:      MotorDriving.pde 
  Contents:  Motor test
             
  Notes:    Target: Arduino UNO R1 & R2
            Arduino IDE version: 0022
            
  History:
  when      who  what/why
  ----      ---  -------------------------------------------
  03/7/14  HMK  program created
  
 /*---------------- Includes ---------------------------------*/

/*---------------- Module Defines ---------------------------*/

// DEFINE TERMS
#define SPEED_SCALER             1 // map 0-255 PWM settings to 0-10 speed settings
#define TRAVELING_SPEED          200 // Left is speed + 1, right is speed - 1

// DEFINE PINS
#define L_MOTOR_DIR              8
#define L_MOTOR_EN               6 // CHANGED FROM 9 because servo library takes over 9 and 10
#define R_MOTOR_DIR              12   //moved this from 10 to 12 in order to give the pin_Servo 9 or 10	
#define R_MOTOR_EN               11



/*---------------- Module Function Prototypes ---------------*/


/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The MotorForwardButtonTest program has started!");
  pinMode(L_MOTOR_DIR, OUTPUT);
  pinMode(R_MOTOR_DIR, OUTPUT);
  pinMode(L_MOTOR_EN, OUTPUT);
  pinMode(R_MOTOR_EN, OUTPUT);

  
}

void loop() { 
DriveForwardCorrected(TRAVELING_SPEED);
      
  }
}

/******************************************************************************
  Function:    Drive
  Contents:    Defines the functions for left and right motor, and forward, backwards, stop
  Parameters:  state
  Returns:     Nothing
  Notes:    
******************************************************************************/ 
void DriveBackwardCorrected(char newSpeed){  // Ideal speed in testing was Right Motor 9, left motor 7
	LeftMtrSpeed(-1 * (newSpeed + 1));
	RightMtrSpeed(-1 * (newSpeed - 1));
}
void DriveForwardCorrected(char newSpeed){
	LeftMtrSpeed((newSpeed + 1));
	RightMtrSpeed((newSpeed - 1));
}
void Stop() {
  LeftMtrSpeed(0);
  RightMtrSpeed(0);
}

void LeftMtrSpeed(char newSpeed) {
  if (newSpeed < 0) {
    digitalWrite(L_MOTOR_DIR,LOW); // set the direction to reverse
  } else {
    digitalWrite(L_MOTOR_DIR,HIGH); // set the direction to forward
  }
  analogWrite(L_MOTOR_EN,SPEED_SCALER*abs(newSpeed));
}

void RightMtrSpeed(char newSpeed){
  if (newSpeed < 0) {
    digitalWrite(R_MOTOR_DIR,LOW); // set the direction to reverse
  } else {
    digitalWrite(R_MOTOR_DIR,HIGH); // set the direction to forward
  }
  analogWrite(R_MOTOR_EN,SPEED_SCALER*abs(newSpeed));
}

