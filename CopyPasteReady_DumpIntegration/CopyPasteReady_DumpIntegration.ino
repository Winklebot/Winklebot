
/**************************************************************
  File:      CopePasteReady_DumpIntegration:
            
  Contents:  Isolates what portion of "Button_Push_Dump_Integration.ino" 
            needs to be added to "Beacon_Orientation_Test.ino" for full
            integration
             
  Notes:    Target: Arduino UNO R1 & R2
            Arduino IDE version: 0022
            
**************************************************************/
// --- ADD TO Includes
#include <Servo.h>

// ---- ADD TO MOVING TIMES -----
#define BUTTON_PRESS_MILLIS    333

// ---- STATES ----
#define ADJUSTING                100
#define FINDING_SERVER           101  // using "100" to avoid state overlap with other codes.
#define PRESSING_SEQUENCE        102
#define PREPARING_BUT_PRESS      103
#define COUNTING_BUTTONS         104
#define MOVING_TOWARDS_EXCHANGE  301 // servo dumping code
#define DUMPING                  302

// ----- TERMS ----
#define BUMPERHIT                0
#define BUMPEROPEN               1
#define SPEED_SCALER             25 // map 0-255 PWM settings to 0-10 speed settings
//#define TRAVELING_SPEED_LEFT     9
//#define TRAVELING_SPEED_RIGHT    7
#define NOMINAL_TRAVELING_SPEED          8 // Left is speed + 1, right is speed - 1

#define DUMP_ANGLE               90
#define RESET_RAMP               9
#define DUMP_TIME                2500

/*---------------- Module Function Prototypes ---------------*/
unsigned char TestTimerExpired(void);
void DropCoins(void);

/*---------------- Module Level Variables -------------------*/
int LFB = BUMPEROPEN;  // set initial bumper reading to open
int RFB = BUMPEROPEN;
int LBB = BUMPEROPEN;
int RBB = BUMPEROPEN;

int coinMax = 3; // total number of button presses needed to get desired coins (2 coins = 3 presses)
                // using "<=" this number for count threshold, so should avoid an off-by-one error

int ExchangeButtonCounter = 0;
int dumped = 0;

/*---------------- Arduino Main Functions -------------------*/
// Add to void setup() {
  myservo.attach(10);
  myservo.write(9);

  
  
//-------------------------------------------------- Driving Functions
// Add to void Init(){
  pinMode(pin_Servo, OUTPUT);
}


// --- ADD COIN DUMPING ---
// Update "state = 100;" to "ADJUSTING"

case(ADJUSTING):
      CheckBumpers();
     if(LBB != RBB){ // if bumpers are not the same, then only one side hit wall
      if((LBB == BUMPEROPEN) && (RBB == BUMPERHIT)) {  // right back hit, left back not, need to twist left towards wall
      LeftMtrSpeed(-1 * NOMINAL_TRAVELING_SPEED);
      RightMtrSpeed(0);
      }
      else {  // left back hit, right back not, need to twist right towards wall
      RightMtrSpeed(-1 * NOMINAL_TRAVELING_SPEED);
      LeftMtrSpeed(0);
      }
     }
     if((LBB == BUMPERHIT) && (RBB == BUMPERHIT)) {
      Serial.println("PRESSING_SEQUENCE");
      ChangeState(PRESSING_SEQUENCE);
     }
      break;
  case(PRESSING_SEQUENCE) :
     ButtonPressingSequence();
     ChangeState(MOVING_TOWARDS_EXCHANGE);
      break;
  case(MOVING_TOWARDS_EXCHANGE) :
    CheckBumpers();
    if ((LFB == BUMPERHIT) && (RFB == BUMPERHIT) && (dumped == 0)) {     //both rear bumpers depressed, stop motors
      dumped = 1;
      DriveForwardCorrected(0);
      DropCoins();
      Serial.println("both bumpers pressed, dumped is ");
      Serial.println(dumped);
      ChangeState(DUMPING);
    }
    else if ((LFB == BUMPERHIT) && (dumped == 0)) {           //left rear bumper depressed, run only right motor
       LeftMtrSpeed(0);
       RightMtrSpeed(NOMINAL_TRAVELING_SPEED);
    }
    else if ((RFB == BUMPERHIT) && (dumped == 0)) {           //right rear bumper depressed, run only left motor
       RightMtrSpeed(0);
       LeftMtrSpeed(NOMINAL_TRAVELING_SPEED);
    }
    break;
      
  }
}

// --- MODULE FUNCTIONS 
/*---------------- Module Functions -------------------------*/

void DropCoins(void) {
   Serial.println("dropping!");
   myservo.write(90);
   delay(2500);
   myservo.write(9);
}

void ChangeState(int newState){
  Serial.print("Changing state to ");
  Serial.println(newState);  
  state = newState;
  SetMotors(newState);
}


/******************************************************************************
  Function:    ButtonPressingSequence
  Contents:    Runs button pressing sequence
  Parameters:  PRESSING_BUTTON
  Returns:     Exchange counter
  Notes:    
******************************************************************************/
void ButtonPressingSequence(){
   Serial.println("Button pressing!");
   ExchangeButtonCounter = 1; //button has already been pushed once
   while (ExchangeButtonCounter <= coinMax){
      Serial.println("Driving forward");
      DriveForwardCorrected(NOMINAL_TRAVELING_SPEED);  // drive forward, away from server
      delay(BUTTON_PRESS_MILLIS);
      Serial.println("Driving backward");
      DriveForwardCorrected(0); 
      DriveBackwardCorrected(NOMINAL_TRAVELING_SPEED);
      delay(BUTTON_PRESS_MILLIS*2); // drive into button for twice the time spend driving away, to ensure you make contact
     if(LBB == BUMPERHIT || RBB == BUMPERHIT){
       ExchangeButtonCounter = ExchangeButtonCounter + 1;   
     }
  }
}
  
/******************************************************************************
  Function:    CheckBumpers
  Contents:    Checks all four bumpers, sets a variable with each
  Parameters:  Called in states ADJUSTING and MOVING_TOWARDS_EXCHANGE
  Returns:     int 0 or 1 value to indicate if bumper hit or open
  Notes:    
******************************************************************************/
void CheckBumpers(){
  LFB = digitalRead(LEFT_FRONT_BUMPER);
  RFB = digitalRead(RIGHT_FRONT_BUMPER);
  LBB = digitalRead(LEFT_BACK_BUMPER);
  //Serial.println(LBB);
  RBB = digitalRead(RIGHT_BACK_BUMPER);
  //Serial.println(RBB);
}



// ---- Add to "Set Motors" switch cases:
     case(MOVING_TOWARDS_TAPE) :
       DriveBackwardCorrected(NOMINAL_TRAVELING_SPEED);
       break;
    case(FINDING_SERVER):
      DriveBackwardCorrected(NOMINAL_TRAVELING_SPEED);
      break;
    case(PRESSING_SEQUENCE):
      Stop();
      break;
    case(MOVING_TOWARDS_EXCHANGE):
      DriveForwardCorrected(NOMINAL_TRAVELING_SPEED);
      break;
